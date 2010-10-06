#include "symtbl.h"
#define NULL 0
//#include <stdio.h>
int (*create_symtbl[67])(AST_NODE*);

void gen_symtbl(AST_NODE* root)
{
	int i;
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
		type = ptr->leftChild->rightSibling->leftChild->nodeType;
		for (cur = ptr->leftChild->rightSibling->rightSibling->leftChild; cur != NULL; cur = cur->leftChild)
		{
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
	return symb_num;
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
	int type;
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
	if ( temp != NULL)
		add_para_list(temp, p->symtbl);
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
