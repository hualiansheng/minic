#include "pipline.h"
#include "decode.h"
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>

#include "interpret.h"
#include "instEx.h"
#include "debugger.h"

extern int v_mode;

int pipline_IF(PIPLINE* pipline, CPU_info* cpu_info);
int pipline_ID(PIPLINE* pipline, CPU_info* cpu_info);
int pipline_Ex(PIPLINE* pipline, CPU_info* cpu_info);
int pipline_Mem(PIPLINE* pipline, CPU_info* cpu_info);
int pipline_WB(PIPLINE* pipline, CPU_info* cpu_info);
void sim_print(PIPLINE* pipline, int32_t data, int type);

PIPLINE* pipline_initial(REGISTERS* regs, CACHE* i_cache,
	CACHE* d_cache){
    PIPLINE* pipline;
    pipline = malloc(sizeof(PIPLINE));
    memset(pipline, 0, sizeof(PIPLINE));
    pipline->regs = regs;
    pipline->i_cache = i_cache;
    pipline->d_cache = d_cache;
    pipline->block = 0;
    pipline->block_reg = -1;
    return pipline;
}

int pipline_destroy(PIPLINE* pipline){
    return 1;
}

int pipline_next_step(PIPLINE* pipline, CPU_info* cpu_info){
    CPU_info* pipline_cpu_info = malloc(sizeof(CPU_info));
    //printf("REG_PC : 0x%.8x\n", pipline->regs->REG_PC);
    if(pipline->ex_begin == 1 && pipline->regs->REG_PC == 0){
	printf("Program finished.\n");
	// print final information
	if(v_mode == 0){
	    debugger_print_cpu_info(cpu_info);
	    debugger_print_inst_statistic(pipline->proc);
	}
	pipline->ex_begin = 0;
	free(pipline_cpu_info);
	return 0;
    }
    pipline_WB(pipline, pipline_cpu_info);
    cpu_info->cycles_total += pipline_cpu_info->cycles_total;
    cpu_info->cycles_work += pipline_cpu_info->cycles_work;
    cpu_info->bubbles += pipline_cpu_info->bubbles;
    cpu_info->rd_mem_times += pipline_cpu_info->rd_mem_times;
    cpu_info->wr_mem_times += pipline_cpu_info->wr_mem_times;
    cpu_info->i_cache_visit += pipline_cpu_info->i_cache_visit;
    cpu_info->i_cache_miss += pipline_cpu_info->i_cache_miss;
    cpu_info->d_cache_visit += pipline_cpu_info->d_cache_visit;
    cpu_info->d_cache_miss += pipline_cpu_info->d_cache_miss;


    //printf("CPU total cycle : %d\n", pipline_cpu_info->cycles_total);
    //printf("CPU work cycle : %d\n", pipline_cpu_info->cycles_work);
    //printf("CPU bubbles : %d\n", pipline_cpu_info->bubbles);

    free(pipline_cpu_info);

    /*
       printf("==============================\n");
       printf("PC : %x\n", pipline->regs->REG_PC);
       printf("now inst : %x\n", (pipline->pipline_data[0])->inst_code);
       printf("now inst cycles : %d\n", cpu_info->cycles_total);
       printf("===============================\n\n");
       */
    return 1;
}

int pipline_IF(PIPLINE* pipline, CPU_info* cpu_info){
    PIPLINE_DATA* data = malloc(sizeof(PIPLINE_DATA));
    CACHE_RETURN cache_return;
    uint32_t addr = pipline->regs->REG_PC;
    //printf("IF addr : 0x%.8x\n", addr);
    pipline->pipline_data[0] = data;
    if(pipline->regs->REG_PC == 0)
	return 0;
    if(pipline->pc_src == 0)
	pipline->regs->REG_PC += 4;
    else
	pipline->pc_src = 0;
    memset(data, 0, sizeof(PIPLINE_DATA));
    data->inst_addr = addr;
    cache_return = cache_search(pipline->i_cache, addr);
    data->inst_code = cache_return.data;
    // save current instruction PC
    data->cur_inst_PC = pipline->regs->REG_PC;
    if(cache_return.cpu_cycles == CACHE_MISSED_CYCLE)
	cpu_info->i_cache_miss ++;
    if(cpu_info->cycles_total < cache_return.cpu_cycles)
	cpu_info->cycles_total = cache_return.cpu_cycles;
    if(cpu_info->cycles_work < 1)
	cpu_info->cycles_work = 1;
    cpu_info->i_cache_visit ++;

    char ass_code[100];
    interpret_inst(data->inst_code, data->inst_addr, ass_code, pipline->proc);
    sprintf(pipline->pipline_info[0],
	    "Pipline #0 : IF  - addr:0x%.8x  code:0x%.8x  %s.\n",
	    data->inst_addr, data->inst_code, ass_code);
    return 1;
}

