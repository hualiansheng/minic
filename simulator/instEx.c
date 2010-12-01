#include "instEx.h"
#include "register.h"
#include <stdio.h>

void inst_Ex_D_Imm_Shift(PIPLINE* pipline, int level);
void inst_Ex_D_Reg_Shift(PIPLINE* pipline, int level);
void inst_Ex_Multiply(PIPLINE* pipline, int level);
void inst_Ex_Branch_Ex(PIPLINE* pipline, int level);
void inst_Ex_D_Immediate(PIPLINE* pipline, int level);
void inst_Ex_L_S_R_offset(PIPLINE* pipline, int level);
void inst_Ex_L_S_Hw_Sb_Rof(PIPLINE* pipline, int level);
void inst_Ex_L_S_Hw_Sb_Iof(PIPLINE* pipline, int level);
void inst_Ex_L_S_I_offset(PIPLINE* pipline, int level);
void inst_Ex_Branch_Link(PIPLINE* pipline, int level);
void inst_Ex_ST(PIPLINE* pipline, int level);
void inst_Unknown(PIPLINE* pipline, int level);
int sign_extend(int imm, int size);
int32_t shift_Ex(int32_t* operand, int imm, int shift_type);
int32_t data_Ex(int32_t operand1, int32_t operand2, int opcodes, int carry);
void set_CMSR(REGISTERS* regs, int32_t operand1, int32_t operand2,
	      int32_t result, int opcodes, int shift_carry);
int logical_shift_left(int32_t* operand, int imm);
int logical_shift_right(int32_t* operand, int imm);
int math_shift_right(int32_t* operand, int imm);
int rotate_shift_right(int32_t* operand, int imm);

int inst_Ex(PIPLINE* pipline, CPU_info* cpu_info, int level){
  if(pipline->pipline_data[level]->inst_type == MULTIPLY
     && cpu_info->cycles_total <= 2)
    cpu_info->cycles_total = 2;
  switch(pipline->pipline_data[level]->inst_type){
  case D_IMM_SHIFT:
    inst_Ex_D_Imm_Shift(pipline, level);
    break;
  case D_REG_SHIFT:
    inst_Ex_D_Reg_Shift(pipline, level);
    break;
  case MULTIPLY:
    inst_Ex_Multiply(pipline, level);
    break;
  case BRANCH_EX:
    inst_Ex_Branch_Ex(pipline, level);
    break;
  case D_IMMEDIATE:
    inst_Ex_D_Immediate(pipline, level);
    break;
  case L_S_R_OFFSET:
    inst_Ex_L_S_R_offset(pipline, level);
    break;
  case L_S_HW_SB_ROF:
    inst_Ex_L_S_Hw_Sb_Rof(pipline, level);
    break;
  case L_S_HW_SB_IOF:
    inst_Ex_L_S_Hw_Sb_Iof(pipline, level);
    break;
  case L_S_I_OFFSET:
    inst_Ex_L_S_I_offset(pipline, level);
    break;
  case BRANCH_LINK:
    inst_Ex_Branch_Link(pipline, level);
    break;
  case ST:
    inst_Ex_ST(pipline, level);
    break;
  default:
    inst_Unknown(pipline, level);
    return 0;
  }
  return 1;
}

void inst_Ex_D_Imm_Shift(PIPLINE* pipline, int level){
  printf("----Execuating Ex_D_Imm_Shift instruction : 0x%.8x.----\n", pipline->pipline_data[level]->inst_code);
  PIPLINE_DATA* data = pipline->pipline_data[level];
  int32_t operand1, operand2;
  int32_t result;
  int carry, shift_carry;
  // extend immediate
  data->imm = sign_extend(data->imm, 5);
  // fetch operands
  operand1 = pipline->regs->r[data->Rn];
  operand2 = pipline->regs->r[data->Rm];
  // shift operand2 and return shift_carry,
  // shift_carry means whether shift operation is carried
  shift_carry = shift_Ex(&operand2, data->imm, data->shift);
  // get register heap C bit in CMSR
  carry = reg_getC(pipline->regs);
  // Caculate result
  result = data_Ex(operand1, operand2, data->opcodes, carry);
  // Some opcodes will not write back
  if(data->opcodes != CAND && data->opcodes != CXOR
     && data->opcodes != CSUB && data->opcodes != CADD
     && data->opcodes != MOV && data->opcodes != MVN)
    pipline->regs->r[data->Rd] = result;
  // MOV and MOV will write back when conditional
  // And operand1 is conditional
  else if(data->opcodes == MOV || data->opcodes == MVN){
    if(operand1 == 0)/* Question5 : how operand1 becomes conditional? */
      pipline->regs->r[data->Rd] = result;
  }
  // Other condition will write back
  else
    pipline->regs->r[data->Rd] = result;
  // Set CMSR when S bit is 1 and Rd is not PC
  if(data->S == 1 && data->Rd != 31)
    set_CMSR(pipline->regs, operand1, operand2,
	     result, data->opcodes, shift_carry);
  return;
}

