#ifndef _REGISTER_H_
#define _REGISTER_H_

#include <stdint.h>

#define REG_BL r[30]
#define REG_PC r[31]
#define REG_CMSR flag

typedef struct{
  int32_t r[32];
  int32_t flag;
}REGISTERS;

extern REGISTERS* reg_initial();
extern void reg_destroy(REGISTERS *regs);
extern int reg_setN(REGISTERS *regs, int N);
extern int reg_getN(REGISTERS *regs);
extern int reg_setZ(REGISTERS *regs, int Z);
extern int reg_getZ(REGISTERS *regs);
extern int reg_setC(REGISTERS *regs, int C);
extern int reg_getC(REGISTERS *regs);
extern int reg_setV(REGISTERS *regs, int V);
extern int reg_getV(REGISTERS *regs);

#endif
