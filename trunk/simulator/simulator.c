#include <stdio.h>
#include <gelf.h>

#include "register.h"
#include "memory.h"
#include "process.h"

int main(int argc, char **argv){
  PROCESS* proc;
  if(argc !=2)
    fprintf(stderr, "usage: %s file-name", argv[0]);
  proc = proc_load(argv[1]);
  if(proc!=NULL)
    proc_destroy(proc);

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
  return 0;
}
