#ifndef GEN_INTERMEDIATE_CODE_H
#define GEN_INTERMEDIATE_CODE_H
#include "AST.h"
#include "symtbl.h"
#include "basic_block.h"
//define

enum operator{
	if_op = 3000, if_not_op, goto_op,negative_op,not_op,address_op,star_op,positive_op,assign_op,star_assign_op,add_op,minus_op,multiply_op, char_to_int_op,
	equal_op, less_op, larger_op, eqlarger_op, eqless_op, noteq_op, or_op, and_op, get_rb, set_rb, call, param, enterF,enterS, leaveF, leaveS , return_op, adds_op, int_to_char_op, Imm_op
};

union arg
{
	char* var_name;
	int temp_index;
	int imm_value;
};
typedef struct triple
{
	//union arg dest;
	enum operator op;
	union arg arg1;
	union arg arg2;
	int result_type;//0为char, 1为int, 2为char型指针
	int arg1_type;//0 var 1 临时变量 2 constant 3 string constant
	int arg2_type;//0 var 1 临时变量 2 constant 3 string constant
	//Brills modified here:
	//Bind triple with symtbl
	symtbl_hdr* symtbl;
	struct basic_block *block;
	int arg1_uni;
	int arg2_uni;
	int tmp_uni;
	int label;
}triple;

typedef struct stack_item//++ -- stack的item
{
	union arg item;
	int flag;// 1 ++ 0 --;
	int item_type;//0 var 1 临时变量 2 constant 3 string constant
}stack_item;



#endif
