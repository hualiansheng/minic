#ifndef _REGISTER_H_
#define _REGISTER_H_

#include <stdint.h>

#define REG_SP r[29]
#define REG_BL r[30]
#define REG_LR r[30]
#define REG_PC r[31]
#define REG_CMSR flag

typedef struct{
    int32_t r[32];
    int32_t flag;
}REGISTERS;

REGISTERS* reg_initial();
void reg_destroy(REGISTERS *regs);
int reg_setN(REGISTERS *regs, int n);
int reg_getN(REGISTERS *regs);
int reg_setZ(REGISTERS *regs, int z);
int reg_getZ(REGISTERS *regs);
int reg_setC(REGISTERS *regs, int c);
int reg_getC(REGISTERS *regs);
int reg_setV(REGISTERS *regs, int v);
int reg_getV(REGISTERS *regs);

#endif
