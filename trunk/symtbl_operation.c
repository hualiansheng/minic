#include "symtbl.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
char* name_pool;
int name_off;
int name_size;
extern AST_NODE* tree_root;
extern int err_num;
symtbl_hdr* init_tbl()
{
	symtbl_hdr* p = (symtbl_hdr*)malloc(sizeof(symtbl_hdr));
	assert(p != NULL);
	p->leftChild_tbl = NULL;
	p->rightSibling_tbl = NULL;
	p->func_name = NULL;
	p->ret_type = VOID_T;
	p->ret_star = 0;
	p->para_num = 0;
	p->func_def = 0;
	p->item_num = 0;
	p->maxSize = 8 * sizeof(symtbl_item);
	p->item = (symtbl_item*)malloc(p->maxSize);
	assert(p->item != NULL);	
	memset(p->item,0,p->maxSize);
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
	void* p_new = (void*)malloc((*max)*2);
	assert(p_new != NULL);
	memcpy(p_new, *p_old, *max);
	//fprintf(stderr,"guabile\n");
	*max = *max * 2 ;
	free(*p_old);
	*p_old = p_new;
	return 0;
}

int add_var_item(AST_NODE* p, symtbl_hdr* p_tbl, int type)
{
	symtbl_hdr* tmp;
	if ((p_tbl->item_num+1)*sizeof(symtbl_item) >= p_tbl->maxSize)
		adjustSize((void**)(&(p_tbl->item)), &(p_tbl->maxSize));
	p = p->leftChild;
	if (p->nodeType == ARRAY_VAR)
	{
		// check dupulicate defs
		if (symtbl_query(p_tbl, (p->leftChild->content).s_content, 1)!=NULL)
		{	//error report here!
			fprintf(stderr,"symtbl_gen: dupulicate definition, halt!\n");
			err_num++;
			return -1;
		}
		(p_tbl->item[p_tbl->item_num]).isGlobal = 0;
		(p_tbl->item[p_tbl->item_num]).rable = 1;
		(p_tbl->item[p_tbl->item_num]).type = type;
		(p_tbl->item[p_tbl->item_num]).star_num = 1;
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
				err_num++;
				return -1;
			}
			(p_tbl->item[p_tbl->item_num]).isGlobal = 0;
			(p_tbl->item[p_tbl->item_num]).rable = 1;
			(p_tbl->item[p_tbl->item_num]).type = type;
			(p_tbl->item[p_tbl->item_num]).writable = 1;
			(p_tbl->item[p_tbl->item_num]).name = name_address((p->content).s_content);
			(p_tbl->item[p_tbl->item_num]).size = -1;
		}
		else
		{
			if ((tmp=func_query(tree_root->symtbl, (p->content).s_content)) == NULL)
				add_func_item(p, type, (p_tbl->item[p_tbl->item_num]).star_num);
			else
			{
				if (func_check(p, tmp, type, (p_tbl->item[p_tbl->item_num]).star_num) == 0)
				{
					fprintf(stderr,"Function defination error!\n");
					err_num++;
					return -1;
				}
			}
			p_tbl->item_num--;
		}
	}
	p_tbl->item_num++;
	return 0;
}

int add_func_item(AST_NODE* p, int type, int star)
{
	/*if (symtbl_query(p_tbl, (p->content).s_content, 1)!=NULL)
	{	//error report here!
		fprintf(stderr,"symtbl_gen: dupulicate definition, halt!\n");
		return -1;
	}*/
	symtbl_hdr* p_tbl = tree_root->symtbl;
	symtbl_hdr* tmp;
	char *func_name;
	if ((p_tbl->item_num+1)*sizeof(symtbl_item) >= p_tbl->maxSize)
		adjustSize((void**)(&(p_tbl->item)), &(p_tbl->maxSize)); 
	p->symtbl = init_tbl();
	p->symtbl->parent_tbl = p_tbl;
	if (p_tbl->leftChild_tbl == NULL)
		p_tbl->leftChild_tbl = p->symtbl;
	else
	{
		for (tmp = p_tbl->leftChild_tbl; tmp->rightSibling_tbl != NULL; tmp = tmp->rightSibling_tbl)
			;
		tmp->rightSibling_tbl = p->symtbl;
	}
	func_name = name_address((p->content).s_content);
	p->symtbl->func_name = func_name;
	p->symtbl->ret_type = type;
	p->symtbl->ret_star = star;
	(p_tbl->item[p_tbl->item_num]).isGlobal = 1;
	(p_tbl->item[p_tbl->item_num]).rable = 0;
	(p_tbl->item[p_tbl->item_num]).type = FUNCTION_DEF;
	(p_tbl->item[p_tbl->item_num]).star_num = 0;
	(p_tbl->item[p_tbl->item_num]).writable = 0;
	(p_tbl->item[p_tbl->item_num]).name = func_name;
	(p_tbl->item[p_tbl->item_num]).size = -1;
	tmp = p->symtbl;
	p = p->rightSibling->rightSibling->leftChild;
	if (p->nodeType != EPSILON && p->nodeType != VOID_T)
		add_para_list(p, tmp);
	p_tbl->item_num++;
	return 0;
}

