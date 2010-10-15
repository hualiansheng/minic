#include "symtbl.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
char* name_pool;
int name_off;
int name_size;
symtbl_hdr* init_tbl()
{
	symtbl_hdr* p = (symtbl_hdr*)malloc(sizeof(symtbl_hdr));
	assert(p != NULL);
	p->leftChild_tbl = NULL;
	p->rightSibling_tbl = NULL;
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

int adjustSize(void** p_old, int* max)
{
	void* p_new = (void*)malloc((*max)*2+1);
	assert(p_new != NULL);
	memcpy(p_new, *p_old, *max);
	//fprintf(stderr,"guabile\n");
	*max = *max * 2 + 1;
	free(*p_old);
	*p_old = p_new;
	return 0;
}

int add_var_item(AST_NODE* p, symtbl_hdr* p_tbl, int type)
{
	if ((p_tbl->item_num+1)*sizeof(symtbl_item) >= p_tbl->maxSize)
		adjustSize((void**)(&(p_tbl->item)), &(p_tbl->maxSize));
	p = p->leftChild;
	if (p->nodeType == ARRAY_VAR)
	{
		// check dupulicate defs
		if (symtbl_query(p_tbl, (p->leftChild->content).s_content, 1)!=NULL)
		{	//error report here!
			fprintf(stderr,"symtbl_gen: dupulicate definition, halt!\n");
			return -1;
		}
		(p_tbl->item[p_tbl->item_num]).type = type;
		(p_tbl->item[p_tbl->item_num]).star_num = 0;
		(p_tbl->item[p_tbl->item_num]).writable = 0;
		p = p->leftChild;
		(p_tbl->item[p_tbl->item_num]).name = name_address((p->content).s_content);
		(p_tbl->item[p_tbl->item_num]).size = (p->rightSibling->rightSibling->content).i_content;
	}
	else
	{
		if (p->nodeType == STAR)
		{
			(p_tbl->item[p_tbl->item_num]).star_num = 1;
			p = p->rightSibling;
		}
		else
			(p_tbl->item[p_tbl->item_num]).star_num = 0;
		p = p->leftChild;
		if (p->rightSibling == NULL)
		{
			if (symtbl_query(p_tbl, (p->content).s_content, 1)!=NULL)
			{	//error report here!
				fprintf(stderr,"symtbl_gen: dupulicate definition, halt!\n");
				return -1;
			}
			(p_tbl->item[p_tbl->item_num]).type = type;
			(p_tbl->item[p_tbl->item_num]).writable = 1;
			(p_tbl->item[p_tbl->item_num]).name = name_address((p->content).s_content);
			(p_tbl->item[p_tbl->item_num]).size = -1;
		}
		else
			p_tbl -> item_num--;
	}
	p_tbl->item_num++;
	return 0;
}

int add_func_item(AST_NODE* p, symtbl_hdr* p_tbl)
{
	if (symtbl_query(p_tbl, (p->content).s_content, 1)!=NULL)
	{	//error report here!
		fprintf(stderr,"symtbl_gen: dupulicate definition, halt!\n");
		return -1;
	}

	if ((p_tbl->item_num+1)*sizeof(symtbl_item) >= p_tbl->maxSize)
		adjustSize((void**)(&(p_tbl->item)), &(p_tbl->maxSize));
	(p_tbl->item[p_tbl->item_num]).type = FUNCTION_DEF;
	(p_tbl->item[p_tbl->item_num]).star_num = 0;
	(p_tbl->item[p_tbl->item_num]).writable = 0;
	(p_tbl->item[p_tbl->item_num]).name = name_address((p->content).s_content);
	(p_tbl->item[p_tbl->item_num]).size = -1;
	p_tbl->item_num++;
	return 0;
}

int add_para_item(AST_NODE* p, symtbl_hdr* p_tbl)
{

	if ((p_tbl->item_num+1)*sizeof(symtbl_item) >= p_tbl->maxSize)
		adjustSize((void**)(&(p_tbl->item)), &(p_tbl->maxSize));
	p = p->leftChild;
	(p_tbl->item[p_tbl->item_num]).type = p->leftChild->nodeType;
	p = p->rightSibling;
	if (p->nodeType == STAR)
	{
		(p_tbl->item[p_tbl->item_num]).star_num = 1;
		p = p->rightSibling;
	}
	else
		(p_tbl->item[p_tbl->item_num]).star_num = 0;
	
	//check dupulicate params:
	if (symtbl_query(p_tbl, (p->content).s_content, 1)!=NULL)
	{	//error report here!
		fprintf(stderr,"symtbl_gen: dupulicate definition, halt!\n");
		return -1;
	}

	(p_tbl->item[p_tbl->item_num]).writable = 1;
	(p_tbl->item[p_tbl->item_num]).name = name_address((p->content).s_content);
	(p_tbl->item[p_tbl->item_num]).size = -1;	
	p_tbl->item_num++;
	p_tbl->para_num++;	
	return 0;
}

int add_para_list(AST_NODE* p, symtbl_hdr* p_tbl)
{
	AST_NODE* ptr;
	for (ptr = p->leftChild; ptr->nodeType != TYPE_NAME; ptr = ptr->leftChild)
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
	int off = name_off;
	while (name_off+strlen(ident_name) >= name_size)
		adjustSize((void**)&name_pool, &name_size);
	strncpy(name_pool+name_off, ident_name, strlen(ident_name));
	name_off += strlen(ident_name)+1;
	name_pool[name_off-1] = '\0';
	return name_pool+off;
}

/**
 * query the symbol table
 * return value:
 * 		symtbl_item* is the table item matching 'target'
 * params:
 * 		is_local = 0 : query in 'h' and its predecessors
 * 		is_local != 0 : query only in 'h'
 */
symtbl_item* symtbl_query(symtbl_hdr* h, const char* target, int is_local)
{
	
	int i;
	for( i = 0 ; i < h -> item_num ; i++ )
		if ( ! strcmp( (h -> item)[i].name, target ) )
			return &((h -> item)[i]);
	//falied: not found in table 'h'
	if ( is_local )
		return NULL;
	//search in predecessors
	for( h = h -> parent_tbl ; h != NULL ; h = h -> parent_tbl )
	{
		for( i = 0 ; i < h -> item_num ; i++ )
			if ( ! strcmp( (h -> item)[i].name, target ) )
				return &((h -> item)[i]);
	}
	return NULL;
	
	//return NULL;
}

