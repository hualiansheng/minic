#include "decode.h"
#include <string.h>
#include "interpret.h"
#include "debugger.h"

int interpret_D(PIPLINE_DATA data, char* result);
int interpret_Multiply(PIPLINE_DATA data, char* result);
int interpret_Branch_Ex(PIPLINE_DATA data, char* result);
int interpret_L_S(PIPLINE_DATA data, char* result);
int interpret_Branch_Link(PIPLINE_DATA data, char* result, PROCESS* proc);
int interpret_ST(PIPLINE_DATA data, char* result);
int interpret_unknown(PIPLINE_DATA data, char* result);
int sign_extend(int imm, int size);

int interpret_inst(uint32_t inst_code, uint32_t inst_addr,
		   char* result, PROCESS* proc){
  PIPLINE_DATA data;
  data.inst_code = inst_code;
  data.inst_addr = inst_addr;
  decode_inst_decode(&data);
  //printf("0x%.8x\n", (uint32_t)proc);
  switch(data.inst_type){
  case D_IMM_SHIFT  :interpret_D(data, result);break;
  case D_REG_SHIFT  :interpret_D(data, result);break;
  case MULTIPLY     :interpret_Multiply(data, result);break;
  case BRANCH_EX    :interpret_Branch_Ex(data, result);break;
  case D_IMMEDIATE  :interpret_D(data, result);break;
  case L_S_R_OFFSET :interpret_L_S(data, result);break;
  case L_S_HW_SB_ROF:interpret_L_S(data, result);break;
  case L_S_HW_SB_IOF:interpret_L_S(data, result);break;
  case L_S_I_OFFSET :interpret_L_S(data, result);break;
  case BRANCH_LINK  :interpret_Branch_Link(data, result, proc);break;
  case ST           :interpret_ST(data, result);break;
  default           :interpret_unknown(data, result);
  }
  return 1;
}

int interpret_D(PIPLINE_DATA data, char* result){
  char operand1[50];
  char operand2[50];
  char opcode[20];
  char shift[5];
  switch(data.opcodes){
  case AND  : strcpy(opcode, "and");break;
  case XOR  : strcpy(opcode, "xor");break;
  case SUB  : strcpy(opcode, "sub");break;
  case RSB  : strcpy(opcode, "rsb");break;
  case ADD  : strcpy(opcode, "add");break;
  case ADC  : strcpy(opcode, "adc");break;
  case SBC  : strcpy(opcode, "sbc");break;
  case RSC  : strcpy(opcode, "rsc");break;
  case CAND : strcpy(opcode, "cmpand");break;
  case CXOR : strcpy(opcode, "cmpxor");break;
  case CSUB : strcpy(opcode, "cmpsub");break;
  case CADD : strcpy(opcode, "cmpadd");break;
  case ORR  : strcpy(opcode, "orr");break;
  case MOV  : strcpy(opcode, "mov");break;
  case CLB  : strcpy(opcode, "clb");break;
  case MVN  : strcpy(opcode, "mvn");break;
  default   : strcpy(opcode, "UNKNOWN");break;
  }
  if(data.S == 1)
    strcat(opcode, ".s");
  sprintf(operand1, "r%d", data.Rn);
  if(data.inst_type == D_IMMEDIATE){
    sprintf(operand2, "#%d <> #%d", 
	    sign_extend(data.imm, 9), sign_extend(data.rotate, 5));
  }
  else{
    switch(data.shift){
    case 0:strcpy(shift, "<<");break;
    case 1:strcpy(shift, ">>");break;
    case 2:strcpy(shift, ">");break;
    case 3:strcpy(shift, "<>");break;
    default:break;
    }
    if(data.inst_type == D_IMM_SHIFT)
      sprintf(operand2, "r%d %s #%d", data.Rm,
	      shift, sign_extend(data.imm, 5));
    else
      sprintf(operand2, "r%d %s r%d", data.Rm,
	      shift, data.Rs);
  }
  if(data.opcodes == MOV || data.opcodes == MVN)
    sprintf(result, "%s r%d, %s", opcode, data.Rd, operand2);
  else
    sprintf(result, "%s r%d, %s, %s", opcode, data.Rd, operand1, operand2);
  return 1;
}

int interpret_Multiply(PIPLINE_DATA data, char* result){
  char opcode[20];
  if(data.S == 0 && data.A == 0)
    strcpy(opcode, "mul");
  else if(data.S == 0 && data.A == 0)
    strcpy(opcode, "mla");
  else if(data.S == 1 && data.A == 0)
    strcpy(opcode, "muls");
  else
    strcpy(opcode, "mlas");
  if(data.A == 0)
    sprintf(result, "%s r%d, r%d, r%d", opcode, data.Rd, data.Rm, data.Rn);
  else
    sprintf(result, "%s r%d, r%d, r%d, r%d",
	    opcode, data.Rd, data.Rm, data.Rn, data.Rs);
  return 1;
}