int pipline_ID(PIPLINE* pipline, CPU_info* cpu_info){
    if(pipline->pipline_data[0] != NULL){
	decode_inst_decode(pipline->pipline_data[0]);
    }
    pipline->pipline_data[1] = pipline->pipline_data[0];
    pipline_IF(pipline, cpu_info);
    if(pipline->block_reg != -1){
	int inst_type = pipline->pipline_data[1]->inst_type;
	if(inst_type == D_IMM_SHIFT || inst_type == D_REG_SHIFT ||
		inst_type == MULTIPLY || inst_type == D_IMMEDIATE ||
		inst_type == L_S_R_OFFSET || inst_type == L_S_HW_SB_ROF ||
		inst_type == L_S_HW_SB_IOF || inst_type == L_S_I_OFFSET)
	    if(pipline->block_reg == pipline->pipline_data[1]->Rn)
		pipline->block = 1;
	if(inst_type == D_REG_SHIFT || inst_type == MULTIPLY)
	    if(pipline->block_reg == pipline->pipline_data[1]->Rs)
		pipline->block = 1;
	if(inst_type == D_IMM_SHIFT || inst_type == D_REG_SHIFT ||
		inst_type == MULTIPLY || inst_type == BRANCH_EX ||
		inst_type == L_S_R_OFFSET || inst_type == L_S_HW_SB_ROF)
	    if(pipline->block_reg == pipline->pipline_data[1]->Rm)
		pipline->block = 1;
	pipline->block_reg = -1;
	//printf("Set block!!!\n");
    }

    if(pipline->pipline_data[1] == NULL)
	sprintf(pipline->pipline_info[1],
		"Pipline #1 : ID  - Inst Decode Level Empty.\n");
    else{
	PIPLINE_DATA* data = pipline->pipline_data[1];
	char ass_code[100];
	//printf("0x%.8x\n", (uint32_t)pipline->proc);
	interpret_inst(data->inst_code, data->inst_addr,
		ass_code, pipline->proc);
	sprintf(pipline->pipline_info[1],
		"Pipline #1 : ID  - addr:0x%.8x  code:0x%.8x  %s.\n",
		data->inst_addr, data->inst_code, ass_code);
    }
    return 1;
}

int pipline_Ex(PIPLINE* pipline, CPU_info* cpu_info){
    if(pipline->block != 0){
	sprintf(pipline->pipline_info[2],
		"Pipline #2 : Ex  - Pipline Blocked Above.\n");
	//sprintf(pipline->pipline_info[1],
	//        "Pipline #1 : ID  - Inst Decode Level Blocked.\n");
	//sprintf(pipline->pipline_info[0],
	//	    "Pipline #0 : IF  - Inst Fetch Level Blocked.\n");
	pipline->block --;
	cpu_info->cycles_work = 0;
	if(pipline->block == 0)
	    pipline->block_reg = -1;
	pipline->pipline_data[2] = NULL;
	return 1;
    }
    if(pipline->pipline_data[1] != NULL){
	pipline->ex_begin = 1;
	// Handle print functions
	if(pipline->pipline_data[1]->inst_type == BRANCH_LINK &&
		pipline->pipline_data[1]->cond == AL){
	    //printf("branch link al.\n");
	    int32_t offset = sign_extend(pipline->pipline_data[1]->imm, 24) * 4;
	    uint32_t addr = pipline->pipline_data[1]->inst_addr + offset + 4;
	    char func_name[100];
	    if(debugger_search_symtbl(pipline->proc, addr,
			func_name, STT_FUNC) != 0){
		//printf("%s\n", func_name);
		if(strcmp(func_name, "print_int") == 0){
		    //printf("print int\n");
		    sim_print(pipline, pipline->regs->r[0], INT);
		}
		else if(strcmp(func_name, "print_char") == 0){
		    //printf("print char\n");
		    sim_print(pipline, pipline->regs->r[0], CHAR);
		}
		else if(strcmp(func_name, "print_string") == 0){
		    //printf("print string");
		    sim_print(pipline, pipline->regs->r[0], STRING);
		}
                else if(strcmp(func_name, "print_blank") == 0){
                    printf(" ");
                }
                else if(strcmp(func_name, "print_ln") == 0){
                    printf("\n");
                }
		// Normal Function
		else
		    inst_Ex(pipline, cpu_info, 1);
	    }
	    else
		inst_Ex(pipline, cpu_info, 1);
	}
	// Normal jump
	else
	    inst_Ex(pipline, cpu_info, 1);
    }
    pipline->pipline_data[2] = pipline->pipline_data[1];
    pipline_ID(pipline, cpu_info);
    // drain pipline
    if(pipline->drain_pipline == 1){
	drain_pipline(pipline, 2);
	pipline->drain_pipline = 0;
    }

    if(pipline->pipline_data[2] == NULL)
	sprintf(pipline->pipline_info[2],
		"Pipline #2 : Ex  - Inst Execuation Level Empty.\n");
    else{
	PIPLINE_DATA* data = pipline->pipline_data[2];
	char ass_code[100];
	//printf("0x%.8x\n", (uint32_t)pipline->proc);
	interpret_inst(data->inst_code, data->inst_addr,
		ass_code, pipline->proc);
	sprintf(pipline->pipline_info[2],
		"Pipline #2 : Ex  - addr:0x%.8x  code:0x%.8x  %s.\n",
		data->inst_addr, data->inst_code, ass_code);
    }
    //    printf("--Ececuating Instruction - addr : 0x%.8x - code : 0x%.8x--\n", pipline->pipline_data[2]->inst_addr, pipline->pipline_data[2]->inst_code);

    return 1;
}

