#ifndef _CPU_H_
#define _CPU_H_

#include <stdio.h>
#include "process.h"
#include "register.h"
#include "cache.h"
#include "pipline.h"

#define CPU_TRAP	0
#define CPU_NORMAL	1

typedef struct{
  int cycles_total;//total cycles of cpu
  int cycles_idle;//idle cycles of cpu, include bubles and cache miss
  int bubbles;//bubbles of pipline
  int rd_mem_times;//times of read memory
  int wr_mem_times;//times of write memory
  int cache_visit;//times of cache visit
  int cache_miss;//times of cache miss
}CPU_info;

typedef struct{
  int cpu_id;
  int mode;
  REGISTERS* regs;
  CACHE *i_cache, *d_cache;
  PIPLINE * pipline;
  PROCESS* proc;
  CPU_info* cpu_info;
}CPU_d;

extern CPU_d* CPU_initial(int id, int mode);
extern int CPU_destroy(CPU_d* cpu);
extern int CPU_load_process(CPU_d* cpu, PROCESS* proc);
extern int CPU_next_beat(CPU_d* cpu);
extern int CPU_run(CPU_d* cpu, int cycles);
extern int CPU_show_info(CPU_d* cpu);
#endif
