#ifndef _PIPLINE_H_
#define _PIPLINE_H_

#include <stdint.h>
#include "cache.h"
#include "register.h"
#include "process.h"


#define PIPLINE_LEVEL	5

typedef struct{
  uint32_t inst_addr;
  uint32_t inst_code;
  int inst_type;
  uint32_t opcodes;
  uint32_t operand1, operant2;
  int32_t *dest_reg;
  int imm;
  int shift;
  int rotate;
  int cond;
  int high_offset, low_offset;
  int S, A, P, U, B, W, L, H;
}PIPLINE_DATA;

typedef struct{
  int block;//1 means pipline block, 0 mean the opposite
  PIPLINE_DATA* pipline_data[PIPLINE_LEVEL];
  int using_regs[31];
  PROC_STACK* stack;
  REGISTERS* regs;
  CACHE *i_cache, *d_cache;
}PIPLINE;

typedef struct{
  int cycles_total;//total cycles of cpu
  int cycles_idle;//idle cycles of cpu, include bubles and cache miss
  int bubbles;//bubbles of pipline
  int rd_mem_times;//times of read memory
  int wr_mem_times;//times of write memory
  int cache_visit;//times of cache visit
  int cache_miss;//times of cache miss
}CPU_info;
  
extern PIPLINE* pipline_initial(REGISTERS* regs, CACHE* i_cache,
				CACHE* d_cache);
extern int pipline_destroy(PIPLINE* pipline);
extern int pipline_next_step(PIPLINE* pipline, CPU_info* cpu_info);

#endif
