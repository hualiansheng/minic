#include "register.h"
#include "gen_intermediate_code.h"
#include <stdlib.h>
#include <memory.h>
void peephole_on_intermediate_code();
void peephole_on_target_code();
void calc_const();
void array_operation_optimize();
void inst_block();
void delete_redundant_mov();
int defined_before_used(int r, int current_inst, int current_block,int* visited, int* next_list);

extern triple* triple_list;
extern int triple_list_index;
extern assemble *assemble_list;
extern int assemble_num;
extern int *instruction_blocks;
extern int instruction_block_num;
extern int bloack_max_num;

void peephole_on_intermediate_code()
{
	calc_const();
}
void peephole_on_target_code()
{
	array_operation_optimize();
	//delete_redundant_mov();
}
void calc_const()
{
	int i, j;
	for(i = 0 ; i < triple_list_index ; i ++){
		switch(triple_list[i].op){
			case negative_op:
				{
					if(triple_list[i].arg1_type == 2){
						triple_list[i].is_deleted = 1;
						for(j = i+1 ; j < triple_list_index ; j ++){
							if(triple_list[j].arg1_type == 1 && triple_list[j].arg1.temp_index == i){
								triple_list[j].arg1_type = 2;
								triple_list[j].arg1 = (union arg)(-triple_list[i].arg1.imm_value);
							}
							else if(triple_list[j].arg2_type == 1 && triple_list[j].arg2.temp_index == i){
								triple_list[j].arg2_type = 2;
								triple_list[j].arg2 = (union arg)(-triple_list[i].arg1.imm_value);
							}
						}
					} 
					break;
				}
			case positive_op:
				{
					triple_list[i].is_deleted = 1;
					for(j = i+1; j < triple_list_index ;j ++){
						if(triple_list[j].arg1_type == 1 && triple_list[j].arg1.temp_index == i){
							triple_list[j].arg1_type = triple_list[i].arg1_type;
							triple_list[j].arg1 = triple_list[i].arg1;
						}
						else if(triple_list[j].arg2_type == 1 && triple_list[j].arg2.temp_index == i){
							triple_list[j].arg2_type = triple_list[i].arg2_type;
							triple_list[j].arg2 = triple_list[i].arg2;
						}
					}
					break;
				}
			case add_op:
				{
					if(triple_list[i].arg1_type == 2 && triple_list[i].arg2_type == 2){
						triple_list[i].is_deleted = 1;
						for(j = i+1 ; j < triple_list_index ; j ++){
							if(triple_list[j].arg1_type == 1 && triple_list[j].arg1.temp_index == i){
								triple_list[j].arg1_type = 2;
								triple_list[j].arg1 = (union arg)(triple_list[i].arg1.imm_value + triple_list[i].arg2.imm_value); 							}
							else if(triple_list[j].arg2_type == 1 && triple_list[j].arg2.temp_index == i){
								triple_list[j].arg2_type = 2;
								triple_list[j].arg2 = (union arg)(triple_list[i].arg1.imm_value + triple_list[i].arg2.imm_value);
							}
						}
					}
					break;
				}
			case minus_op:
				{
					if(triple_list[i].arg1_type == 2 && triple_list[i].arg2_type == 2){
						triple_list[i].is_deleted = 1;
						for(j = i+1 ; j < triple_list_index ; j ++){
							if(triple_list[j].arg1_type == 1 && triple_list[j].arg1.temp_index == i){
								triple_list[j].arg1_type = 2;
								triple_list[j].arg1 = (union arg)(triple_list[i].arg1.imm_value - triple_list[i].arg2.imm_value); 							}
							else if(triple_list[j].arg2_type == 1 && triple_list[j].arg2.temp_index == i){
								triple_list[j].arg2_type = 2;
								triple_list[j].arg2 = (union arg)(triple_list[i].arg1.imm_value - triple_list[i].arg2.imm_value);
							}
						}
					}
					break;
				}
			case multiply_op:
				{
					if(triple_list[i].arg1_type == 2 && triple_list[i].arg2_type == 2){
						triple_list[i].is_deleted = 1;
						for(j = i+1 ; j < triple_list_index ; j ++){
							if(triple_list[j].arg1_type == 1 && triple_list[j].arg1.temp_index == i){
								triple_list[j].arg1_type = 2;
								triple_list[j].arg1 = (union arg)(triple_list[i].arg1.imm_value * triple_list[i].arg2.imm_value); 							}
							else if(triple_list[j].arg2_type == 1 && triple_list[j].arg2.temp_index == i){
								triple_list[j].arg2_type = 2;
								triple_list[j].arg2 = (union arg)(triple_list[i].arg1.imm_value * triple_list[i].arg2.imm_value);
							}
						}
					}
					else if(triple_list[i].arg2_type == 2){
						for(j = 0 ; j < 32 ;j ++){
							if(triple_list[i].arg2.imm_value == 1<<j){
								triple_list[i].op = lshift_op;
								triple_list[i].arg2_type = 2;
								triple_list[i].arg2 = (union arg)j;
							}
						}
					}
					break;
				}
			case equal_op:
				{
					if(triple_list[i].arg1_type == 2 && triple_list[i].arg2_type == 2){
						triple_list[i].is_deleted = 1;
						for(j = i+1 ; j < triple_list_index ; j ++){
							if(triple_list[j].arg1_type == 1 && triple_list[j].arg1.temp_index == i){
								triple_list[j].arg1_type = 2;
								triple_list[j].arg1 = (union arg)((triple_list[i].arg1.imm_value == triple_list[i].arg2.imm_value)?1:0); 							}
							else if(triple_list[j].arg2_type == 1 && triple_list[j].arg2.temp_index == i){
								triple_list[j].arg2_type = 2;
								triple_list[j].arg2 = (union arg)((triple_list[i].arg1.imm_value == triple_list[i].arg2.imm_value)?1:0);
							}
						}
					}
					break;
				}
			case less_op:
				{
					if(triple_list[i].arg1_type == 2 && triple_list[i].arg2_type == 2){
						triple_list[i].is_deleted = 1;
						for(j = i+1 ; j < triple_list_index ; j ++){
							if(triple_list[j].arg1_type == 1 && triple_list[j].arg1.temp_index == i){
								triple_list[j].arg1_type = 2;
								triple_list[j].arg1 = (union arg)((triple_list[i].arg1.imm_value < triple_list[i].arg2.imm_value)?1:0); 							}
							else if(triple_list[j].arg2_type == 1 && triple_list[j].arg2.temp_index == i){
								triple_list[j].arg2_type = 2;
								triple_list[j].arg2 = (union arg)((triple_list[i].arg1.imm_value < triple_list[i].arg2.imm_value)?1:0);
							}
						}
					}
					break;
				}
			case larger_op:
				{
					if(triple_list[i].arg1_type == 2 && triple_list[i].arg2_type == 2){
						triple_list[i].is_deleted = 1;
						for(j = i+1 ; j < triple_list_index ; j ++){
							if(triple_list[j].arg1_type == 1 && triple_list[j].arg1.temp_index == i){
								triple_list[j].arg1_type = 2;
								triple_list[j].arg1 = (union arg)((triple_list[i].arg1.imm_value > triple_list[i].arg2.imm_value)?1:0); 							}
							else if(triple_list[j].arg2_type == 1 && triple_list[j].arg2.temp_index == i){
								triple_list[j].arg2_type = 2;
								triple_list[j].arg2 = (union arg)((triple_list[i].arg1.imm_value > triple_list[i].arg2.imm_value)?1:0);
							}
						}
					}
					break;
				}
			case eqlarger_op:
				{
					if(triple_list[i].arg1_type == 2 && triple_list[i].arg2_type == 2){
						triple_list[i].is_deleted = 1;
						for(j = i+1 ; j < triple_list_index ; j ++){
							if(triple_list[j].arg1_type == 1 && triple_list[j].arg1.temp_index == i){
								triple_list[j].arg1_type = 2;
								triple_list[j].arg1 = (union arg)((triple_list[i].arg1.imm_value >= triple_list[i].arg2.imm_value)?1:0); 							}
							else if(triple_list[j].arg2_type == 1 && triple_list[j].arg2.temp_index == i){
								triple_list[j].arg2_type = 2;
								triple_list[j].arg2 = (union arg)((triple_list[i].arg1.imm_value >= triple_list[i].arg2.imm_value)?1:0);
							}
						}
					}
					break;
				}
			case eqless_op: 
				{
					if(triple_list[i].arg1_type == 2 && triple_list[i].arg2_type == 2){
						triple_list[i].is_deleted = 1;
						for(j = i+1 ; j < triple_list_index ; j ++){
							if(triple_list[j].arg1_type == 1 && triple_list[j].arg1.temp_index == i){
								triple_list[j].arg1_type = 2;
								triple_list[j].arg1 = (union arg)((triple_list[i].arg1.imm_value <= triple_list[i].arg2.imm_value)?1:0); 							}
							else if(triple_list[j].arg2_type == 1 && triple_list[j].arg2.temp_index == i){
								triple_list[j].arg2_type = 2;
								triple_list[j].arg2 = (union arg)((triple_list[i].arg1.imm_value <= triple_list[i].arg2.imm_value)?1:0);
							}
						}
					}
					break;
				}
			case noteq_op:
				{
					if(triple_list[i].arg1_type == 2 && triple_list[i].arg2_type == 2){
						triple_list[i].is_deleted = 1;
						for(j = i+1 ; j < triple_list_index ; j ++){
							if(triple_list[j].arg1_type == 1 && triple_list[j].arg1.temp_index == i){
								triple_list[j].arg1_type = 2;
								triple_list[j].arg1 = (union arg)((triple_list[i].arg1.imm_value != triple_list[i].arg2.imm_value)?1:0); 							}
							else if(triple_list[j].arg2_type == 1 && triple_list[j].arg2.temp_index == i){
								triple_list[j].arg2_type = 2;
								triple_list[j].arg2 = (union arg)((triple_list[i].arg1.imm_value != triple_list[i].arg2.imm_value)?1:0);
							}
						}
					}
					break;
				}
			default:break;
		}
	}
}


