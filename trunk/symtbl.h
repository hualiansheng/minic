#ifndef _SYMTBL_H
#define _SYMTBL_H
#include "AST.h"

typedef struct symtbl_hdr symtbl_hdr;
typedef struct symtbl_item symtbl_item;
typedef struct symtbl_func_para symtbl_func_para;
struct symtbl_hdr
{
	symtbl_hdr* parent_tbl;
	symtbl_hdr* leftChild_tbl;
	symtbl_hdr* rightSibling_tbl;
	int ret_type;
	int ret_star;
	int para_num;
	int item_num;
	int maxSize;
	symtbl_item* item;
};
struct symtbl_item
{
	int type;
	int star_num;
	int writable;
	char* name;
	int size;
};

/*struct symtbl_func_para
{
	int type;
	int star_num;
	int writable;
	char* name;
	int size;
	symtbl_func_para* next;
};*/

//functions in gen_symtbl.c
int do_nothing(AST_NODE* p);
int fill_symtbl(AST_NODE* p, symtbl_hdr* p_tbl);
int program_symtbl(AST_NODE *p);
int function_symtbl(AST_NODE* p);
int compound_symtbl(AST_NODE* p);
//functions in symtbl_operation.c: 
symtbl_hdr* init_tbl();
int adjustSize(void** p_old, int* max);
int add_var_item(AST_NODE* p, symtbl_hdr* p_tbl, int type);
int add_func_item(AST_NODE* p, symtbl_hdr* p_tbl);
int add_para_item(AST_NODE* p, symtbl_hdr* p_tbl);
int add_para_list(AST_NODE* p, symtbl_hdr* p_tbl);
char* name_address(char* ident_name);
symtbl_item* symtbl_query(symtbl_hdr* h, const char* target, int is_local);
#endif
