#ifndef _CPU_H_
#define _CPU_H_

#include <stdio.h>
#include "process.h"
#include "register.h"
#include "cache.h"
#include "pipline.h"

#define CPU_TRAP	  0
#define CPU_NORMAL	1
#define CPU_STOP    2
#define CPU_INIT    3


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
