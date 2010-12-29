#include "basic_block.h"
#include "gen_intermediate_code.h"

extern AST_NODE* tree_root;
extern triple *triple_list;
extern int *index_index;
extern int triple_list_index;
extern basic_block *bblist;
extern func_block *fblist;
extern int block_num;

int pointer_anal()
{
	func_block *fb;
	for (fb = fblist; fb != NULL; fb = fb->next)
	{
		ptr_anal_init(fb);
		ptr_iter_anal(fb);
		gen_pointed_var(fb);
	}
	return 0;
}

int init_ptrlist(func_block *fb, PtrInfo **ptr)
{
	int j;
	PtrInfo *p;
	for (j = 0; j < fb->pointer_num; j++)
	{
		if (j == 0)
		{
			*ptr = (PtrInfo*)malloc(sizeof(PtrInfo));
			(*ptr)->ptr_uni = fb->ptr_list[j];
			(*ptr)->point_to = (unsigned int*)malloc(fb->width*sizeof(unsigned int));
			memset((*ptr)->point_to, 0, fb->width*sizeof(unsigned int));
			(*ptr)->next = NULL;
			p = *ptr;
		}
		else
		{
			p->next = (PtrInfo*)malloc(sizeof(PtrInfo));
			p->next->ptr_uni = fb->ptr_list[j];
			p->next->point_to = (unsigned int*)malloc(fb->width*sizeof(unsigned int));
			memset(p->next->point_to, 0, fb->width*sizeof(unsigned int));
			p->next->next = NULL;
			p = p->next;
		}
	}
	return 0;
} 

int ptr_anal_init(func_block *fb)
{
	int i, j;
	fb->pointer_status = (PtrInfo**)malloc(fb->code_num*sizeof(PtrInfo*));
	fb->p_in = (PtrInfo**)malloc(fb->bb_num*sizeof(PtrInfo*));
	fb->p_out = (PtrInfo**)malloc(fb->bb_num*sizeof(PtrInfo*));
	fb->ptr_list = (int*)malloc(16*sizeof(int));
	fb->ptr_list_size = 16 * sizeof(int);
	fb->pointer_num = 0;
	for (i = 0; i < fb->uni_item_num; i++)
	{
		if (fb->uni_table[i]->star_num != 0 && fb->uni_table[i]->size == -1)
		{
			if ((fb->pointer_num+1)*sizeof(int) > fb->ptr_list_size)
				adjustSize((void**)&fb->ptr_list, &fb->ptr_list_size);
			fb->ptr_list[fb->pointer_num++] = i;
		}
	}
	for (i = 0; i < fb->bb_num; i++)
	{
		init_ptrlist(fb, &fb->p_in[i]);
		init_ptrlist(fb, &fb->p_out[i]);
	}
	for (i = 0; i < fb->code_num; i++)
		init_ptrlist(fb, &fb->pointer_status[i]);
	return 0;
}

int ptr_iter_anal(func_block *fb)
{
	basic_block *bb;
	PreList *pre, *temp, *next;
	int i, j, l, base, change, u1, u2;
	base = fb->start->begin;
	init_ptrlist(fb, &temp);
	change = 1;
	while (change)
	{
		change = 0;
		for (bb = fb->start, i = 0; i < fb->bb_num; bb = bb->next, i++)
		{
			ptrlist_assign(fb, fb->p_in[i], NULL, 0);
			for (pre = bb->predecessor; pre != NULL; pre = pre->next)
				ptrlist_assign(fb, fb->p_in[i], fb->p_out[pre->m], 1);
			ptrlist_assign(fb, fb->pointer_status[bb->begin-base], fb->p_in[i], 0);
			for (j = bb->begin; j <= bb->end; j++)
			{
				ptrlist_assign(fb, temp, fb->pointer_status[j-base], 0);
				if (triple_list[index_index[j]].is_deleted)
				{
					u1 = triple_list[index_index[j]].arg1_uni;
					if (triple_list[index_index[j]].op == assign_op && fb->uni_table[u1]->star_num > 0)
					{
						if (triple_list[index_index[j]].arg2_type == 0)
						{
							u2 = triple_list[index_index[j]].arg2_uni;
							assign_ptr(fb, temp, u1, u2);
						}
						else
						{
							l = triple_list[index_index[j]].arg2.temp_index;
							if (triple_list[index_index[l]].op == address_op)
								u2 = triple_list[index_index[l]].arg1_uni;
							else
								u2 = -1;
							assign_address(fb, temp, u1, u2);
						}
					}
				}
				if (j < bb->end)
					next = fb->pointer_status[j+1-base];
				else
					next = fb->p_out[i];
				if (check_change(fb, temp, next))
					change = 1;
			}
		}
	}
	return 0;
}

