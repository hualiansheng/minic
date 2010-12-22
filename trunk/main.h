#ifndef _MAIN_H_
#define _MAIN_H_
#include <stdio.h>
#include "AST.h"
extern AST_NODE* tree_root;
extern int yydebug;
extern int scope_number;
extern FILE* yyin;
int yyparse();
int gen_symtbl(AST_NODE*);
void dfs_type_verification(AST_NODE* root);
void intermediate_code(AST_NODE *root);
int gen_basic_block();
int live_var_anal();
int gen_basic_block();
int print_basic_block();
int print_live_var();
int register_allocation(int);
int gen_target_code();
void print_target_code(FILE*,int);
void instruction_dispatch();
#endif
