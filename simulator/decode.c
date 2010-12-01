#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "register.h"
#include "decode.h"

PIPLINE_DATA* decode_pipline_data;

uint32_t get_bits(uint32_t num, int bit_num_high, int bit_num_low){
  uint32_t len = bit_num_high - bit_num_low + 1;
  uint32_t mask = ((1 << len) -1);
  if(bit_num_high > 31 || bit_num_high <0
     || bit_num_low > 31 || bit_num_low <0){
    fprintf(stderr, "Error : fault number of bit.\n");
    exit(1);
  }
  if(bit_num_high < bit_num_low){
    fprintf(stderr, "Error : high bit is lower than low bit.\n");
    exit(1);
  }
  num = num >> bit_num_low;
  num = num & mask;
  return num;
}

int inst_D_Imm_Shift(){
  printf("----Decoder : Inst Type - inst_D_Imm_Shift.----\n");
  PIPLINE_DATA* pd = decode_pipline_data;
  pd->inst_type = D_IMM_SHIFT;
  pd->opcodes = get_bits(pd->inst_code, 28, 25);
  pd->S = get_bits(pd->inst_code, 24, 24);
  pd->Rn = get_bits(pd->inst_code, 23, 19);
  pd->Rd = get_bits(pd->inst_code, 18, 14);
  pd->imm = get_bits(pd->inst_code, 13, 9);
  pd->shift = get_bits(pd->inst_code, 7, 6);
  pd->Rm = get_bits(pd->inst_code, 4, 0);
  return 0;
}

int inst_D_Reg_Shift(){
  printf("----Decoder : Inst Type - inst_D_Reg_Shift.----\n");
  PIPLINE_DATA* pd = decode_pipline_data;
  pd->inst_type = D_REG_SHIFT;
  pd->opcodes = get_bits(pd->inst_code, 28, 25);
  pd->S = get_bits(pd->inst_code, 24, 24);
  pd->Rn = get_bits(pd->inst_code, 23, 19);
  pd->Rd = get_bits(pd->inst_code, 18, 14);
  pd->Rs = get_bits(pd->inst_code, 13, 9);
  pd->shift = get_bits(pd->inst_code, 7, 6);
  pd->Rm = get_bits(pd->inst_code, 4, 0);
  return 0;
}

int inst_Multiply(){
  printf("----Decoder : Inst Type - inst_Multiply.----\n");
  PIPLINE_DATA* pd = decode_pipline_data;
  pd->inst_type = MULTIPLY;
  pd->A = get_bits(pd->inst_code, 25, 25);
  pd->S = get_bits(pd->inst_code, 24, 24);
  pd->Rn = get_bits(pd->inst_code, 23, 19);
  pd->Rd = get_bits(pd->inst_code, 18, 14);
  pd->Rs = get_bits(pd->inst_code, 13, 9);
  pd->Rm = get_bits(pd->inst_code, 4, 0);
  return 0;
}

int inst_Branch_Ex(){
  printf("----Decoder : Inst Type - inst_Branch_Ex.----\n");
  PIPLINE_DATA* pd = decode_pipline_data;
  pd->inst_type = BRANCH_EX;
  pd->L = get_bits(pd->inst_code, 24, 24);
  pd->Rm = get_bits(pd->inst_code, 4, 0);
  return 0;
}

int inst_D_Immediate(){
  printf("----Decoder : Inst Type - inst_D_Immediate.----\n");
  PIPLINE_DATA* pd = decode_pipline_data;
  pd->inst_type = D_IMMEDIATE;
  pd->opcodes = get_bits(pd->inst_code, 28, 25);
  pd->S = get_bits(pd->inst_code, 24, 24);
  pd->Rn = get_bits(pd->inst_code, 23, 19);
  pd->Rd = get_bits(pd->inst_code, 18, 14);
  pd->rotate = get_bits(pd->inst_code, 13, 9);
  pd->imm = get_bits(pd->inst_code, 8, 0);
  return 0;
}

int inst_L_S_R_offset(){
  printf("----Decoder : Inst Type - inst_L_S_R_offset.----\n");
  PIPLINE_DATA* pd = decode_pipline_data;
  pd->inst_type = L_S_R_OFFSET;
  pd->P = get_bits(pd->inst_code, 28, 28);
  pd->U = get_bits(pd->inst_code, 27, 27);
  pd->B = get_bits(pd->inst_code, 26, 26);
  pd->W = get_bits(pd->inst_code, 25, 25);
  pd->L = get_bits(pd->inst_code, 24, 24);
  pd->Rn = get_bits(pd->inst_code, 23, 19);
  pd->Rd = get_bits(pd->inst_code, 18, 14);
  pd->imm = get_bits(pd->inst_code, 13, 9);
  pd->shift = get_bits(pd->inst_code, 7, 6);
  pd->Rm = get_bits(pd->inst_code, 4, 0);
  return 0;
}

