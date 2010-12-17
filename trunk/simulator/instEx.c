#include "instEx.h"
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

int inst_Ex_D_Imm_Shift(PIPLINE* pipline, int level);
int inst_Ex_D_Reg_Shift(PIPLINE* pipline, int level);
int inst_Ex_Multiply(PIPLINE* pipline, int level);
int inst_Ex_Branch_Ex(PIPLINE* pipline, int level);
int inst_Ex_D_Immediate(PIPLINE* pipline, int level);
int inst_Ex_L_S_R_offset(PIPLINE* pipline, int level);
int inst_Ex_L_S_Hw_Sb_Rof(PIPLINE* pipline, int level);
int inst_Ex_L_S_Hw_Sb_Iof(PIPLINE* pipline, int level);
int inst_Ex_L_S_I_offset(PIPLINE* pipline, int level);
int inst_Ex_Branch_Link(PIPLINE* pipline, int level);
int inst_Ex_ST(PIPLINE* pipline, int level);
int inst_Unknown(PIPLINE* pipline, int level);
//int sign_extend(int imm, int size);
int32_t shift_Ex(int32_t* operand, int imm, int shift_type);
int32_t data_Ex(int32_t operand1, int32_t operand2, int opcodes, int carry);
void set_CMSR(REGISTERS* regs, int32_t operand1, int32_t operand2,
              int32_t result, int opcodes, int shift_carry);
int logical_shift_left(int32_t* operand, int imm);
int logical_shift_right(int32_t* operand, int imm);
int math_shift_right(int32_t* operand, int imm);
int rotate_shift_right(int32_t* operand, int imm);
int branch_judge(PIPLINE* pipline, int cond);

// The entry of instruction execuation
// pipline - Pipline of cpu
// cpu_info - cpu_info to be write when execuating
// level - the leve of pipline to execuation
int inst_Ex(PIPLINE* pipline, CPU_info* cpu_info, int level){
    if(pipline->pipline_data[level]->inst_type == MULTIPLY
       && cpu_info->cycles_total <= 2){
        cpu_info->cycles_total = 2;
        cpu_info->cycles_work = 2;
    }
    else if(cpu_info->cycles_total <= 1){
        cpu_info->cycles_total = 1;
        cpu_info->cycles_work = 1;
    }
    int result;//Save if pipline should be dumped, 1 is true
    switch(pipline->pipline_data[level]->inst_type){
        case D_IMM_SHIFT:
            result = inst_Ex_D_Imm_Shift(pipline, level);
            break;
        case D_REG_SHIFT:
            result = inst_Ex_D_Reg_Shift(pipline, level);
            break;
        case MULTIPLY:
            result = inst_Ex_Multiply(pipline, level);
            break;
        case BRANCH_EX:
            result = inst_Ex_Branch_Ex(pipline, level);
            break;
        case D_IMMEDIATE:
            result = inst_Ex_D_Immediate(pipline, level);
            break;
        case L_S_R_OFFSET:
            result = inst_Ex_L_S_R_offset(pipline, level);
            break;
        case L_S_HW_SB_ROF:
            result = inst_Ex_L_S_Hw_Sb_Rof(pipline, level);
            break;
        case L_S_HW_SB_IOF:
            result = inst_Ex_L_S_Hw_Sb_Iof(pipline, level);
            break;
        case L_S_I_OFFSET:
            result = inst_Ex_L_S_I_offset(pipline, level);
            break;
        case BRANCH_LINK:
            result = inst_Ex_Branch_Link(pipline, level);
            break;
        case ST:
            result = inst_Ex_ST(pipline, level);
            break;
        default:
            result = inst_Unknown(pipline, level);
            break;
    }
    if(result == 1){
        cpu_info->cycles_work -= 2;
        cpu_info->bubbles += 2;
    }
    return 1;
}