int pipline_Mem(PIPLINE* pipline, CPU_info* cpu_info){
    PIPLINE_DATA* data = pipline->pipline_data[2];
    if(pipline->pipline_data[2] != NULL){
	if(data->inst_type == L_S_R_OFFSET ||
		data->inst_type == L_S_I_OFFSET ||
		data->inst_type == L_S_HW_SB_ROF ||
		data->inst_type == L_S_HW_SB_IOF){
	    //printf("----Visiting Mem, inst code : 0x%.8x\n.----", data->inst_code);
	    //printf("--Mem addr : 0x%.8x\n.--", data->addr);
	}
	if(data->inst_type == L_S_R_OFFSET
		|| data->inst_type == L_S_I_OFFSET){
	    if(data->B == 0 && data->addr % 4 != 0){
		fprintf(stderr, "Error : Data Transportation Error, Address not Aligned.\n");
		exit(1);
	    }
	    CACHE_RETURN cache_return;
	    cache_return = cache_search(pipline->d_cache, data->addr);
	    //printf("addr:%.8x  data:%.8x\n",data->addr,cache_return.data);
	    // update cpu info about cache
	    cpu_info->d_cache_visit ++;
	    if(cache_return.cpu_cycles == CACHE_MISSED_CYCLE)
		cpu_info->d_cache_miss ++;
	    if(cpu_info->cycles_total < cache_return.cpu_cycles)
		cpu_info->cycles_total = cache_return.cpu_cycles;
	    // update cpu info about mem
	    if(data->L == 1)
		cpu_info->rd_mem_times ++;
	    else
		cpu_info->wr_mem_times ++;
	    // visit memory
	    if(data->L == 1)
		pipline->regs->r[data->Rd] = cache_return.data;
	    else{
		//printf("store.\n");
		// Handle PC is Rd
		int32_t write_data;
		if(data->Rd == 31)
		    write_data = data->cur_inst_PC;
		else
		    write_data = pipline->regs->r[data->Rd];
		// write memory
		if(data->B == 1)
		    cache_write(pipline->d_cache, data->addr,
			    write_data, DATA_B);
		else
		    cache_write(pipline->d_cache, data->addr,
			    write_data, DATA_W);
	    }
	    // handle when PC is Rd, drain pipline
	    if(data->L == 1 && data->Rd == 31){
		pipline->pc_src = 1;
		pipline->drain_pipline = 1;
	    }
	}
	else if(data->inst_type == L_S_HW_SB_ROF ||
		data->inst_type == L_S_HW_SB_IOF){
	    CACHE_RETURN cache_return;
	    cache_return = cache_search(pipline->d_cache, data->addr);
	    // update cpu info about cache
	    cpu_info->d_cache_visit ++;
	    if(cache_return.cpu_cycles == CACHE_MISSED_CYCLE)
		cpu_info->d_cache_miss ++;
	    if(cpu_info->cycles_total < cache_return.cpu_cycles)
		cpu_info->cycles_total = cache_return.cpu_cycles;
	    // update cpu info about mem
	    if(data->L == 1)
		cpu_info->rd_mem_times ++;
	    else
		cpu_info->wr_mem_times ++;
	    // visit memory
	    if(data->L == 1){
		if(data->H == 1){
		    pipline->regs->r[data->Rd] = cache_return.data & 0x0000FFFF;
		    if(data->S == 1)
			pipline->regs->r[data->Rd] = sign_extend(pipline->regs->r[data->Rd], 16);
		}
		else{
		    pipline->regs->r[data->Rd] = cache_return.data & 0x000000FF;
		    if(data->S == 1)
			pipline->regs->r[data->Rd] = sign_extend(pipline->regs->r[data->Rd], 8);
		}
	    }
	    else{
		int32_t write_data;
		// Handle PC is Rd
		if(data->Rd == 31)
		    write_data = data->cur_inst_PC;
		else
		    write_data = pipline->regs->r[data->Rd];
		if(data->H == 1)
		    cache_write(pipline->d_cache, data->addr,
			    write_data, DATA_HW);
		else
		    cache_write(pipline->d_cache, data->addr,
			    write_data, DATA_B);
	    }
	    // handle when PC is Rd, drain pipline
	    if(data->L == 1 && data->Rd == 31){
		pipline->pc_src = 1;
		pipline->drain_pipline = 1;
	    }
	}
    }
    pipline->pipline_data[3] = pipline->pipline_data[2];
    pipline_Ex(pipline, cpu_info);
    if(pipline->drain_pipline == 1){
	drain_pipline(pipline, 3);
	pipline->drain_pipline = 0;
    }

    if(pipline->pipline_data[3] == NULL)
	sprintf(pipline->pipline_info[3],
		"Pipline #3 : Mem - Memory Level Empty.\n");
    else{
	char ass_code[100];
	interpret_inst(data->inst_code, data->inst_addr,
		ass_code, pipline->proc);
	sprintf(pipline->pipline_info[3],
		"Pipline #3 : Mem - addr:0x%.8x  code:0x%.8x  %s.\n",
		data->inst_addr, data->inst_code, ass_code);
    }
    return 1;
}