void inst_Ex_D_Reg_Shift(PIPLINE* pipline, int level){
  printf("----Execuating Ex_D_Reg_Shift instruction : 0x%.8x.----\n", pipline->pipline_data[level]->inst_code);
}

void inst_Ex_Multiply(PIPLINE* pipline, int level){
  printf("----Execuating Ex_Multiply instruction : 0x%.8x.----\n", pipline->pipline_data[level]->inst_code);
}

void inst_Ex_Branch_Ex(PIPLINE* pipline, int level){
  printf("----Execuating Ex_Branch_Ex instruction : 0x%.8x.----\n", pipline->pipline_data[level]->inst_code);
}

void inst_Ex_D_Immediate(PIPLINE* pipline, int level){
  printf("----Execuating Ex_D_Immediate instruction : 0x%.8x.----\n", pipline->pipline_data[level]->inst_code);
}

void inst_Ex_L_S_R_offset(PIPLINE* pipline, int level){
  printf("----Execuating Ex_L_S_R_offset instruction : 0x%.8x.----\n", pipline->pipline_data[level]->inst_code);
}

void inst_Ex_L_S_Hw_Sb_Rof(PIPLINE* pipline, int level){
  printf("----Execuating Ex_L_S_Hw_Sb_Rof instruction : 0x%.8x.----\n", pipline->pipline_data[level]->inst_code);
}

void inst_Ex_L_S_Hw_Sb_Iof(PIPLINE* pipline, int level){
  printf("----Execuating Ex_L_S_Hw_Sb_Iof instruction : 0x%.8x.----\n", pipline->pipline_data[level]->inst_code);
}

void inst_Ex_L_S_I_offset(PIPLINE* pipline, int level){
  printf("----Execuating Ex_L_S_I_offset instruction : 0x%.8x.----\n", pipline->pipline_data[level]->inst_code);
}

void inst_Ex_Branch_Link(PIPLINE* pipline, int level){
  printf("----Execuating Ex_Branch_Link instruction : 0x%.8x.----\n", pipline->pipline_data[level]->inst_code);
}

void inst_Ex_ST(PIPLINE* pipline, int level){
  printf("----Execuating Ex_Ex_ST instruction : 0x%.8x.----\n", pipline->pipline_data[level]->inst_code);
}

void inst_Unknown(PIPLINE* pipline, int level){
  printf("----Execuating Unknown instruction : 0x%.8x.----\n",
	 pipline->pipline_data[level]->inst_code);
}

int sign_extend(int imm, int size){
  return (imm << (32-size)) >> (32-size);
}

int shift_Ex(int32_t* operand, int imm, int shift_type){
  switch(shift_type){
  case 0:
    return logical_shift_left(operand, imm);
  case 1:
    return logical_shift_right(operand, imm);
  case 2:
    return math_shift_right(operand, imm);
  case 3:
    return rotate_shift_right(operand, imm);
  }
  return 0;
}

int32_t data_Ex(int32_t operand1, int32_t operand2, int opcodes, int carry){
  int32_t op1 = operand1;
  int32_t op2 = operand2;
  switch(opcodes){
  case AND: 
  case CAND: return op1 & op2;
  case XOR:
  case CXOR: return op1 ^ op2;
  case SUB:
  case CSUB: return op1 - op2;
  case RSB:  return op2 - op2;
  case ADD:
  case CADD: return op1 + op2;
  case ADC:  return op1 + op2 + carry;
  case SBC:  return op1 - op2 + carry - 1;
  case RSC:  return op2 - op1 + carry - 1;
  case ORR:  return op1 | op2;
  case MOV:  return op2;
  case CLB:  return op1 & (~op2);
  case MVN:  return ~op2;
  }
  fprintf(stderr, "Data Ex failed when executing data instruction.\n");
  return op1;
}