// The execuation func of Ex_D_Imm_Shift
// return val -- if pipline need be drained, return 1, else 0
int inst_Ex_D_Imm_Shift(PIPLINE* pipline, int level){
    //printf("----Execuating Ex_D_Imm_Shift instruction : 0x%.8x.----\n",
    //       pipline->pipline_data[level]->inst_code);
    PIPLINE_DATA* data = pipline->pipline_data[level];
    int32_t operand1, operand2;
    int32_t result;
    int carry, shift_carry;
    // instruction statistic
    (pipline->proc->inst_statistic).inst_total ++;
    (pipline->proc->inst_statistic).inst_d_imm_shift ++;
    // extend immediate
    //data->imm = sign_extend(data->imm, 5);
    // fetch operands
    if(data->Rn == 31)
        operand1 = data->cur_inst_PC;
    else
        operand1 = pipline->regs->r[data->Rn];
    if(data->Rm == 31)
        operand1 = data->cur_inst_PC;
    else
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
    else if(data->opcodes == MOV || data->opcodes == MVN){
        if((data->Rn & 0x10) != 0){
            data->Rn = data->Rn &0x0F;
            if(branch_judge(pipline, data->Rn) == 1)
                pipline->regs->r[data->Rd] = result;
        }
        else
            pipline->regs->r[data->Rd] = result;
    }
    // Set CMSR when S bit is 1 and Rd is not PC
    if(data->S == 1 && data->Rd != 31)
        set_CMSR(pipline->regs, operand1, operand2,
                 result, data->opcodes, shift_carry);
    //printf("Rd : %d\n", data->Rd);
    // If Rd is PC, drain pipline of 0 and 1, and return 1
    if(data->Rd == 31){
        pipline->pc_src = 1;
        pipline->drain_pipline = 1;
        return 1;
    }
    return 0;
}

// The execuation func of Ex_D_Reg_Shift
// return val -- if pipline need be drained, return 1, else 0
int inst_Ex_D_Reg_Shift(PIPLINE* pipline, int level){
    //printf("----Execuating Ex_D_Reg_Shift instruction : 0x%.8x.----\n",
    //       pipline->pipline_data[level]->inst_code);
    PIPLINE_DATA* data = pipline->pipline_data[level];
    int32_t operand1, operand2;
    int32_t result;
    int carry, shift_carry;
    // instruction statistic
    (pipline->proc->inst_statistic).inst_total ++;
    (pipline->proc->inst_statistic).inst_d_reg_shift ++;
    // fetch operands
    if(data->Rn == 31)
        operand1 = data->cur_inst_PC;
    else
        operand1 = pipline->regs->r[data->Rn];
    if(data->Rm == 31)
        operand1 = data->cur_inst_PC;
    else
        operand2 = pipline->regs->r[data->Rm];
    // shift operand2 and return shift_carry,
    // shift_carry means whether shift operation is carried
    shift_carry = shift_Ex(&operand2, pipline->regs->r[data->Rs], data->shift);
    // get register heap C bit in CMSR
    carry = reg_getC(pipline->regs);
    // Caculate result
    result = data_Ex(operand1, operand2, data->opcodes, carry);
    // Some opcodes will not write back
    if(data->opcodes != CAND && data->opcodes != CXOR
       && data->opcodes != CSUB && data->opcodes != CADD
       && data->opcodes != MOV && data->opcodes != MVN)
        pipline->regs->r[data->Rd] = result;
    else if(data->opcodes == MOV || data->opcodes == MVN){
        if((data->Rn & 0x10) != 0){
            data->Rn = data->Rn &0x0F;
            if(branch_judge(pipline, data->Rn) == 1)
                pipline->regs->r[data->Rd] = result;
        }
        else
            pipline->regs->r[data->Rd] = result;
    }
    // Set CMSR when S bit is 1 and Rd is not PC
    if(data->S == 1 && data->Rd != 31)
        set_CMSR(pipline->regs, operand1, operand2,
                 result, data->opcodes, shift_carry);
    // If Rd is PC, drain pipline of 0 and 1, and return 1
    if(data->Rd == 31){
        pipline->pc_src = 1;
        pipline->drain_pipline = 1;
        return 1;
    }
    return 0;
}

