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
void gen_basic_block();
#endif