int ptr_op(int type, int *x, int *y)
{
	if (type == 0)
		*x = *y;
	if (type == 1)
		*x |= *y;
	return 0;
}

int ptrlist_assign(func_block *fb, PtrInfo *x, PtrInfo *y, int type)
{
	int i;
	PtrInfo *p, *q;
	for (p = x, q = y; p != NULL; p = p->next)
	{
		if (y != NULL)
		{
			for (i = 0; i < fb->width; i++)
				ptr_op(type, &x->point_to[i], &y->point_to[i]);
			q = q->next;
		}
		else
		{
			for (i = 0; i < fb->width; i++)
				x->point_to[i] = 0;
		}
	}
	return 0;
}

PtrInfo* get_ptr(PtrInfo *p, int u)
{
	while (p->ptr_uni != u)
		p = p->next;
	return p;
}

int assign_ptr(func_block *fb, PreList *p, int u1, int u2)
{
	int i;
	PtrInfo *p1, *p2;
	p1 = get_ptr(p, u1);
	p2 = get_ptr(p, u2);
	for (i = 0; i < fb->width; i++)
		p1->point_to[i] = p2->point_to[i];
	return 0;
}

int assign_address(func_block *fb, PreList *p, int u1, int u2)
{
	int i;
	PtrInfo *p1;
	p1 = get_ptr(p, u1);
	for (i = 0; i < fb->width; i++)
		p1->point_to[i] = 0;
	if (u2 != -1)
		p1->point_to[u2/32] &= 1<<(31-u2%32);	
	return 0;
}

int check_change(func_block *fb, PtrInfo *p, PtrInfo *q)
{
	int i, j, flag;
	flag = 0;
	for (i = 0; i < fb->pointer_num; i++, p = p->next, q = q->next)
	{
		for (j = 0; j < fb->width; j++)
		{
			if (p->point_to[j] != q->point_to[j])
			{
				q->point_to[j] = p->point_to[j];
				flag = 1;
			}
		}
	}
	return flag;
}

int gen_pointed_var(func_block *fb)
{
	int i, j, l, u;
	PtrInfo *p;
	fb->pointed_var = (unsigned int**)malloc(fb->code_num*sizeof(unsigned int*));
	for (i = 0; i < fb->code_num; i++)
	{
		fb->pointed_var[i] = (unsigned int*)malloc(fb->width*sizeof(unsigned int));
		memset(fb->pointed_var[i], 0, fb->width*sizeof(unsigned int));
		for (p = fb->pointer_status[i]; p != NULL; p = p->next)
		{
			for (j = 0; j < fb->width; j++)
				fb->pointed_var[i][j] |= p->point_to[j];
		}
		if (triple_list[index_index[i+fb->start->begin]].op == call)
		{
			for (j = i-1; triple_list[index_index[j].op == param; j--)
			{
				if (triple_list[index_index[j]].arg1_type == 1)
				{
					l = triple_list[index_index[j]].arg1.temp_index;
					if (triple_list[index_index[l]].op == address_op)
					{
						u = triple_list[index_index[l]].arg1_uni;
						fb->pointed_var[i][u/32] &= 1<<(31-u%32);
					}
				}
			}
		}
	}
	return 0;
}