// The execuation func of Ex_Multiply instruction
// return val -- if pipline need be drained, return 1, else 0
int inst_Ex_Multiply(PIPLINE* pipline, int level){
    //printf("----Execuating Ex_Multiply instruction : 0x%.8x.----\n",
    //       pipline->pipline_data[level]->inst_code);
    int32_t operand_Rm, operand_Rn, operand_Rs;
    int32_t result;
    PIPLINE_DATA* data = pipline->pipline_data[level];
    // instruction statistic
    (pipline->proc->inst_statistic).inst_total ++;
    (pipline->proc->inst_statistic).inst_multiply ++;
    // Get operands
    if(data->Rm == 31)
        operand_Rm = data->cur_inst_PC;
    else
        operand_Rm = pipline->regs->r[data->Rm];
    if(data->Rn == 31)
        operand_Rn = data->cur_inst_PC;
    else
        operand_Rn = pipline->regs->r[data->Rn];
    if(data->Rs == 31)
        operand_Rs = data->cur_inst_PC;
    else
        operand_Rs = pipline->regs->r[data->Rs];
    // Execuate multiply instruction
    result = operand_Rm * operand_Rn;
    // Execuate multiply and plus instruction
    if(data->A == 1)
        result = result + operand_Rs;
    // Write back
    pipline->regs->r[data->Rd] = result;
    // Set CMSR when S bit is 1
    if(data->S == 1){
        // Set N
        if(result < 0)
            reg_setN(pipline->regs, 1);
        else
            reg_setN(pipline->regs, 0);
        // Set Z
        if(result == 0)
            reg_setZ(pipline->regs, 1);
        else
            reg_setZ(pipline->regs, 0);
        // Set C
        reg_setC(pipline->regs, 0);
    }
    // If Rd is PC, drain pipline of 0 and 1, and return 1
    if(data->Rd == 31){
        pipline->pc_src = 1;
        pipline->drain_pipline = 1;
        return 1;
    }
    return 0;
}

// The execuation func of Ex_Branch_Ex instruction
// return val -- if pipline need be drained, return 1, else 0
int inst_Ex_Branch_Ex(PIPLINE* pipline, int level){
    //printf("----Execuating Ex_Branch_Ex instruction : 0x%.8x.----\n",
    //       pipline->pipline_data[level]->inst_code);
    PIPLINE_DATA* data = pipline->pipline_data[level];
    // instruction statistic
    (pipline->proc->inst_statistic).inst_total ++;
    if(data->cond == AL)
        (pipline->proc->inst_statistic).inst_uncond_branch ++;
    else
        (pipline->proc->inst_statistic).inst_cond_branch ++;

    if(data->L == 1)
        pipline->regs->REG_LR = data->cur_inst_PC;
    pipline->regs->REG_PC = pipline->regs->r[data->Rm];
    pipline->pc_src = 1;
    pipline->drain_pipline = 1;
    return 1;
}

// The execuation func of Ex_D_Immediate
// return val -- if pipline need be drained, return 1, else 0
int inst_Ex_D_Immediate(PIPLINE* pipline, int level){
    //printf("----Execuating Ex_D_Immediate instruction : 0x%.8x.----\n",
    //       pipline->pipline_data[level]->inst_code);
    PIPLINE_DATA* data = pipline->pipline_data[level];
    int32_t operand1, operand2;
    int32_t result;
    int carry, shift_carry;
    // instruction statistic
    (pipline->proc->inst_statistic).inst_total ++;
    (pipline->proc->inst_statistic).inst_d_immediate ++;
    // extend immediate
    //data->imm = sign_extend(data->imm, 9);
    //printf("Sign extend imm : %.8x\n", data->imm);
    // fetch operands
    if(data->Rn == 31)
        operand1 = data->cur_inst_PC;
    else
        operand1 = pipline->regs->r[data->Rn];
    operand2 = data->imm;
    // shift operand2 and return shift_carry,
    // shift_carry means whether shift operation is carried
    shift_carry = rotate_shift_right(&operand2, data->rotate);
    //printf("Rotate shift right imm : %.8x\n", operand2);
    // get register heap C bit in CMSR
    carry = reg_getC(pipline->regs);
    // Caculate result
    result = data_Ex(operand1, operand2, data->opcodes, carry);
    // Some opcodes will not write back
    if(data->opcodes != CAND && data->opcodes != CXOR
       && data->opcodes != CSUB && data->opcodes != CADD
       && data->opcodes != MOV && data->opcodes != MVN)
        pipline->regs->r[data->Rd] = result;
    else if(data->opcodes == MOV || data->opcodes == MVN){
        if((data->Rn & 0x10) != 0){
            data->Rn = data->Rn &0x0F;
            if(branch_judge(pipline, data->Rn) == 1)
                pipline->regs->r[data->Rd] = result;
        }
        else
            pipline->regs->r[data->Rd] = result;
    }
    // Set CMSR when S bit is 1 and Rd is not PC
    if(data->S == 1 && data->Rd != 31)
        set_CMSR(pipline->regs, operand1, operand2,
                 result, data->opcodes, shift_carry);
    //printf("Operand2 : %.8x\n", operand2);
    //printf("Result : %.8x\n", result);
    // If Rd is PC, drain pipline of 0 and 1, and return 1
    if(data->Rd == 31){
        pipline->pc_src = 1;
        pipline->drain_pipline = 1;
        return 1;
    }
    return 0;
}