// set_CMSR : set CMSR when execuating instruction
// regs -- register heap
// operand1 -- the first operand
// operand2 -- the second operand
// result -- the result of the ALU operation
// opcodes -- the type of the ALU operation
// shift_carry -- show the carry of shift operation
void set_CMSR(REGISTERS* regs, int32_t operand1, int32_t operand2,
	      int32_t result, int opcodes, int shift_carry){
  if(opcodes == AND || opcodes == XOR || opcodes == CAND ||
     opcodes == CXOR || opcodes == ORR || opcodes == MOV ||
     opcodes == CLB || opcodes == MVN){
    if(opcodes == MOV || opcodes == MVN)
      if(operand1 == 0)
	return;
    //set CMSR C bit
    if(shift_carry != -1)
      reg_setC(regs, shift_carry);
    //set CMSR Z bit
    if(result == 0)
      reg_setZ(regs, 1);
    else
      reg_setZ(regs, 0);
    //set CMSR N bit
    if(result < 0)
      reg_setN(regs, 1);
    else
      reg_setN(regs, 0);
  }else{
    // set CMSR V bit
    if(opcodes == ADD || opcodes == ADC || opcodes == CADD){
      if((operand1 > 0 && operand2 > 0 && result < 0) ||
	 (operand1 < 0 && operand2 < 0 && result > 0))
	reg_setV(regs, 1);
      else
	reg_setV(regs, 0);
    }
    else if(opcodes == SUB || opcodes == SBC || opcodes == CSUB){
      if((operand1 > 0 && operand2 < 0 && result < 0) ||
	 (operand1 < 0 && operand2 > 0 && result > 0))
	reg_setV(regs, 1);
      else
	reg_setV(regs, 0);
    }
    else if(opcodes == RSB || opcodes == RSC){
      if((operand1 < 0 && operand2 > 0 && result < 0) ||
	 (operand1 > 0 && operand2 < 0 && result > 0))
	reg_setV(regs, 1);
      else
	reg_setV(regs, 0);
    }
    //set CMSR N bit
    if(result < 0)
      reg_setN(regs, 1);
    else
      reg_setN(regs, 0);
    //set CMSR Z bit
    if(result == 0)
      reg_setZ(regs, 1);
    else
      reg_setZ(regs, 0);
    //set CMSR C bit
    if(opcodes == ADD || opcodes == ADC || opcodes == CADD){
      uint32_t tmp = (uint32_t)operand1 + (uint32_t)operand2;
      if(opcodes == SBC && reg_getC(regs) == 1)
	tmp = tmp + 1;
      if(tmp < (uint32_t)operand1 || tmp < (uint32_t)operand2)
	reg_setC(regs, 1);
      else
	reg_setC(regs, 0);
    }
    else if(opcodes == SUB || opcodes == SBC || opcodes == CSUB){
      uint32_t tmp = (uint32_t)operand1 - 1;
      if(opcodes == SBC && reg_getC(regs) == 1)
	tmp = tmp + 1;
      if(tmp > (uint32_t)operand2)
	reg_setC(regs, 1);
      else
	reg_setC(regs, 0);
    }
    else if(opcodes == RSB || opcodes == RSC){
      uint32_t tmp = (uint32_t)operand2 - 1;
      if(opcodes == RSC && reg_getC(regs) == 1)
	tmp = tmp + 1;
      if(tmp > (uint32_t)operand1)
	reg_setC(regs, 1);
      else
	reg_setC(regs, 0);
    }
  }
}

// logical shift left, imm is shift immediate
int logical_shift_left(int32_t* operand, int imm){
  if(imm == 0)
    return -1;
  int result;
  if(imm >= 32){
    if(imm == 32)
      result = *operand & 1;
    else
      result = 0;
    *operand = 0;
    return result;
  }
  if(((*operand << (imm - 1)) & 0x80000000) == 0)
    result = 0;
  else
    result = 1;
  *operand = *operand << imm;
  return result;
}

// logical shift right, imm is shift immediate
int logical_shift_right(int32_t* operand, int imm){
  if(imm == 0)
    return -1;
  int result;
  if(imm >= 32){
    if(imm == 32)
      result = (*operand >> 31) & 1;
    else
      result = 0;
    *operand = 0;
    return result;
  }
  if(((*operand >> (imm - 1)) & 1) == 0)
    result = 0;
  else
    result = 1;
  *operand = (int32_t)((uint32_t)*operand >> imm);
  return result;
}

// mathematical shift right, imm is shift immediate
int math_shift_right(int32_t* operand, int imm){
  if(imm == 0)
    return -1;
  int result;
  if(imm >= 32){
    result = (*operand >> 31) & 1;
    *operand = *operand >> 31;
    return result;
  }
  if(((*operand >> (imm - 1)) & 1) == 0)
    result = 0;
  else
    result = 1;
  *operand = *operand >> imm;
  return result;
}

// rotate shift right, imm is shift immediate
int rotate_shift_right(int32_t* operand, int imm){
  if(imm == 0)
    return -1;
  int result;
  if(imm % 32 == 0){
    result = (*operand >> 31) & 1;
    return result;
  }
  imm = imm % 32;
  result = (*operand >> (imm-1)) & 1;
  *operand = (int32_t)(((uint32_t)*operand >> imm) |
		       ((uint32_t)*operand << (32 - imm)));
  return result;
}