int interpret_Branch_Ex(PIPLINE_DATA data, char* result){
  if(data.L == 0)
    sprintf(result, "b r%d", data.Rm);
  else
    sprintf(result, "b.l r%d", data.Rm);
  return 1;
}

int interpret_Branch_Link(PIPLINE_DATA data, char* result, PROCESS* proc){
  char opcode[20];
  char target[100];
  char tmp[100];
  //printf("0x%.8x\n", (uint32_t)proc);
  switch(data.cond){
  case EQ  : strcpy(opcode, "beq");break;
  case NE  : strcpy(opcode, "bne");break;
  case UGE : strcpy(opcode, "buge");break;
  case ULT : strcpy(opcode, "bult");break;
  case N   : strcpy(opcode, "bn");break;
  case NN  : strcpy(opcode, "bnn");break;
  case OV  : strcpy(opcode, "bov");break;
  case NV  : strcpy(opcode, "bnv");break;
  case UGT : strcpy(opcode, "bugt");break;
  case ULE : strcpy(opcode, "bule");break;
  case SGE : strcpy(opcode, "bsge");break;
  case SLT : strcpy(opcode, "bslt");break;
  case SGT : strcpy(opcode, "bsgt");break;
  case SLE : strcpy(opcode, "bsle");break;
  case AL  : strcpy(opcode, "b");break;
  default  : break;
  }
  int32_t offset = sign_extend(data.imm, 24) * 4;
  uint32_t addr = data.inst_addr + offset + 4;
  if(debugger_search_symtbl(proc, addr, tmp, STT_FUNC) == 0)
    sprintf(target, "<0x%.8x>", addr);
  else
    sprintf(target, "<%s>", tmp);
  if(data.L == 0)
    sprintf(result, "%s 0x%.8x %s", opcode, addr, target);
  else
    sprintf(result, "%s.l 0x%.8x %s", opcode, addr, target);
  return 1;
}

int interpret_L_S(PIPLINE_DATA data, char* result){
  char opcode[20]={0};
  char operand2[50]={0};
  char operand[50]={0};
  char shift[5]={0};
  // interpret op cmd
  if(data.L == 1)
    strcpy(opcode, "ldr");
  else
    strcpy(opcode, "str");
  if(data.inst_type == L_S_R_OFFSET ||
     data.inst_type == L_S_I_OFFSET){
    if(data.B == 0)
      strcat(opcode, "w");
    else
      strcat(opcode, "b");
  }
  else{
    if(data.S == 1)
      strcat(opcode, "s");
    if(data.H == 1)
      strcat(opcode, "h");
    else
      strcat(opcode, "b");
  }
  if(data.inst_type == L_S_R_OFFSET){
    // shift interpret
    switch(data.shift){
    case 0:strcpy(shift, "<<");break;
    case 1:strcpy(shift, ">>");break;
    case 2:strcpy(shift, ">");break;
    case 3:strcpy(shift, "<>");break;
    default:break;
    }
    sprintf(operand2, "r%d, %s#%d",
	    data.Rm, shift, sign_extend(data.imm, 5));
  }
  else if(data.inst_type == L_S_I_OFFSET){
    sprintf(operand2, "#%d", sign_extend(data.imm, 14));
  }
  else if(data.inst_type == L_S_HW_SB_ROF){
    sprintf(operand2, "r%d", data.Rm);
  }
  else{
    int imm = (data.high_offset << 5) + data.low_offset;
    sprintf(operand2, "#%d", sign_extend(imm, 10));
  }
  // generate operand2
  // pre_indexed
  if(data.P == 1){
    if(data.W == 1 && data.U == 1)
      sprintf(operand, "[&r%d+, %s]", data.Rn, operand2);
    else if(data.W == 1 && data.U == 0)
      sprintf(operand, "[&r%d-, %s]", data.Rn, operand2);
    else if(data.W == 0 && data.U == 1)
      sprintf(operand, "[r%d+, %s]", data.Rn, operand2);
    else
      sprintf(operand, "[r%d-, %s]", data.Rn, operand2);
  }
  // post_indexed
  else{
    if(data.U == 1)
      sprintf(operand, "[r%d+], %s", data.Rn, operand2);
    else
      sprintf(operand, "[r%d-], %s", data.Rn, operand2);
  }
  sprintf(result, "%s r%d, %s", opcode, data.Rd, operand);
  return 1;
}

int interpret_ST(PIPLINE_DATA data, char* result){
  sprintf(result, "ST 0x%.8x", data.imm);
  return 1;
}

int interpret_unknown(PIPLINE_DATA data, char* result){
  sprintf(result, ".data 0x%.8x", data.inst_code);
  return 1;
}