// The execuation func of Ex_L_S_R_offset
// return val -- if pipline need be drained, return 1, else 0
int inst_Ex_L_S_R_offset(PIPLINE* pipline, int level){
    //printf("----Execuating Ex_L_S_R_offset instruction : 0x%.8x.----\n", pipline->pipline_data[level]->inst_code);
    PIPLINE_DATA* data = pipline->pipline_data[level];
    // addr is the final address
    // addr_c is the address which is calculated
    // offset is the offset of calculation
    int32_t addr, addr_c, offset, operand;
    // instruction statistic
    (pipline->proc->inst_statistic).inst_total ++;
    (pipline->proc->inst_statistic).inst_l_s_r_offset ++;
    // extend immediate
    //data->imm = sign_extend(data->imm, 5);
    offset = pipline->regs->r[data->Rm];
    // shift operation
    shift_Ex(&offset, data->imm, data->shift);
    if(data->Rn == 31)
        operand = data->cur_inst_PC;
    else
        operand = pipline->regs->r[data->Rn];
    if(data->U == 1)
        addr_c = operand + offset;
    else
        addr_c = operand - offset;
    if(data->P == 1)
        addr = addr_c;
    else{
        addr = pipline->regs->r[data->Rn];
        if(data->Rn != 31)
            pipline->regs->r[data->Rn] = addr_c;
    }
    if(data->W == 1 && data->Rn != 31)
        pipline->regs->r[data->Rn] = addr_c;
    pipline->block_reg = data->Rd;
    data->addr = addr;
    return 0;
}

// The execuation func of Ex_L_S_Hw_Sb_Rof
// return val -- if pipline need be drained, return 1, else 0
int inst_Ex_L_S_Hw_Sb_Rof(PIPLINE* pipline, int level){
    //printf("----Execuating Ex_L_S_Hw_Sb_Rof instruction : 0x%.8x.----\n", pipline->pipline_data[level]->inst_code);
    PIPLINE_DATA* data = pipline->pipline_data[level];
    // addr is the final address
    // addr_c is the address which is calculated
    // offset is the offset of calculation
    int32_t addr, addr_c, offset, operand;
    // instruction statistic
    (pipline->proc->inst_statistic).inst_total ++;
    (pipline->proc->inst_statistic).inst_l_s_hw_sb_rof ++;
    // extend immediate
    offset = pipline->regs->r[data->Rm];
    // U
    if(data->Rn == 31)
        operand = data->cur_inst_PC;
    else
        operand = pipline->regs->r[data->Rn];
    if(data->U == 1)
        addr_c = operand + offset;
    else
        addr_c = operand - offset;
    // P
    if(data->P == 1)
        addr = addr_c;
    else{
        addr = pipline->regs->r[data->Rn];
        if(data->Rn != 31)
            pipline->regs->r[data->Rn] = addr_c;
    }
    // W
    if(data->W == 1 && data->Rn != 31)
        pipline->regs->r[data->Rn] = addr_c;
    pipline->block_reg = data->Rd;
    data->addr = addr;
    return 0;
}

