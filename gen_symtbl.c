#include "symtbl.h"
#include <assert.h>
#include <stdlib.h>

#include <stdio.h>
int (*create_symtbl[67])(AST_NODE*);	//function pointer
extern char* name_pool;
extern int name_off;
extern int name_size;
extern char name[][30];
extern AST_NODE* tree_root;
int err_num;
int gen_symtbl(AST_NODE* root)
{	
	int i;
	name_size = 1024;
	name_off = 0;
	name_pool = (char*)malloc(name_size);
	assert(name_pool != NULL);
	for (i = 0; i < 67; i++)
		create_symtbl[i] = do_nothing;
	create_symtbl[0] = program_symtbl;
	create_symtbl[9] = function_symtbl;
	create_symtbl[18] = compound_symtbl;	
	err_num = 0;	
	tree_traversal(root,create_symtbl);
	for (i = 0; i < root->symtbl->item_num; i++)
	{
		(root->symtbl->item[i]).isGlobal = 1;
		//(root->symtbl->item[i]).rable = 0;
	}
	if (err_num)
		return -1;
	return 0;
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
	symtbl_hdr* tmp;
	int type, star, i;
	type = INT_T;
	star = 0;
	temp = p->leftChild->leftChild;
	if (temp->nodeType == TYPE_NAME)
	{
		type = temp->leftChild->nodeType;
		temp = temp->rightSibling;
	}
	if (temp->nodeType == STAR)
	{
		star = 1;
		temp = temp->rightSibling;
	}
	if ((tmp=func_query(tree_root->symtbl, (temp->content).s_content)) == NULL)
	{
		add_func_item(temp, type, star);
		p->symtbl = temp->symtbl;
	}
	else
	{
		if (tmp->func_def == 1 || func_check(temp, tmp, type, star) == 0)
		{
			fprintf(stderr,"Function defination error!\n");
			err_num++;
			return -1;
		}
		p->symtbl = tmp;
	}	
	p->symtbl->func_def = 1;
	temp = temp->rightSibling->rightSibling->leftChild;
	if (temp->nodeType != EPSILON && temp->nodeType != VOID_T)
	{
		for (temp = temp->leftChild, i = 0; temp->nodeType != PARM_DECL; temp = temp->leftChild, i++)
			add_para_name(temp->rightSibling->rightSibling, p->symtbl, i);
		add_para_name(temp, p->symtbl, i);
	}
	fill_symtbl(p->leftChild->rightSibling->rightSibling->leftChild, p->symtbl);
	return 0;
}
int compound_symtbl(AST_NODE* p)
{
	p->symtbl = init_tbl();
	symtbl_hdr* tmp;
	if (p->father->symtbl->leftChild_tbl == NULL)
		p->father->symtbl->leftChild_tbl = p->symtbl;
	else
	{
		for (tmp = p->father->symtbl->leftChild_tbl; tmp->rightSibling_tbl != NULL; tmp = tmp->rightSibling_tbl)
			;
		tmp->rightSibling_tbl = p->symtbl;
	}
	p->symtbl->parent_tbl = p->father->symtbl;
	fill_symtbl(p->leftChild->rightSibling ,p->symtbl);
	return 0;
}
