#include "symtbl.h"
#include <assert.h>
#include <stdlib.h>

#include <stdio.h>
int (*create_symtbl[67])(AST_NODE*);
extern char* name_pool;
extern int name_off;
extern int name_size;
extern char name[][30];
void gen_symtbl(AST_NODE* root)
{	
	int i;
	// init create_symtbl[]
	//create_symtbl = (int (*)(AST_NODE*))malloc(67*sizeof(int(*)(AST_NODE*)));
	name_size = 64;
	name_off = 0;
	name_pool = (char*)malloc(name_size);
	assert(name_pool != NULL);
	for (i = 0; i < 67; i++)
		create_symtbl[i] = do_nothing;
	create_symtbl[0] = program_symtbl;
	create_symtbl[9] = function_symtbl;
	create_symtbl[18] = compound_symtbl;	
	tree_traversal(root,create_symtbl);
}
int do_nothing(AST_NODE* p)
{
	p -> symtbl = p -> father -> symtbl;
	return 0;
}
int fill_symtbl(AST_NODE* p, symtbl_hdr* p_tbl)
{
	AST_NODE *ptr, *cur;
	int type;
	for (ptr = p->leftChild; ptr->nodeType != FUNCTION_LIST && ptr->nodeType != EPSILON; ptr = ptr->rightSibling->leftChild)
	{
		//printf("%s\n", name[ptr->nodeType-FUNC_OFFSET]);
		cur = ptr->leftChild;
		if (cur->nodeType != TYPE_NAME)
			cur = cur->rightSibling;
		type = cur->leftChild->nodeType;
		for (cur = cur->rightSibling->leftChild ; cur->father->nodeType != VAR_ITEM; cur = cur->leftChild)
		{
			//printf("%s\n", name[cur->nodeType-FUNC_OFFSET]);
			if (cur->nodeType == VAR_ITEM)
				add_var_item(cur, p_tbl, type);
			else
				add_var_item(cur->rightSibling->rightSibling, p_tbl, type);
		}
	}
	/*if (ptr->nodeType == FUNCTION_LIST)
	{
		for (cur = ptr->leftChild; cur != NULL; cur = cur->leftChild)
		{
			if (cur->nodeType == FUNCTION_DEF)
				temp = cur->leftChild->leftChild;
			else
				temp = cur->rightSibling->rightSibling->leftChild->leftChild;
			add_func_item(temp, p->symtbl);
		}
	}*/
	return 0;
}
int program_symtbl(AST_NODE *p)
{
	p->symtbl = init_tbl();
	p->symtbl->parent_tbl = NULL;
	fill_symtbl(p->leftChild, p->symtbl);
	return 0;
}
int function_symtbl(AST_NODE* p)
{
	AST_NODE* temp;
	//printf("%s\n", name[p->nodeType-FUNC_OFFSET]);
	p->symtbl = init_tbl();
	p->symtbl->parent_tbl = p->father->symtbl;
	p->symtbl->ret_type = INT_T;
	temp = p->leftChild->leftChild;
	if (temp->nodeType == TYPE_NAME)
	{
		p->symtbl->ret_type = temp->leftChild->nodeType;
		temp = temp->rightSibling;
	}
	if (temp->nodeType == STAR)
	{
		p->symtbl->ret_star = 1;
		temp = temp->rightSibling;
	}
	add_func_item(temp, p->symtbl->parent_tbl);
	temp = temp->rightSibling->rightSibling->leftChild;
	if ( temp -> nodeType != EPSILON && temp->nodeType != VOID_T)
		add_para_list(temp, p->symtbl);
	//printf("%s\n", name[p->leftChild->rightSibling->rightSibling->leftChild->nodeType-FUNC_OFFSET]);
	fill_symtbl(p->leftChild->rightSibling->rightSibling->leftChild, p->symtbl);
	return 0;
}
int compound_symtbl(AST_NODE* p)
{
	p->symtbl = init_tbl();
	p->symtbl->parent_tbl = p->father->symtbl;
	fill_symtbl(p->leftChild->rightSibling ,p->symtbl);
	return 0;
}