int inst_L_S_Hw_Sb_Rof(){
  printf("----Decoder : Inst Type - inst_L_S_Hw_Sb_Rof.----\n");
  PIPLINE_DATA* pd = decode_pipline_data;
  pd->inst_type = L_S_HW_SB_ROF;
  pd->P = get_bits(pd->inst_code, 28, 28);
  pd->U = get_bits(pd->inst_code, 27, 27);
  pd->W = get_bits(pd->inst_code, 25, 25);
  pd->L = get_bits(pd->inst_code, 24, 24);
  pd->Rn = get_bits(pd->inst_code, 23, 19);
  pd->Rd = get_bits(pd->inst_code, 18, 14);
  pd->S = get_bits(pd->inst_code, 7, 7);
  pd->H = get_bits(pd->inst_code, 6, 6);
  pd->Rm = get_bits(pd->inst_code, 4, 0);
  return 0;
}

int inst_L_S_Hw_Sb_Iof(){
  printf("----Decoder : Inst Type - inst_L_S_Hw_Sb_Iof.----\n");
  PIPLINE_DATA* pd = decode_pipline_data;
  pd->inst_type = L_S_HW_SB_IOF;
  pd->P = get_bits(pd->inst_code, 28, 28);
  pd->U = get_bits(pd->inst_code, 27, 27);
  pd->W = get_bits(pd->inst_code, 25, 25);
  pd->L = get_bits(pd->inst_code, 24, 24);
  pd->Rn = get_bits(pd->inst_code, 23, 19);
  pd->Rd = get_bits(pd->inst_code, 18, 14);
  pd->S = get_bits(pd->inst_code, 7, 7);
  pd->H = get_bits(pd->inst_code, 6, 6);
  pd->Rm = get_bits(pd->inst_code, 4, 0);
  pd->high_offset = get_bits(pd->inst_code, 13, 9);
  pd->low_offset = get_bits(pd->inst_code, 4, 0);
  return 0;
}

int inst_L_S_I_offset(){
  printf("----Decoder : Inst Type - inst_L_S_I_offset.----\n");
  PIPLINE_DATA* pd = decode_pipline_data;
  pd->inst_type = L_S_I_OFFSET;
  pd->P = get_bits(pd->inst_code, 28, 28);
  pd->U = get_bits(pd->inst_code, 27, 27);
  pd->B = get_bits(pd->inst_code, 26, 26);
  pd->W = get_bits(pd->inst_code, 25, 25);
  pd->L = get_bits(pd->inst_code, 24, 24);
  pd->Rn = get_bits(pd->inst_code, 23, 19);
  pd->Rd = get_bits(pd->inst_code, 18, 14);
  pd->imm = get_bits(pd->inst_code, 13, 0);
  return 0;
}

int inst_Branch_Link(){
  printf("----Decoder : Inst Type - inst_Branch_Link.----\n");
  PIPLINE_DATA* pd = decode_pipline_data;
  pd->inst_type = BRANCH_LINK;
  pd->cond = get_bits(pd->inst_code, 28, 25);
  pd->L = get_bits(pd->inst_code, 24, 24);
  pd->imm = get_bits(pd->inst_code, 23, 0);
  return 0;
}

int inst_ST(){
  printf("----Decoder : Inst Type - inst_ST.----\n");
  PIPLINE_DATA* pd = decode_pipline_data;
  pd->inst_type = ST;
  pd->imm = get_bits(pd->inst_code, 23, 0);
  return 0;
}

int inst_unknown(){
  PIPLINE_DATA* pd = decode_pipline_data;
  pd->inst_type = -1;
  return 0;
}

int decode_inst_decode(PIPLINE_DATA* _pipline_data){
  uint32_t inst = _pipline_data->inst_code;
  decode_pipline_data = _pipline_data;
  int inst_type =  get_bits(inst, 31, 29);
  int cycles;
  if(inst_type == 0 && get_bits(inst, 8, 8) == 0
     && get_bits(inst, 5, 5) == 0)
    cycles = inst_D_Imm_Shift();
  else if(inst_type == 0 && get_bits(inst, 8, 8) == 0
	  && get_bits(inst, 5, 5) == 1)
    cycles = inst_D_Reg_Shift();
  else if(inst_type == 0 && get_bits(inst, 28, 26) == 0
	  && get_bits(inst, 8, 5) == 9)
    cycles = inst_Multiply();
  else if(inst_type == 0 && get_bits(inst, 28, 25) == 8
	  && get_bits(inst, 23, 5) == 0x7FE09)
    cycles = inst_Branch_Ex();
  else if(inst_type == 1)
    cycles = inst_D_Immediate();
  else if(inst_type == 2 && get_bits(inst, 8, 8) == 0
	  && get_bits(inst, 5, 5) == 0)
    cycles = inst_L_S_R_offset();
  else if(inst_type == 2 && get_bits(inst, 13, 8) == 1
	  && get_bits(inst, 26, 26) == 0 && get_bits(inst, 5, 5) == 1)
    cycles = inst_L_S_Hw_Sb_Rof();
  else if(inst_type == 2 && get_bits(inst, 26, 26) == 1
	  && get_bits(inst, 8, 8) == 1 && get_bits(inst, 5, 5) == 1)
    cycles = inst_L_S_Hw_Sb_Iof();
  else if(inst_type == 3)
    cycles = inst_L_S_I_offset();
  else if(inst_type == 5)
    cycles = inst_Branch_Link();
  else if(get_bits(inst, 31, 24) == 0xFF)
    cycles = inst_ST();
  else{
    fprintf(stderr, "Decoder Error : Unrecognize instruction 0x%x.\n", inst);
    cycles = inst_unknown();
    return -1;
  }
  
  return cycles;
}