// The execuation func of Ex_L_S_Hw_Sb_Iof
// return val -- if pipline need be drained, return 1, else 0
int inst_Ex_L_S_Hw_Sb_Iof(PIPLINE* pipline, int level){
    //printf("----Execuating Ex_L_S_Hw_Sb_Iof instruction : 0x%.8x.----\n", pipline->pipline_data[level]->inst_code);
    PIPLINE_DATA* data = pipline->pipline_data[level];
    // addr is the final address
    // addr_c is the address which is calculated
    // offset is the offset of calculation
    int32_t addr, addr_c, offset, operand;
    // instruction statistic
    (pipline->proc->inst_statistic).inst_total ++;
    (pipline->proc->inst_statistic).inst_l_s_hw_sb_iof ++;
    // extend immediate
    offset = pipline->regs->r[data->high_offset] << 5;
    offset += pipline->regs->r[data->low_offset];
    // offset = sign_extend(offset, 10);
    // U
    if(data->Rn == 31)
        operand = data->cur_inst_PC;
    else
        operand = pipline->regs->r[data->Rn];
    if(data->U == 1)
        addr_c = operand + offset;
    else
        addr_c = operand - offset;
    // P
    if(data->P == 1)
        addr = addr_c;
    else{
        addr = pipline->regs->r[data->Rn];
        if(data->Rn != 31)
            pipline->regs->r[data->Rn] = addr_c;
    }
    // W
    if(data->W == 1 && data->Rn != 31)
        pipline->regs->r[data->Rn] = addr_c;
    pipline->block_reg = data->Rd;
    data->addr = addr;
    return 0;
}

// The execuation func of Ex_L_S_I_offset
// return val -- if pipline need be drained, return 1, else 0
int inst_Ex_L_S_I_offset(PIPLINE* pipline, int level){
    //printf("----Execuating Ex_L_S_I_offset instruction : 0x%.8x.----\n", pipline->pipline_data[level]->inst_code);
    PIPLINE_DATA* data = pipline->pipline_data[level];
    // addr is the final address
    // addr_c is the address which is calculated
    int32_t addr, addr_c, operand;
    // instruction statistic
    (pipline->proc->inst_statistic).inst_total ++;
    (pipline->proc->inst_statistic).inst_l_s_i_offset ++;
    // extend immediate
    // data->imm = sign_extend(data->imm, 14);
    if(data->Rn == 31)
        operand = data->cur_inst_PC;
    else
        operand = pipline->regs->r[data->Rn];
    if(data->U == 1)
        addr_c = operand + data->imm;
    else
        addr_c = operand - data->imm;
    if(data->P == 1)
        addr = addr_c;
    else{
        addr = pipline->regs->r[data->Rn];
        if(data->Rn != 31)
            pipline->regs->r[data->Rn] = addr_c;
    }
    if(data->W == 1 && data->Rn != 31)
        pipline->regs->r[data->Rn] = addr_c;
    pipline->block_reg = data->Rd;
    data->addr = addr;
    //printf("Memory addr : 0x%.8x\n", addr);
    return 0;
}

// The execuation func of Ex_Branch_Link
// return val -- if pipline need be drained, return 1, else 0
int inst_Ex_Branch_Link(PIPLINE* pipline, int level){
    //printf("----Execuating Ex_Branch_Link instruction : 0x%.8x.----\n", pipline->pipline_data[level]->inst_code);
    PIPLINE_DATA* data = pipline->pipline_data[level];
    // instruction statistic
    (pipline->proc->inst_statistic).inst_total ++;
    (pipline->proc->inst_statistic).inst_uncond_branch ++;
    // immediate sign extend
    data->imm = sign_extend(data->imm, 24);
    //printf("PC of this inst : %.8x\n", data->cur_inst_PC);
    //printf("Branch imm : %.8x\n", data->imm);
    if(branch_judge(pipline, data->cond) == 1){
        pipline->regs->REG_PC = data->cur_inst_PC + data->imm * 4;
        if(data->L == 1)
            pipline->regs->REG_BL = data->cur_inst_PC;
        pipline->pc_src = 1;
        pipline->drain_pipline = 1;
        return 1;
    }
    return 0;
}

