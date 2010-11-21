#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "register.h"
#include "decode.h"

PIPLINE_DATA* pipline_data;

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
  printf("Inst Type : inst_D_Imm_Shift\n");
  
  return 0;
}

int inst_D_Reg_Shift(){
  printf("Inst Type : inst_D_Reg_Shift\n");
  return 0;
}

int inst_Multiply(){
  printf("Inst Type : inst_Multiply\n");
  return 0;
}

int inst_Branch_Ex(){
  printf("Inst Type : inst_Branch_Ex\n");
  return 0;
}

int inst_D_Immediate(){
  printf("Inst Type : inst_D_Immediate\n");
  return 0;
}

int inst_L_S_R_offset(){
  printf("Inst Type : inst_L_S_R_offset\n");
  return 0;
}

int inst_L_S_Hw_Sb_Rof(){
  printf("Inst Type : inst_L_S_Hw_Sb_Rof\n");
  return 0;
}

int inst_L_S_Hw_Sb_Iof(){
  printf("Inst Type : inst_L_S_Hw_Sb_Iof\n");
  return 0;
}

int inst_L_S_I_offset(){
  printf("Inst Type : inst_L_S_I_offset\n");
  return 0;
}

int inst_Branch_Link(){
  printf("Inst Type : inst_Branch_Link\n");
  return 0;
}

int inst_ST(){
  printf("Inst Type : inst_ST\n");
  return 0;
}

int inst_decode(uint32_t inst, PIPLINE_DATA* _pipline_data){
  pipline_data = _pipline_data;
  pipline_data->inst_type = get_bits(inst, 31, 29);
  int inst_type = pipline_data->inst_type;
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
    fprintf(stderr, "Error : Unrecognize instruction.\n");
    exit(1);
  }
  
  return cycles;
}
