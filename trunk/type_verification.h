#ifndef _TYPE_VERIFICATION_H
#define _TYPE_VERIFICATION_H
#include "AST.h"
#include <stdio.h>
#include "symtbl.h"
typedef struct data_type
{
	int type;
	int star_num;
	int size;
}data_type;

void check_initial();
void dfs_type_verification(AST_NODE* root);
data_type check_type_exp(AST_NODE* exp_node);
data_type check_type_assignment(AST_NODE* assignment_node);
data_type check_type_lvalue(AST_NODE* lvalue);
data_type check_type_rvalue(AST_NODE* rvalue);
data_type check_type_SIGN(AST_NODE *p);
data_type check_type_ID(AST_NODE *p);
data_type check_type_op(AST_NODE *p);
data_type check_type_constant(AST_NODE *p);
data_type check_single(int op_type,data_type op_num);
data_type check_double(int op_type,data_type op1, data_type op2);
data_type check_wrong();
#endif
