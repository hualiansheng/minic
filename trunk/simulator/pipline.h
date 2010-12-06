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
  int opcodes;
  uint32_t Rn, Rd, Rs, Rm;
  int imm;
  int shift;
  int rotate;
  int cond;
  int high_offset, low_offset;
  int S, A, P, U, B, W, L, H;
  uint32_t cur_inst_PC;
  uint32_t addr;
}PIPLINE_DATA;

typedef struct{
  int block;//1 means pipline block, 0 mean the opposite
  int block_reg;
  PIPLINE_DATA* pipline_data[PIPLINE_LEVEL];
  PROC_STACK* stack;
  REGISTERS* regs;
  CACHE *i_cache, *d_cache;
  int drain_pipline;
  int pc_src;
  int ex_begin;
}PIPLINE;

typedef struct{
  int cycles_total;//total cycles of cpu
  //int cycles_idle;//idle cycles of cpu, include bubles and cache miss
  int cycles_work;//work cycles of cpu
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
extern void drain_pipline(PIPLINE* pipline, int level);
extern int sign_extend(int imm, int size);

#endif
