#ifndef GEN_INTERMEDIATE_CODE_H
#define GEN_INTERMEDIATE_CODE_H
#include "AST.h"
#include "symtbl.h"
//define

enum operator{
	if_op = 3000, if_not_op, goto_op,negative_op,not_op,address_op,star_op,positive_op,assign_op,star_assign_op,add_op,minus_op,multiply_op, char_to_int_op,
	equal_op, less_op, larger_op, eqlarger_op, eqless_op, noteq_op, or_op, and_op, get_rb, set_rb, call, param, enter, leave,return_op

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
	int result_type;//0为char 1为int
	int arg1_type;//0 var 1 临时变量 2 constant 3 string constant
	int arg2_type;//0 var 1 临时变量 2 constant 3 string constant
	//Brills modified here:
	//Bind triple with symtbl
	symtbl_hdr* symtbl;
	basic_block *block;
}triple;

typedef struct stack_item//++ -- stack的item
{
	union arg item;
	int flag;// 1 ++ 0 --;
	int item_type;//0 var 1 临时变量 2 constant 3 string constant
}stack_item;


void gen_code_initial();
void push_scope(symtbl_hdr *scope);
void pop_scope();
void push(stack_item a);
void add_triple(enum operator op, int arg1, int arg2, int result_type, int arg1_type, int arg2_type);
void initialize();
int compound_code(AST_NODE *p);
int if_code(AST_NODE *p);
int statement_code(AST_NODE *p);
int null_code(AST_NODE *p);
int for_code(AST_NODE *p);
int while_code(AST_NODE *p);
int expression_code(AST_NODE *p);
int rvalue_code(AST_NODE *p);
int lvalue_code(AST_NODE *p);
int assignment_expression_code(AST_NODE *p);
int return_code(AST_NODE *p);
void add_triple_single_op(int temp_rvalue, enum operator op, AST_NODE *ptr);
void add_triple_double_op(int temp_rvalue1, int temp_rvalue2, enum operator op, AST_NODE *ptr1, AST_NODE *ptr2);//1, 2分别对应两个操作数
#endif
