#include "CPU.h"
#include "register.h"
#include "pipline.h"
#include <malloc.h>
#include <memory.h>

#include "debugger.h"

CPU_d* CPU_initial(int id, int mode){
  CPU_d* cpu = malloc(sizeof(CPU_d));
  memset(cpu, 0, sizeof(CPU_d));
  cpu->cpu_id = id;
  cpu->mode = mode;
  cpu->regs = reg_initial();
  cpu->i_cache = cache_initial(NULL);
  cpu->d_cache = cache_initial(NULL);
  cpu->pipline = pipline_initial(cpu->regs, cpu->i_cache,
				 cpu->d_cache);
  cpu->cpu_info = malloc(sizeof(CPU_info));
  memset(cpu->cpu_info, 0, sizeof(CPU_info));
  return cpu;
}

int CPU_destroy(CPU_d* cpu){
  reg_destroy(cpu->regs);
  cache_destroy(cpu->i_cache);
  cache_destroy(cpu->d_cache);
  pipline_destroy(cpu->pipline);
  free(cpu->cpu_info);
  return 1;
}

int CPU_load_process(CPU_d* cpu, PROCESS* proc){
  cpu->proc = proc;
  cache_mem_link(cpu->i_cache, proc->mem);
  cache_mem_link(cpu->d_cache, proc->mem);
  cpu->regs->REG_PC = proc->entry;
  cpu->regs->REG_SP = (uint32_t)proc->stack->vaddr_offset + (uint32_t)(proc->stack->size * 4);
  cpu->pipline->stack = proc->stack;
  return 1;
}

int CPU_next_beat(CPU_d* cpu){
  int i;
  for(i=0; i<1; i++)
    pipline_next_step(cpu->pipline, cpu->cpu_info);
  debugger_print_cpu_info(cpu->cpu_info);
  
//  pipline_next_step(cpu->pipline, cpu->cpu_info);
  return 1;
}

int CPU_run(CPU_d* cpu, int cycles){

  return 0;
}
