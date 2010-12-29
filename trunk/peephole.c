#include "register.h"
#include "gen_intermediate_code.h"
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
void peephole_on_intermediate_code();
void peephole_on_target_code();
void calc_const();
void array_operation_optimize();
void inst_block();
void delete_redundant_mov();
int defined_before_used(int r, int current_inst, int current_block,int* visited, int* next_list, int* jump_list,int* cir_enter, int in_circulating, int mov_rd);
void change_mov_target(int mov_inst, int begin, int current_block,  int *visited, int* next_list, int* jump_list, int* cir_center, int in_circulation);
//void print_target_code(FILE* target_file, int dispatch_flag);
void calc_star();

extern triple* triple_list;
extern int triple_list_index;
extern assemble *assemble_list;
extern int assemble_num;
extern int *instruction_blocks;
extern int instruction_block_num;
extern int bloack_max_num;

int able_to_change, able_to_delete;
void peephole_on_intermediate_code()
{
	calc_star();
	calc_const();
}
void peephole_on_target_code()
{
	array_operation_optimize();
	delete_redundant_mov();
}
void calc_star(){
	int i, j;
	symtbl_item* temp_symtbl;
	for(i = 0 ; i < triple_list_index ;i ++){
		if(triple_list[i].arg1_type == 0 && triple_list[i].arg2_type == 2 && triple_list[i].arg2.imm_value == 0){
			temp_symtbl = symtbl_query( triple_list[i].symtbl, triple_list[i].arg1.var_name,0);
			if(temp_symtbl != NULL && temp_symtbl->star_num == 1){
				for(j = i+1; j < triple_list_index ; j++){
					if(triple_list[j].arg1_type == 1 && triple_list[j].arg1.temp_index == i)
					{
						triple_list[j].arg1_type = 0;
						triple_list[j].arg1 = (union arg) triple_list[i].arg1.var_name;
						triple_list[i].is_deleted = 1;
						break;					
					}
				}
			}
		}
	}
}
void calc_const()
{
	int i, j;
	for(i = 0 ; i < triple_list_index ; i ++){
		if(triple_list[i].is_deleted == 1) continue;
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
						if(triple_list[j].arg2_type == 1 && triple_list[j].arg2.temp_index == i){
							triple_list[j].arg2_type = triple_list[i].arg1_type;
							triple_list[j].arg2 = triple_list[i].arg1;
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
			if(assemble_list[i].Rn != 27 && assemble_list[i].Rn != 29){
				for(j = i-1 ; j >=0 ; j--){
					if(assemble_list[j].ins == add && assemble_list[j].Rd == assemble_list[i].Rn){
						flag = 1;
						for(k = j+1 ; k < i ; k ++){
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
	int *jump_list;
	int *visited;
	int *cir_enter;
//	FILE *debug;
	int i, j, k, flag, cflag;
	int temp, current_block;
	inst_block(0);
	cflag = 0;
	next_list = malloc(sizeof(int)*instruction_block_num);
	jump_list = malloc(sizeof(int)*instruction_block_num);
	visited = malloc(sizeof(int)*instruction_block_num);
	cir_enter = malloc(sizeof(int)*instruction_block_num);
	//create next list
	for(i = 0 ; i < instruction_block_num; i ++){
		next_list[i] = i+1;
		jump_list[i] = -1;
		if(assemble_list[instruction_blocks[2*i+1]+1].ins == bne || assemble_list[instruction_blocks[2*i+1]+1].ins == beq || assemble_list[instruction_blocks[2*i+1]+1].ins == bsl || assemble_list[instruction_blocks[2*i+1]+1].ins == beg || assemble_list[instruction_blocks[2*i+1]+1].ins == bsg || assemble_list[instruction_blocks[2*i+1]+1].ins == bel){
			temp = assemble_list[instruction_blocks[2*i+1]+1].label;
			for(j = 0 ; j < assemble_num; j ++){
				if(assemble_list[j].ins == label && assemble_list[j].label == temp){
					for(k = 0 ; k < instruction_block_num ; k ++){
						if(instruction_blocks[2*k]>j){
							jump_list[i] = k;
							break;
						}
					}
					break;
				}		
			}
		}
		else if(assemble_list[instruction_blocks[2*i+1]+1].ins == b){
			next_list[i] = -1;
			temp = assemble_list[instruction_blocks[2*i+1]+1].label;
			for(j = 0 ; j < assemble_num; j ++){
				if(assemble_list[j].ins == label && assemble_list[j].label == temp){
					for(k = 0 ; k < instruction_block_num ; k ++){
						if(instruction_blocks[2*k]>j){
							jump_list[i] = k;
							break;
						}
					}
					break;
				}		
			}
		}
		else if(assemble_list[instruction_blocks[2*i+1]+1].ins == jump && assemble_list[instruction_blocks[2*i+1]+1].Rm_Imm == 30){
			next_list[i] = -1;
			jump_list[i] = -1;
		}
	}
	//create cir_enter
	memset(cir_enter, 0, sizeof(int)*instruction_block_num);
	for(i = 0 ; i < instruction_block_num ; i ++){
		if(jump_list[i] < i && jump_list[i] != -1) cir_enter[jump_list[i]] = 1;
	}

	for(i = 0 ; i < assemble_num ; i++){
		if(assemble_list[i].ins == mov){
			for(k = 0 ; k < instruction_block_num; k++){
				if(i >= instruction_blocks[2*k] && i<=instruction_blocks[2*k+1])
				{
					current_block = k;
					break;
				}
			}
		//source operand
//			flag = 1;
			cflag = 0;
			for(j = i-1 ; j >= instruction_blocks[2*current_block] ; j --){
				if(assemble_list[j].is_deleted == 1) continue;
				if(assemble_list[j].ins != stw && assemble_list[j].Rd == assemble_list[i].Rd) break;
				if(assemble_list[j].ins != stw && assemble_list[j].Rd == assemble_list[i].Rm_Imm && assemble_list[i].Rm_or_Imm == 0){
					for(k = 0 ; k < instruction_block_num ; k++){
						visited[k] = 0;
					}
			
					//	after j and before i;
					flag = 1;	
					for(k = j+1 ; k < i ; k++){
						if((assemble_list[k].ins == stw && assemble_list[k].Rd == assemble_list[i].Rd) || assemble_list[k].Rn == assemble_list[i].Rd|| (assemble_list[k].Rm_or_Imm ==0 && assemble_list[k].Rm_Imm ==  assemble_list[i].Rd) ||(assemble_list[k].Rs_or_Imm==0 && assemble_list[k].Rs_Imm == assemble_list[i].Rd))
						{
							flag = 0;
							break;
						}
					}
					if(flag == 0) break;
					able_to_change = 1;
					able_to_delete = 1;
					if(defined_before_used(assemble_list[i].Rm_Imm, i+1, current_block, visited, next_list, jump_list, cir_enter, 0, assemble_list[i].Rd))
					{
						assemble_list[j].Rd = assemble_list[i].Rd;
						assemble_list[i].is_deleted = 1;
						
						for(k = j+1; k < i ;k ++){
							if(assemble_list[k].ins == stw && assemble_list[k].Rd == assemble_list[i].Rm_Imm){
								assemble_list[k].Rd = assemble_list[i].Rd;
							}
							if(assemble_list[k].Rn == assemble_list[i].Rm_Imm){
								assemble_list[k].Rn = assemble_list[i].Rd;
							}
							if(assemble_list[k].Rm_or_Imm ==0 && assemble_list[k].Rm_Imm ==  assemble_list[i].Rm_Imm){
								assemble_list[k].Rm_Imm=assemble_list[i].Rd;
							}
							if(assemble_list[k].Rs_or_Imm==0 && assemble_list[k].Rs_Imm == assemble_list[i].Rm_Imm){
								assemble_list[k].Rs_Imm=assemble_list[i].Rd;
							}
						}
					//	debug = fopen("debug.s", "w");
					//	print_target_code(debug,0);
					//	fclose(debug);
					}
					break;
				}
			}
			/*
			if(cflag == 0){
				able_to_change = 1;
				able_to_delete = 0;
				change_mov_target(i, i+1, current_block, visited, next_list, jump_list, cir_enter, cir_enter[current_block]);
				if(able_to_delete == 1) assemble_list[i].is_deleted = 1;
			}*/
			//target operand
			/*
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
			   }*/
		}
	}
	if(cflag == 1){
		for(i = 0 ; i < assemble_num ; i++){
			if(assemble_list[i].is_deleted) continue;
			if(assemble_list[i].ins == mov){
				for(k = 0 ; k < instruction_block_num; k++){
					if(i >= instruction_blocks[2*k] && i<=instruction_blocks[2*k+1])
					{
						current_block = k;
						break;
					}
				}
			able_to_change = 1;
			able_to_delete = 0;
			change_mov_target(i, i+1, current_block, visited, next_list, jump_list, cir_enter, cir_enter[current_block]);
			if(able_to_delete == 1) assemble_list[i].is_deleted = 1;
			}
		}
	}
	free(cir_enter);
	free(next_list);
	free(jump_list);
	free(visited);
	free(instruction_blocks);

}

int defined_before_used(int r, int begin, int current_block,int* visited, int* next_list, int* jump_list, int* cir_enter, int in_circulation, int mov_rd)
{
	int end, i;
	int result = 1;
	end = instruction_blocks[current_block*2+1];
	if(visited[current_block] == 1) return 1;
	visited[current_block] = 1;
	if(!in_circulation){
		for(i = begin ; i <= end; i ++)
		{
			if(assemble_list[i].ins != stw && assemble_list[i].Rd == r)
				return 1;
			if(assemble_list[i].ins != stw && assemble_list[i].Rd == mov_rd)
				able_to_change = 0 ;
				/*if((assemble_list[i].ins == stw && assemble_list[i].Rd == r) || assemble_list[i].Rn == r || (assemble_list[i].Rm_or_Imm ==0 && assemble_list[i].Rm_Imm == r ) ||(assemble_list[i].Rs_or_Imm==0 && assemble_list[i].Rs_Imm == r)) 
				return 0;	*/
			if(assemble_list[i].ins == stw && assemble_list[i].Rd == r){
				if(able_to_change == 1)	assemble_list[i].Rd = mov_rd;
				else return 0;
			}
			if(assemble_list[i].Rn == r){
				if(able_to_change) assemble_list[i].Rn = mov_rd;
				else return 0;
			}
			if(assemble_list[i].Rm_or_Imm == 0 && assemble_list[i].Rm_Imm == r)
			{
				if(able_to_change) assemble_list[i].Rm_Imm = mov_rd;
				else return 0;
			}
			if(assemble_list[i].Rs_or_Imm == 0 && assemble_list[i].Rs_Imm == r)
			{
				if(able_to_change) assemble_list[i].Rs_Imm = mov_rd;
				else return 0;
			}
		}
	}
	else{
		for(i = begin ; i < end; i++){
			if(assemble_list[i].ins != stw && assemble_list[i].Rd == r)
				return 1;
			if((assemble_list[i].ins == stw && assemble_list[i].Rd == r) || assemble_list[i].Rn == r || (assemble_list[i].Rm_or_Imm ==0 && assemble_list[i].Rm_Imm == r ) ||(assemble_list[i].Rs_or_Imm==0 && assemble_list[i].Rs_Imm == r)) 
				return 0;
		}
	}
	if(next_list[current_block] != -1) result = result && defined_before_used(r, instruction_blocks[2*next_list[current_block]], next_list[current_block], visited,next_list, jump_list, cir_enter, cir_enter[next_list[current_block]], mov_rd);
	if(jump_list[current_block] != -1) result = result && defined_before_used(r, instruction_blocks[2*jump_list[current_block]], jump_list[current_block], visited, next_list, jump_list, cir_enter, cir_enter[next_list[current_block]], mov_rd);
	return result;
}

void change_mov_target(int mov_inst, int begin, int current_block,  int *visited, int* next_list, int* jump_list, int* cir_enter, int in_circulation)
{
	int i, end;
	end = instruction_blocks[current_block*2+1];
	if(in_circulation){
		able_to_delete = 0;
		return;
	}
	for(i = begin; i < end; i++){
		if(assemble_list[i].ins != stw && assemble_list[i].Rd == assemble_list[mov_inst].Rm_Imm && assemble_list[mov_inst].Rm_or_Imm ==0)
			able_to_change = 0;
		if(assemble_list[i].ins != stw && assemble_list[i].Rd == assemble_list[mov_inst].Rd){
			able_to_delete = 1;
			return;
		}
		if(!able_to_change) continue;	
		if(assemble_list[i].ins == stw && assemble_list[i].Rd == assemble_list[mov_inst].Rd && assemble_list[mov_inst].Rm_or_Imm == 0) 
		{
			if(able_to_change) assemble_list[i].Rd = assemble_list[mov_inst].Rm_Imm;
			else{
				able_to_delete = 0;
				return;
			}
		}
		if(assemble_list[i].Rn == assemble_list[mov_inst].Rd && assemble_list[mov_inst].Rm_or_Imm == 1){
			able_to_delete = 0;
			return;
		}
		if(assemble_list[i].Rn == assemble_list[mov_inst].Rd && assemble_list[mov_inst].Rm_or_Imm == 0){
			if((assemble_list[i].ins == ldw || assemble_list[i].ins == stw) && assemble_list[i].Rm_Imm == assemble_list[mov_inst].Rm_Imm)
			{
				able_to_delete = 0;
			}
			else{
				if(able_to_change) assemble_list[i].Rn = assemble_list[mov_inst].Rm_Imm;
				else{
					able_to_delete = 0;
					return ;
				}
			}
			if(/*assemble_list[i].Rm_or_Imm == 0 &&*/ assemble_list[i].Rm_Imm == assemble_list[mov_inst].Rd){
				if((assemble_list[i].ins == ldw || assemble_list[i].ins == stw) && assemble_list[i].Rn == assemble_list[mov_inst].Rm_Imm){
					able_to_delete = 0;
				}
				else{
					if(able_to_change){
						assemble_list[i].Rm_Imm = assemble_list[mov_inst].Rm_Imm;
						assemble_list[i].Rm_or_Imm = assemble_list[mov_inst].Rm_or_Imm;
					}
					else{
						able_to_delete = 0;
						return;
					}
				}
			}
		}
	}
	if(next_list[current_block] != -1)
		change_mov_target(mov_inst, instruction_blocks[next_list[current_block]*2], next_list[current_block], visited, next_list, jump_list, cir_enter, cir_enter[next_list[current_block]]);
	if(jump_list[current_block] != -1)
		change_mov_target(mov_inst, instruction_blocks[jump_list[current_block]*2], jump_list[current_block], visited, next_list, jump_list, cir_enter, cir_enter[jump_list[current_block]]);
	able_to_change = 1;
	return ;
}