int add_para_item(AST_NODE* p, symtbl_hdr* p_tbl)
{

	if ((p_tbl->item_num+1)*sizeof(symtbl_item) >= p_tbl->maxSize)
		adjustSize((void**)(&(p_tbl->item)), &(p_tbl->maxSize));
	p = p->leftChild;
	(p_tbl->item[p_tbl->item_num]).isGlobal = 0;
	if (p_tbl->item_num < 4)
		(p_tbl->item[p_tbl->item_num]).rable = 0;
	else
		(p_tbl->item[p_tbl->item_num]).rable = 1;
	(p_tbl->item[p_tbl->item_num]).type = p->leftChild->nodeType;
	p = p->rightSibling;
	if (p->nodeType == STAR)
	{
		(p_tbl->item[p_tbl->item_num]).star_num = 1;
		p = p->rightSibling;
	}
	else
		(p_tbl->item[p_tbl->item_num]).star_num = 0;
	
	//check dupulicate params:p = p->rightSibling->rightSibling->leftChild->leftChild;
	if (symtbl_query(p_tbl, (p->content).s_content, 1)!=NULL)
	{	//error report here!
		fprintf(stderr,"symtbl_gen: dupulicate definition, halt!\n");
		err_num++;
		return -1;
	}

	(p_tbl->item[p_tbl->item_num]).writable = 1;
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

int add_para_name(AST_NODE* p, symtbl_hdr* p_tbl, int i)
{
	p = p->leftChild->rightSibling;
	if (p->nodeType == STAR)
		p = p->rightSibling;
	(p_tbl->item[i]).name = name_address((p->content).s_content);
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
	if(target == NULL)
		i = 1;
	assert(target != NULL);
	for( i = 0 ; i < h -> item_num ; i++ )
		if ( (h->item)[i].name != NULL && ! strcmp( (h -> item)[i].name, target ) )
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

int func_check(AST_NODE* p, symtbl_hdr* q, int type, int star)
{
	AST_NODE* temp;
	int i;
	if (type != q->ret_type || star != q->ret_star)
		return 0;
	p = p->rightSibling->rightSibling->leftChild;
	if (p->nodeType == EPSILON || p->nodeType == VOID_T)
	{
		if (q->para_num == 0)
			return 1;
		else
			return 0;
	}
	else
	{
		for (i = 0, p = p->leftChild; i < q->para_num && p->nodeType != TYPE_NAME; i++, p = p->leftChild)
		{
			if (p->rightSibling == NULL)
				temp = p->leftChild;
			else
				temp = p->rightSibling->rightSibling->leftChild;
			if ((q->item)[i].type != temp->leftChild->nodeType)
				return 0;
			if (temp->rightSibling->nodeType == STAR && (q->item)[i].star_num == 0)
				return 0;
			if (temp->rightSibling->nodeType != STAR && (q->item)[i].star_num == 1)
				return 0;
		}
		if (i != q->para_num || p->nodeType != TYPE_NAME)
			return 0;
	}
	return 1;
}

symtbl_hdr* func_query(symtbl_hdr* h, const char* target)
{
	
	int i, k;
	symtbl_hdr* p;
	for( i = 0, k = 0 ; i < h -> item_num ; i++ )
	{
		if ((h->item)[i].type == FUNCTION_DEF)
		{
			if ( ! strcmp( (h -> item)[i].name, target ) )
			{
				for (p = h->leftChild_tbl; k > 0; k--, p = p->rightSibling_tbl)
					;
				return p;
			}
			k++;
		}
	}
	return NULL;
}
