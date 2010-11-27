#include <stdio.h>
#include <stdlib.h>
#include <gelf.h>

#include "register.h"
#include "memory.h"
#include "process.h"
#include "CPU.h"

int main(int argc, char **argv){
  if(argc !=2){
    fprintf(stderr, "usage: %s file-name\n", argv[0]);
    exit(1);
  }
  PROCESS* proc;
  CPU_d* cpu;
  proc = proc_initial(argv[1]);
  cpu = CPU_initial(1, CPU_TRAP);
  CPU_load_process(cpu, proc);

  CPU_next_beat(cpu);

  CPU_destroy(cpu);
  proc_destroy(proc);

  /*
  PROCESS* proc;
  CACHE* cache;
  if(argc !=2){
    fprintf(stderr, "usage: %s file-name\n", argv[0]);
    exit(1);
  }
  proc = proc_initial(argv[1]);
  
  cache = cache_initial(proc->mem);

  if(proc!=NULL)
    proc_destroy(proc);
  */


/*
  ELF_initial(argv[1]);
  printf("main entry:%x\n", ELF_main_entry());
  ELF_close();
*/

  /* REGISTER module
  REGISTERS* regs;
  regs = reg_initial();
  reg_setN(regs, 2);
  printf("N=%d\n", reg_getN(regs));
  reg_destroy(regs);
  */

  /* stack module
  int a;
  int b;
  PROC_STACK* stack;
  stack = initial_stack(4096);
  a = 10;
  stack_pop(stack, &a, sizeof(a));
  stack_push(stack, &a, sizeof(a));
  stack_pop(stack, &b, sizeof(b));
  printf("%d\n", b);
  destroy_stack(stack);;
  */

  /* MEM module
  PROC_MEM* mem;
  int data1[10]={0,1,2,3,4,5,6,7,8,9};
  int data2[10]={0,-1,-2,-3,-4,-5,-6,-7,-8,-9};
  int result;
  mem = initial_mem(2);
  segment_load(mem, 0, 10000, 1024, (void*) data1, 10*sizeof(int));
  segment_load(mem, 1, 20000, 1024, (void*) data2, 10*sizeof(int));
  mem_fetch(mem, 10064, &result, sizeof(int));
  printf("%d\n", result);
  int p = 100;
  mem_set(mem, 10064, &p, sizeof(int));
  mem_fetch(mem, 10064, &result, sizeof(int));
  printf("%d\n", result);
  destroy_mem(mem);
  */

  /*
  REGISTERS *regs = malloc(sizeof(REGISTERS));
  inst_inter(0xFF000000,regs);
  */
  return 0;
}