int pipline_WB(PIPLINE* pipline, CPU_info* cpu_info){
    PIPLINE_DATA* data = pipline->pipline_data[3];
    // Initial cpu_info
    cpu_info->cycles_total = 1;
    cpu_info->cycles_work = 1;
    cpu_info->bubbles = 0;
    cpu_info->rd_mem_times = 0;
    cpu_info->wr_mem_times = 0;
    cpu_info->i_cache_visit = 0;
    cpu_info->i_cache_miss = 0;
    cpu_info->d_cache_visit = 0;
    cpu_info->d_cache_miss = 0;

    if(pipline->pipline_data[4] != NULL)
	free(pipline->pipline_data[4]);
    pipline->pipline_data[4] = pipline->pipline_data[3];
    pipline_Mem(pipline, cpu_info);

    if(pipline->pipline_data[4] == NULL)
	sprintf(pipline->pipline_info[4],
		"Pipline #4 : WB  - Write Back Level Empty.\n");
    else{
	char ass_code[100];
	interpret_inst(data->inst_code, data->inst_addr,
		ass_code, pipline->proc);
	sprintf(pipline->pipline_info[4],
		"Pipline #4 : WR  - addr:0x%.8x  code:0x%.8x  %s.\n",
		data->inst_addr, data->inst_code, ass_code);
    }
    /*
       if(pipline->pipline_data[4] == NULL)
       printf("--Write Back Level Empty.--\n");
       else
       printf("--Writing Back--\n");
       */
    return 1;
}

// Drain pipline before level (exclude level)
void drain_pipline(PIPLINE* pipline, int level){
    int i;
    for(i=0; i<level; i++){
	if(pipline->pipline_data[i] != NULL)
	    free(pipline->pipline_data[i]);
	pipline->pipline_data[i] = NULL;
    }
    //printf("--Drain pipline.--\n");
    return;
}

int sign_extend(int imm, int size){
    return (imm << (32-size)) >> (32-size);
}

// Handle simulator print function
void sim_print(PIPLINE* pipline, int32_t data, int type){
    // print int
    if(type == INT){
	printf("%d", data);
    }
    // print char
    else if(type == CHAR){
	char c = (char)data;
	printf("%c", c);
    }
    // print string
    else{
	PROC_MEM* mem =  pipline->proc->mem;
	uint32_t start_addr = (uint32_t) data;
	uint32_t end_addr = start_addr;
	char tmp[4];
	char* output;
	while(1){
	    if(mem_invalid(mem, end_addr) == -1){
		fprintf(stderr, "Print string : address is not an invalid string address, start addr 0x%.8x, error addr 0x%.8x", start_addr, end_addr);
		exit(-1);
	    }
	    mem_fetch(mem, end_addr, tmp, 4*sizeof(char), DATA_RD);
	    if(tmp[0] == '\0' || tmp[1] == '\0' ||
		    tmp[2] == '\0' || tmp[3] == '\0'){
		end_addr += 4;
		break;
	    }
	    else
		end_addr += 4;
	}
	if(end_addr > start_addr){
	    output = malloc((end_addr - start_addr) * sizeof(char));
	    mem_fetch(mem, start_addr, output,
		    (end_addr - start_addr) * sizeof(char),
		    DATA_RD);
	    printf("%s", output);
	    free(output);
	}
    }
}
