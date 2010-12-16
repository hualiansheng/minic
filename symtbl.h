#ifndef _SYMTBL_H
#define _SYMTBL_H
#include "AST.h"

typedef struct symtbl_hdr symtbl_hdr;
typedef struct symtbl_item symtbl_item;
typedef struct symtbl_func_para symtbl_func_para;

//header of the symbol table, including some basic information of the symbol table
struct symtbl_hdr
{
	symtbl_hdr* parent_tbl;		//parent symtbl
	symtbl_hdr* leftChild_tbl;	//leftChild symtbl
	symtbl_hdr* rightSibling_tbl;	//rightSibling symtbl

	//ret_type, ret_star, para_num, func are useful only for function's symtbl
	char* func_name;
	int ret_type;			//return value's type of a function; default is VOID_T
	int ret_star;			//indicate whether the return value is a pointer; default is 0
	int para_num;			//the number of function's parameters
	int func_def;			//record whether the function body is defined
	int item_num;			//the number of items in the symtbl
	int maxSize;			//the max memory size for symtbl items currently, noticing that it's calculated by byte!
	symtbl_item* item;		//the list of the items
};

//items in the symbol table
struct symtbl_item
{
	int isGlobal;
	//int rable;
	int type;			//the type of the item; if this item is a function, then it's type is FUNCTION_DEF
	int star_num;			//indicate whether it's a pointer
	int writable;			//common varibles are writable, but constants, arrays and functions are unwritable
	char* name;			//name of the item 
	int size;			//used for arrays only, indicating the number of its elements
	int func_off;
	int offset;
	int reg;
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
int do_nothing(AST_NODE* p);		//the actions when visiting normal nodes
int fill_symtbl(AST_NODE* p, symtbl_hdr* p_tbl);	//fill the item into the symtbl
int program_symtbl(AST_NODE *p);	//the actions when visiting PROGRAM, creating new symtbl
int function_symtbl(AST_NODE* p);	//the actions when visiting FUNCTION_DEF, creating new symtbl and add the function item into current symtbl
int compound_symtbl(AST_NODE* p);	//the actions when visiting COMPOUNDSTMT, creating new symtbl

//functions in symtbl_operation.c: 
symtbl_hdr* init_tbl();			//initialize the symtbl
int adjustSize(void** p_old, int* max);		//adjust the size of the item list when not enough
int add_var_item(AST_NODE* p, symtbl_hdr* p_tbl, int type);	//add a new variable item
int add_func_item(AST_NODE* p, int type, int star);		//add a new function item
int add_para_item(AST_NODE* p, symtbl_hdr* p_tbl);		//add a new parameter of function into the function's symtbl
int add_para_list(AST_NODE* p, symtbl_hdr* p_tbl);		//add the whole parameter list into the function's symtbl
int add_para_name(AST_NODE* p, symtbl_hdr* p_tbl, int i);
char* name_address(char* ident_name);		//get the address of given name in the namepool
int func_check(AST_NODE* p, symtbl_hdr* q, int type, int star);
symtbl_item* symtbl_query(symtbl_hdr* h, const char* target, int is_local);	//query a variable in the symtbl; return the item in the symtbl
symtbl_hdr* func_query(symtbl_hdr* h, const char* target);			//query a function in the symtbl; return the pointer to that function's symtbl
#endif
