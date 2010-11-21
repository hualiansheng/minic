#ifndef _PIPLINE_H_
#define _PIPLINE_H_

#include <stdint.h>

#define PIPLINE_LEVEL	5

typedef struct{
  uint32_t inst_code;
  uint32_t opcodes;
  int inst_type;
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
  PIPLINE_DATA pipline_data[PIPLINE_LEVEL];
}PIPLINE;

#endif
