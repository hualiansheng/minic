#include "pipline.h"
#include "decode.h"
#include <malloc.h>
#include <memory.h>

#include "memory.h"

int pipline_IF(PIPLINE* pipline, CPU_info* cpu_info);
int pipline_ID(PIPLINE* pipline, CPU_info* cpu_info);
int pipline_Ex(PIPLINE* pipline, CPU_info* cpu_info);
int pipline_Mem(PIPLINE* pipline, CPU_info* cpu_info);
int pipline_WB(PIPLINE* pipline, CPU_info* cpu_info);

PIPLINE* pipline_initial(REGISTERS* regs, CACHE* i_cache,
			 CACHE* d_cache){
  PIPLINE* pipline;
  pipline = malloc(sizeof(PIPLINE));
  memset(pipline, 0, sizeof(PIPLINE));
  pipline->regs = regs;
  pipline->i_cache = i_cache;
  pipline->d_cache = d_cache;
  return pipline;
}

int pipline_destroy(PIPLINE* pipline){
  return 1;
}

int pipline_next_step(PIPLINE* pipline, CPU_info* cpu_info){
  CPU_info* pipline_cpu_info = malloc(sizeof(CPU_info));
  pipline_WB(pipline, pipline_cpu_info);
  cpu_info->cycles_total += pipline_cpu_info->cycles_total;
  cpu_info->cycles_idle += pipline_cpu_info->cycles_idle;
  cpu_info->bubbles += pipline_cpu_info->bubbles;
  cpu_info->rd_mem_times += pipline_cpu_info->rd_mem_times;
  cpu_info->wr_mem_times += pipline_cpu_info->wr_mem_times;
  cpu_info->cache_visit += pipline_cpu_info->cache_visit;
  cpu_info->cache_miss += pipline_cpu_info->cache_miss;
  
  printf("stack base addr : %x\n", (unsigned int)pipline->stack->base);
  printf("==============================\n");

  printf("PC : %x\n", pipline->regs->REG_PC);
  printf("now inst : %x\n", (pipline->pipline_data[0])->inst_code);
  printf("now inst cycles : %d\n", cpu_info->cycles_total);
  printf("===============================\n\n");

  return 0;
}

int pipline_IF(PIPLINE* pipline, CPU_info* cpu_info){
  PIPLINE_DATA* data = malloc(sizeof(PIPLINE_DATA));
  CACHE_RETURN cache_return;
  uint32_t addr = pipline->regs->REG_PC;
  pipline->regs->REG_PC += 4;
  memset(data, 0, sizeof(PIPLINE_DATA));
  data->inst_addr = addr;
  cache_return = cache_search(pipline->i_cache, addr);
  data->inst_code = cache_return.data;
  pipline->pipline_data[0] = data;
  if(cache_return.cpu_cycles == CACHE_MISSED_CYCLE)
    cpu_info->cache_miss ++;
  if(cpu_info->cycles_total < cache_return.cpu_cycles)
    cpu_info->cycles_total = cache_return.cpu_cycles;
  cpu_info->cache_visit ++;
  return 1;
}

int pipline_ID(PIPLINE* pipline, CPU_info* cpu_info){
  if(pipline->pipline_data[1] == NULL){
    pipline_IF(pipline, cpu_info);
    return 0;
  }
  inst_decode(pipline->pipline_data[1]);
  pipline->pipline_data[1] = pipline->pipline_data[0];
  return 1;
}

int pipline_Ex(PIPLINE* pipline, CPU_info* cpu_info){
  if(pipline->pipline_data[2] == NULL){
    pipline_ID(pipline, cpu_info);
    return 0;
  }

  pipline->pipline_data[2] = pipline->pipline_data[1];
  return 1;
}

int pipline_Mem(PIPLINE* pipline, CPU_info* cpu_info){
  if(pipline->pipline_data[3] == NULL){
    pipline_Ex(pipline, cpu_info);
    return 0;
  }

  pipline->pipline_data[3] = pipline->pipline_data[2];
  return 1;
}

int pipline_WB(PIPLINE* pipline, CPU_info* cpu_info){
  if(pipline->pipline_data[4] != NULL)
    free(pipline->pipline_data[4]);
  pipline->pipline_data[4] = pipline->pipline_data[3];
  cpu_info->cycles_total = 1;
  cpu_info->cycles_idle = 0;
  cpu_info->bubbles = 0;
  cpu_info->rd_mem_times = 0;
  cpu_info->wr_mem_times = 0;
  cpu_info->cache_visit = 0;
  cpu_info->cache_miss = 0;
  pipline_Mem(pipline, cpu_info);
  return 1;
}
