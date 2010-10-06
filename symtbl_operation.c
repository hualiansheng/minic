#include "symtbl.h"

#include <assert.h>

symtbl_hdr* init_tbl()
{
	symtbl_hdr* p = (symtbl_hdr*)malloc(sizeof(symtbl_hdr));
	assert(p != NULL);
	p->ret_type = VOID_T;
	p->ret_star = 0;
	p->para_num = 0;
	p->item_num = 0;
	p->maxSize = 8 * sizeof(symtbl_item);
	p->item = (symtbl_item*)malloc(p->maxSize);
	assert(p->item != NULL);
	return p;
}

/*int adjustSize(symtbl_hdr* p_tbl)
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
}*/

int adjustSize(void* p_old, int* max)
{
	void* p_new = (void*)malloc((*max)*2+1);
	assert(p_new != NULL);
	strncpy(p_new, p_old, *max);
	*max = *max * 2 + 1;
	free(p_old);
	p_old = p_new;
	return 0;
}

int add_var_item(AST_NODE* p, symtbl_hdr* p_tbl, int type)
{
	if ((p_tbl->item_num+1)*sizeof(syntbl_item) >= p_tbl->maxSize)
		adjustSize((void*)(p_tbl->item), &(p_tbl->maxSize));
	p = p->leftChild;
	if (p->nodeType == ARRAY_VAR)
	{
		p_tbl->(item+p_tbl->item_num)->type = type;
		p_tbl->(item+p_tbl->item_num)->star_num = 0;
		p_tbl->(item+p_tbl->item_num)->writable = 0;
		p = p->leftChild;
		p_tbl->(item+p_tbl->item_num)->name = name_address((p->content).s_content);
		p_tbl->(item+p_tbl->item_num)->size = (p->rightSibling->rightSibling->content).i_content;
	}
	else
	{
		if (p->nodeType == STAR)
		{
			p_tbl->(item+p_tbl->item_num)->star_num = 1;
			p = p->rightSibling;
		}
		else
			p_tbl->(item+p_tbl->item_num)->star_num = 0;
		p = p->leftChild;
		if (p->rightSibling == NULL)
		{
			p_tbl->(item+p_tbl->item_num)->type = type;
			p_tbl->(item+p_tbl->item_num)->writable = 1;
			p_tbl->(item+p_tbl->item_num)->name = name_address((p->content).s_content);
			p_tbl->(item+p_tbl->item_num)->size = -1;
		}
	}
	p_tbl->item_num++;
	return 0;
}

int add_func_item(AST_NODE* p, symtbl_hdr* p_tbl)
{
	if ((p_tbl->item_num+1)*sizeof(syntbl_item) >= p_tbl->maxSize)
		adjustSize((void*)(p_tbl->item), &(p_tbl->maxSize));
	p_tbl->(item+p_tbl->item_num)->type = FUNCTION_DEF;
	p_tbl->(item+p_tbl->item_num)->star_num = 0;
	p_tbl->(item+p_tbl->item_num)->writable = 0;
	p_tbl->(item+p_tbl->item_num)->name = name_address((p->content).s_content);
	p_tbl->(item+p_tbl->item_num)->size = -1;
	p_tbl->item_num++;
	return 0;
}

int add_para_item(AST_NODE* p, symtbl_hdr* p_tbl)
{
	if ((p_tbl->item_num+1)*sizeof(syntbl_item) >= p_tbl->maxSize)
		adjustSize((void*)(p_tbl->item), &(p_tbl->maxSize));
	p = p->leftChild;
	p_tbl->(item+p_tbl->item_num)->type = p->leftChild->nodeType;
	p = p->rightSibling;
	if (p->nodeType == STAR)
	{
		p_tbl->(item+p_tbl->item_num)->star_num = 1;
		p = p->rightSibling;
	}
	else
		p_tbl->(item+p_tbl->item_num)->star_num = 0;
	p_tbl->(item+p_tbl->item_num)->writable = 1;
	p_tbl->(item+p_tbl->item_num)->name = name_address((p->content).s_content);
	p_tbl->(item+p_tbl->item_num)->size = -1;	
	p_tbl->item_num++;
	p_tbl->para_num++;	
	return 0;
}

int add_para_list(AST_NODE* p, symtbl_hdr* p_tbl)
{
	AST_NODE* ptr;
	for (ptr = p->leftChild; ptr != NULL; ptr = ptr->leftChild)
	{
		if (ptr->rightSibling == NULL)
			add_para_item(ptr, p_tbl);
		else
			add_para_item(ptr->rightSibling->rightSibling, p_tbl);
	}
	return 0;
}

char* name_address(char* ident_name)
{
	if (name_off+strlen(ident_name) >= name_size)
		adjustSize((void*)name_pool, &name_size);
	strncpy(name_pool+name_off, ident_name, strlen(ident_name));
	name_off += strlen(ident_name)+1;
	name_pool[name_off-1] = '\0';
	return name_pool+name_off;
}