void array_operation_optimize()
{
	int i, j, k, flag;
	for(i = 0; i < assemble_num; i++){
		if(assemble_list[i].ins == ldw || assemble_list[i].ins == stw){
			if(assemble_list[i].Rn != 27){
				for(j = i-1 ; j >=0 ; j--){
					if(assemble_list[j].ins == add && assemble_list[j].Rd == assemble_list[i].Rn){
						flag = 1;
						for(k = j ; k < i ; k ++){
							if(assemble_list[k].ins != stw && (assemble_list[k].Rd == assemble_list[j].Rn || (assemble_list[j].Rm_or_Imm == 0 && assemble_list[k].Rd == assemble_list[j].Rm_Imm))) flag = 0;
						}
						if(flag == 1){
							assemble_list[j].is_deleted = 1;
							assemble_list[i].Rn =assemble_list[j].Rn;
							assemble_list[i].Rm_or_Imm = assemble_list[j].Rm_or_Imm;
							assemble_list[i].Rm_Imm = assemble_list[j].Rm_Imm;
							assemble_list[i].Rs_or_Imm = assemble_list[j].Rs_or_Imm;
							assemble_list[i].Rs_Imm = assemble_list[j].Rs_Imm;
						}
						break;
					}
				}
			}
		}
	}
}



