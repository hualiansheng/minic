#include "AST.h"
#include "symtbl.h"
#include <assert.h>
int (*create_symtbl[67])(AST_NODE*);
void gen_symtbl(AST_NODE* root)
{
	tree_traverse(root,create_symtbl);
}
int do_nothing(AST_NODE* p)
{
	p -> symtbl = p -> father -> symtbl;
	return 0;
}
int fill_symtbl(AST_NODE* p)
{
	AST_NODE* ptr, cur, temp;
	int type;
	for (ptr = p; ptr->nodeType != FUNCTION_LIST && ptr->nodeType != EPSILON; ptr = ptr->leftChild->rightSibling)
	{
		type = ptr->leftChild->leftChild->rightSibling->leftChild->nodeType;
		for (cur = ptr->leftChild->leftChild->rightSibling->rightSibling->leftChild; cur != NULL; cur = cur->leftChild)
		{
			if (cur->nodeType == VAR_ITEM)
				add_var_item(cur, p->symtbl, type);
			else
				add_var_item(cur->rightSibling->rightSibling, p->symtbl, type);
		}
	}
	if (ptr->nodeType == FUNCTION_LIST)
	{
		for (cur = ptr->leftChild; cur != NULL; cur = cur->leftChild)
		{
			if (cur->nodeType == FUNCTION_DEF)
				temp = cur->leftChild->leftChild;
			else
				temp = cur->rightSibling->rightSibling->leftChild->leftChild;
			add_func_item(temp, p->symtbl);
		}
	}
	return symb_num;
}
int program_symtbl(AST_NODE *p)
{
	p -> symtbl = init_tbl();
	p -> symtbl -> parent_tbl = NULL;
	fill_symtbl(p->leftChild);
	return 0;
}
int 
