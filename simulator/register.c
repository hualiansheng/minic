#include <unistd.h>
#include "register.h"

#include <stdio.h>

//initial function
int reg_initial(REGISTERS *regs){
  if(regs == NULL)
    return 0;
  int i;
  for(i=0; i<32; i++){
    regs->r[i] = 0;
    regs->flag = 0;
  }
  return 1;
}




//flag operations
//flag N operation
int reg_setN(REGISTERS *regs, int N){
  if(regs == NULL)
    return 0;
  if(N!=0)
    N = 1;
  regs->flag = regs->flag | (N<<31);
  return 1;
}

int reg_getN(REGISTERS *regs){
  if(regs == NULL)
    return 2;
  return (regs->flag)>>31 & 1;
}

//flag Z operation
int reg_setZ(REGISTERS *regs, int Z){
  if(regs == NULL)
    return 0;
  if(Z!=0)
    Z = 1;
  regs->flag = regs->flag | (Z<<30);
  return 1;
}

int reg_getZ(REGISTERS *regs){
  if(regs == NULL)
    return 2;
  return (regs->flag)>>30 & 1;
}

//flag C operation
int reg_setC(REGISTERS *regs, int C){
  if(regs == NULL)
    return 0;
  if(C!=0)
    C = 1;
  regs->flag = regs->flag | (C<<29);
  return 1;
}

int reg_getC(REGISTERS *regs){
  if(regs == NULL)
    return 2;
  return (regs->flag)>>29 & 1;
}

//flag V operation
int reg_setV(REGISTERS *regs, int V){
  if(regs == NULL)
    return 0;
  if(V!=0)
    V = 1;
  regs->flag = regs->flag | (V<<28);
  return 1;
}

int reg_getV(REGISTERS *regs){
  if(regs == NULL)
    return 2;
  return (regs->flag)>>28 & 1;
}
