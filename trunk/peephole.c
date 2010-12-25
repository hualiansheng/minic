#include "register.h"
#include "gen_intermediate_code.h"
void peephole_on_intermediate_code();
void peephole_on_target_code();
void calc_const();
void array_operation_optimize();

extern triple* triple_list;
extern int triple_list_index;
extern assemble *assemble_list;
extern int assemble_num;

void peephole_on_intermediate_code()
{
	calc_const();
}
void peephole_on_target_code()
{
	array_operation_optimize();
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
							triple_list[j].arg2_type = triple_list[i].arg1_type;
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
	int i, j;
	for(i = 0; i < assemble_num; i++){
		if(assemble_list[i].ins == ldw || assemble_list[i].ins == stw){
			if(assemble_list[i].Rn != 27){
				for(j = 0 ; j < i ; j++){
					if(assemble_list[j].ins == add && assemble_list[j].Rd == assemble_list[i].Rn){
						assemble_list[j].is_deleted = 1;
						assemble_list[i].Rn =assemble_list[j].Rn;
						assemble_list[i].Rm_or_Imm = 0;
						assemble_list[i].Rm_Imm = assemble_list[i].Rm_Imm;
						assemble_list[i].Rs_or_Imm = 1;
						assemble_list[i].Rs_Imm = assemble_list[j].Rs_Imm;
					}
				}
			}
		}
	}
}