// The execuation func of
// return val -- if pipline need be drained, return 1, else 0
int inst_Ex_ST(PIPLINE* pipline, int level){
    //printf("----Execuating Ex_Ex_ST instruction : 0x%.8x.----\n", pipline->pipline_data[level]->inst_code);
    // instruction statistic
    (pipline->proc->inst_statistic).inst_total ++;
    (pipline->proc->inst_statistic).inst_st ++;
    fprintf(stderr, "Software Trap, unknown manner, do nothing : 0x%.8x.\n",
            pipline->pipline_data[level]->inst_code);
    return 0;
}

// The execuation func of
// return val -- if pipline need be drained, return 1, else 0
int inst_Unknown(PIPLINE* pipline, int level){
    // instruction statistic
    (pipline->proc->inst_statistic).inst_total ++;
    (pipline->proc->inst_statistic).inst_unknown ++;
    fprintf(stderr,
            "Warning : Execuating Unknown instruction : 0x%.8x, ignored.\n",
            pipline->pipline_data[level]->inst_code);
    return 1;
}


// execuate shift operation
// operand - the operand to shift
// imm - shift offset
// shift_type - 0 : logical shift left
//              1 : logical shift right
//              2 : mathematical shift right
//              3 : rotate shift right
// return val - shift carry bit
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

// execuate data, referred by 
//          D_Imm_Shift, D_Reg_Shift, D_Immediate
// operand1 - the first operand
// operand2 - the second operand
// opcodes - operation code, see file "inst.h"
// carry - the carry bit of CMSR
// return val - result of data execuation
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
        case RSB:  return op2 - op1;
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
        //set CMSR C bit
        if(shift_carry != -1)
            reg_setC(regs, shift_carry);
		else
			reg_setC(regs, 0);
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
        //printf("aaa : %d\n", result);
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
		/**
		 * WRONG, Fixed by Chunqi Li
		 * */
        else if(opcodes == SUB || opcodes == SBC || opcodes == CSUB){
			if(opcodes != SBC){
				if(operand1 < operand2)
					reg_setC(regs, 0);
				else
					reg_setC(regs, 1);
			}else{
				if(operand1 == 0x80000000 && operand2 != 0x80000000)
					reg_setC(regs, 0);
				else if(operand1 - reg_getC(regs) < operand2)
					reg_setC(regs, 0);
				else
					reg_setC(regs, 1);
			}
        }
        else if(opcodes == RSB || opcodes == RSC){
			if(opcodes != RSC){
				if(operand2 < operand1)
					reg_setC(regs, 0);
				else 
					reg_setC(regs, 1);
			}else{
				if(operand2 == 0x80000000 && operand1 != 0x80000000)
					reg_setC(regs, 0);
				else if(operand2 - reg_getC(regs) < operand1)
					reg_setC(regs, 0);
				else
					reg_setC(regs, 1);
			}
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

// Judge the branch condition
// return bal - 1 : true, 0 : false
int branch_judge(PIPLINE* pipline, int cond){
    REGISTERS* regs = pipline->regs;
    switch(cond){
        case EQ : if(reg_getZ(regs) == 1) return 1; break;
        case NE : if(reg_getZ(regs) == 0) return 1; break;
        case UGE: if(reg_getC(regs) == 1) return 1; break;
        case ULT: if(reg_getC(regs) == 0) return 1; break;
        case N  : if(reg_getN(regs) == 1) return 1; break;
        case NN : if(reg_getN(regs) == 0) return 1; break;
        case OV : if(reg_getV(regs) == 1) return 1; break;
        case NV : if(reg_getV(regs) == 0) return 1; break;
        case UGT: 
            if(reg_getC(regs) == 1 && reg_getZ(regs) == 0)
                return 1;
            break;
        case ULE:
            if(reg_getC(regs) == 0 || reg_getZ(regs) == 1)
                return 1;
            break;
        case SGE:
            if(reg_getN(regs) == reg_getV(regs))
                return 1;
            break;
        case SLT:
            if(reg_getN(regs) != reg_getV(regs))
                return 1;
            break;
        case SGT:
            if(reg_getZ(regs) == 0 && reg_getN(regs) == reg_getV(regs))
                return 1;
            break;
        case SLE:
            if(reg_getZ(regs) == 1 || reg_getN(regs) != reg_getV(regs))
                return 1;
            break;
        case AL: return 1;
        default: return 0;
    }
    return 0;
}