void delete_redundant_mov()
{
	int *next_list;
	int *visited;
	int i, j, k, flag, able_to_change, able_to_delete;
	int temp, current_block;
	inst_block();
	next_list = malloc(sizeof(int)*instruction_block_num);
	visited = malloc(sizeof(int)*instruction_block_num);
	//create next list
	for(i = 0 ; i < instruction_block_num; i ++){
		next_list[i] = -1;
		if(assemble_list[instruction_blocks[2*i+1]+1].ins == bne || assemble_list[instruction_blocks[2*i+1]+1].ins == beq || assemble_list[instruction_blocks[2*i+1]+1].ins == bsl || assemble_list[instruction_blocks[2*i+1]+1].ins == beg || assemble_list[instruction_blocks[2*i+1]+1].ins == bsg || assemble_list[instruction_blocks[2*i+1]+1].ins == bel || assemble_list[instruction_blocks[2*i+1]+1].ins == b){
			temp = assemble_list[instruction_blocks[2*i+1]].label;
			for(j = 0 ; j < assemble_num; j ++){
				if(assemble_list[j].ins == label && assemble_list[j].label == temp){
					for(k = 0 ; k < instruction_block_num ; k ++){
						if(instruction_blocks[2*k]>j){
							next_list[i] = k;
							break;
						}
					}
					break;
				}		
			}
		}
	}

	for(i = 0 ; i < assemble_num ; i++){
		if(assemble_list[i].ins == mov){
			for(k = 0 ; k < instruction_block_num; k++){
				if(i >= instruction_blocks[2*k] && i<=instruction_blocks[2*k+1])
					break;
				current_block = i;
			}
		//source operand
			flag = 0;
			for(j = i-1 ; j >= instruction_blocks[2*current_block] ; j --){
				if(assemble_list[j].ins != stw && assemble_list[j].Rd == assemble_list[i].Rm_Imm){
					if(defined_before_used(assemble_list[i].Rm_Imm, i, current_block, visited, next_list))
					{
						assemble_list[j].Rd = assemble_list[i].Rd;
						assemble_list[i].is_deleted = 1;
						flag = 1;
					}
					break;
				}
			}
			able_to_change = 1;
			able_to_delete = 1;
		//target operand
			if(flag == 1) continue;
			for(j = i+1; j <= instruction_blocks[2*current_block+1] ; j ++)
			{
				if(assemble_list[j].ins != stw && assemble_list[j].Rd == assemble_list[i].Rm_Imm){
					able_to_change = 0;
				}
				if(assemble_list[j].ins != stw && assemble_list[j].Rd == assemble_list[i].Rd){
					if(able_to_delete){
						assemble_list[i].is_deleted = 1;
					}
					break;
				}

				if(able_to_change){
					if(assemble_list[j].ins == stw && assemble_list[j].Rd == assemble_list[i].Rd) assemble_list[j].Rd = assemble_list[i].Rm_Imm;
					else if(assemble_list[j].Rn == assemble_list[i].Rd){
						if((assemble_list[j].ins == ldw || assemble_list[j].ins == stw) && assemble_list[j].Rm_Imm == assemble_list[i].Rm_Imm);
						else assemble_list[j].Rn = assemble_list[i].Rm_Imm;
					}
					else if(assemble_list[j].Rm_or_Imm == 0 && assemble_list[j].Rm_Imm == assemble_list[i].Rd){
						if((assemble_list[j].ins == ldw || assemble_list[j].ins == stw) && assemble_list[j].Rn == assemble_list[i].Rm_Imm);
						else assemble_list[j].Rm_Imm = assemble_list[i].Rm_Imm;
					}
				}
				else{
					if(assemble_list[j].ins == stw && assemble_list[j].Rd == assemble_list[i].Rd) able_to_delete = 0;
					else if(assemble_list[j].Rn == assemble_list[i].Rd){
						able_to_delete = 0;
					}
					else if(assemble_list[j].Rm_or_Imm == 0 && assemble_list[j].Rm_Imm == assemble_list[i].Rd){
						able_to_delete = 0;
					}
				}
			}
		}
	}
}

int defined_before_used(int r, int current_inst, int current_block,int* visited, int* next_list)
{
	int begin, end, i, temp_block;
	begin = current_inst+1;
	end = instruction_blocks[current_block*2+1];
	temp_block = current_block;
	for(i = 0 ; i < instruction_block_num ; i++){
		visited[i] = 0;
	}	
	while(1){
		visited[temp_block] = 1;
		for(i = begin ; i <= end; i ++)
		{
			if(assemble_list[i].ins != stw && assemble_list[i].Rd == r)
				return 1;
			if((assemble_list[i].ins == stw && assemble_list[i].Rd == r) || assemble_list[i].Rn == r || (assemble_list[i].Rm_or_Imm ==0 && assemble_list[i].Rm_Imm == r ) ||(assemble_list[i].Rs_or_Imm==0 && assemble_list[i].Rs_Imm == r)) 
				return 0;		
		}
		if(next_list[temp_block] == -1 || visited[next_list[temp_block]] == 1)
			break;
		else{
			temp_block = next_list[temp_block];
			begin = instruction_blocks[2*temp_block];
			end = instruction_blocks[2*temp_block+1];
		}
	}
	return 1;
}
