#include "symtbl.h"
symtbl_hdr* init_tbl()
{
	symtbl_hdr* p = (symtbl_hdr*)malloc(sizeof(symtbl_hdr));
	assert(p != NULL);
	p->item_num = 0;
	p->maxSize = 8;
	p->item = (symtbl_item*)malloc(p->maxSize*sizeof(symtbl_item));
	assert(p->item != NULL);
	return p;
}

int adjustSize(symtbl_hdr* p_tbl)
{
	int i;
	symtbl_item* p_new = (symtbl_item*)malloc(2*p_tbl->maxSize*sizeof(symtbl_item));
	assert(p_new != NULL);
	for (i = 0; i < p_tbl->item_num; i++)
		p_new[i] = p_tbl->item[i];
	p_tbl->maxSize *= 2;
	free(p_tbl->item);
	p_tbl->item = p_new;
	return 0;
}

int add_var_item(AST_NODE* p, symtbl_hdr* p_tbl, int type)
{
	if (p_tbl->item_num == p_tbl->maxSize)
		adjustSize(p_tbl);
	p = p->leftChild;
	if (p->nodeType == ARRAY_VAR)
	{
		p_tbl->(item+p_tbl->item_num)->type = type;
		p_tbl->(item+p_tbl->item_num)->star_num = 0;
		p_tbl->(item+p_tbl->item_num)->writable = 0;
		p = p->leftChild;
		p_tbl->(item+p_tbl->item_num)->name = ;
		p_tbl->(item+p_tbl->item_num)->size = (p->rightSibling->rightSibling->content).i_content;
	}
	else
	{
		if (p->nodeType == STAR)
		{
			p_tbl->(item+p_tbl->item_num)->star_num = 1;
\			p = p->rightSibling;
		}
		else
			p_tbl->(item+p_tbl->item_num)->star_num = 0;
		p = p->leftChild;
		if (p->rightSibling == NULL)
		{
			p_tbl->(item+p_tbl->item_num)->type = type;
			p_tbl->(item+p_tbl->item_num)->writable = 1;
			p_tbl->(item+p_tbl->item_num)->name = ;
			p_tbl->(item+p_tbl->item_num)->size = -1;
		}
	}
	p_tbl->item_num++;
	return 0;
}
int add_func_item(AST_NODE* p, symtbl_hdr* p_tbl)
{
	int i, type = INT_T;
	if (p_tbl->item_num == p_tbl->maxSize)
		adjustSize(p_tbl);
	if (p->nodeType == TYPE_NAME)
	{
		type = p->leftChild->nodeType;
		p = p->rightSibling;
	}
	p_tbl->(item+p_tbl->item_num)->ret_type = type;
	if (p->nodeType == STAR)
	{
		p_tbl->(item+p_tbl->item_num)->star_num = 1;
		p = p->rightSibling;
	}
	else
		p_tbl->(item+p_tbl->item_num)->star_num = 0;
	for ()
	p_tbl->item_num++;
	return 0;
}
