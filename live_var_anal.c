#include "basic_block.h"
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include "gen_intermediate_code.h"

extern AST_NODE* tree_root;
extern triple *triple_list;
extern int *index_index;
extern int triple_list_index;
extern basic_block *bblist;
extern func_block *fblist;
extern int block_num;

int live_var_anal();
/*
int gen_uni_table(func_block *fb);
int gen_tmp_item(func_block *fb, int i);
int get_uni_item(func_block *fb, int i, char *var_name);
*/
int set_prepare(func_block *fb);
int solve_in_out(func_block *fb);
int analyze_live(func_block *fb);
int change_live(func_block *fb, int code_k, int def_or_use, int uni_k);
int check_para_ptr(func_block *fb, int i, int base);

int live_var_anal()
{
	func_block *fb;
	for (fb = fblist; fb != NULL; fb = fb->next)
	{
		set_prepare(fb);
		solve_in_out(fb);
		analyze_live(fb);
	}
	return 0;
}

/*
int gen_uni_table(func_block *fb)
{
	int begin, end, i;
	enum operator tmp_op;
	begin = fb->start->begin;
	end = fb->over->end;
	fb->uni_table = (symtbl_item**)malloc(INIT_ITEM_NUM*sizeof(symtbl_item));
	fb->uni_item_num = 0;
	fb->code_num = end - begin + 1;
	fb->uni_table_size = INIT_ITEM_NUM*sizeof(symtbl_item);
	fb->mapping = (map_table*)malloc(INIT_ITEM_NUM*sizeof(map_table));
	fb->map_table_size = INIT_ITEM_NUM*sizeof(map_table);
	symtbl_hdr *ptr = triple_list[index_index[begin]].symtbl;
	for (i = 0; i < ptr->para_num; i++)
	{
		//if (i >= ptr->para_num-4)
		//	ptr->item[i].rable = 0;
		get_uni_item(fb, begin, ptr->item[i].name);
	}
	for (i = begin; i <= end; i++)
	{
		tmp_op = triple_list[index_index[i]].op;
		if (tmp_op != if_op && tmp_op != if_not_op && tmp_op != goto_op && tmp_op != set_rb && tmp_op != param && tmp_op != enterS && tmp_op != enterF && tmp_op != leaveS && tmp_op != leaveF && tmp_op != return_op)
			triple_list[index_index[i]].tmp_uni = gen_tmp_item(fb, i);
		else
			triple_list[index_index[i]].tmp_uni = -1;
		if (tmp_op == call || tmp_op == goto_op || tmp_op == c_str)
		{
			triple_list[index_index[i]].arg1_uni = -1;
			triple_list[index_index[i]].arg2_uni = -1;
			continue;
		}
		if (triple_list[index_index[i]].arg1_type == 0)
			triple_list[index_index[i]].arg1_uni = get_uni_item(fb, i, triple_list[index_index[i]].arg1.var_name);
		else if (triple_list[index_index[i]].arg1_type == 1)
			triple_list[index_index[i]].arg1_uni = triple_list[index_index[triple_list[index_index[i]].arg1.temp_index]].tmp_uni;
			else triple_list[index_index[i]].arg1_uni = -1;
		if (tmp_op == if_op || tmp_op == if_not_op)
		{
			triple_list[index_index[i]].arg2_uni = -1;
			continue;
		}
		if (triple_list[index_index[i]].arg2_type == 0)
			triple_list[index_index[i]].arg2_uni = get_uni_item(fb, i, triple_list[index_index[i]].arg2.var_name);
		else if (triple_list[index_index[i]].arg2_type == 1)
			triple_list[index_index[i]].arg2_uni = triple_list[index_index[triple_list[index_index[i]].arg2.temp_index]].tmp_uni;
			else triple_list[index_index[i]].arg2_uni = -1;
	}
	return 0;
}

int gen_tmp_item(func_block *fb, int i)
{
	symtbl_item *new_tmp_item = (symtbl_item*)malloc(sizeof(symtbl_item));
	new_tmp_item->isGlobal = 0;
	//new_tmp_item->rable = 1;
	new_tmp_item->size = -1;
	if ((fb->uni_item_num+1)*sizeof(symtbl_item) > fb->uni_table_size)
	{
		adjustSize((void**)(&(fb->uni_table)), &(fb->uni_table_size));
		adjustSize((void**)(&(fb->mapping)), &(fb->map_table_size));
	}
	fb->mapping[fb->uni_item_num].isTmp = 1;
	fb->mapping[fb->uni_item_num].tmp_k = i;
	fb->mapping[fb->uni_item_num].var_name = NULL;	
	fb->uni_table[fb->uni_item_num++] = new_tmp_item;
	return fb->uni_item_num - 1;
}

int get_uni_item(func_block *fb, int i, char *var_name)
{
	int j;
	symtbl_item *new_uni_item;
	new_uni_item = symtbl_query(triple_list[index_index[i]].symtbl, var_name, 0);
	for (j = 0; j < fb->uni_item_num; j++)
	{
		if (fb->uni_table[j] == new_uni_item)
			break;
	}
	if (j == fb->uni_item_num)
	{
		if ((fb->uni_item_num+1)*sizeof(symtbl_item) > fb->uni_table_size)
		{
			adjustSize((void**)(&(fb->uni_table)), &(fb->uni_table_size));
			adjustSize((void**)(&(fb->mapping)), &(fb->map_table_size));
		}
		fb->mapping[fb->uni_item_num].isTmp = 0;
		fb->mapping[fb->uni_item_num].tmp_k = -1;
		fb->mapping[fb->uni_item_num].var_name = var_name;	
		fb->uni_table[fb->uni_item_num++] = new_uni_item;
	}
	return j;
}
*/
int set_prepare(func_block *fb)
{
	int i,j;
	basic_block *bb;
	int *def_use;
	fb->live_status = (unsigned int**)malloc(fb->code_num*sizeof(unsigned int*));
	for (i = 0; i < fb->code_num; i++)
	{
		fb->live_status[i] = (unsigned int*)malloc(fb->width*sizeof(unsigned int));
		memset(fb->live_status[i], 0, fb->width*sizeof(unsigned int));
	}
	fb->v_out = (unsigned int**)malloc(fb->bb_num*sizeof(unsigned int*));
	for (i = 0; i < fb->bb_num; i++)
	{
		fb->v_out[i] = (unsigned int*)malloc(fb->width*sizeof(unsigned int));
		memset(fb->v_out[i], 0, fb->width*sizeof(unsigned int));
	}
	fb->v_in = (unsigned int**)malloc(fb->bb_num*sizeof(unsigned int*));
	for (i = 0; i < fb->bb_num; i++)
	{
		fb->v_in[i] = (unsigned int*)malloc(fb->width*sizeof(unsigned int));
		memset(fb->v_in[i], 0, fb->width*sizeof(unsigned int));
	}
	fb->def = (unsigned int**)malloc(fb->bb_num*sizeof(unsigned int*));
	for (i = 0; i < fb->bb_num; i++)
	{
		fb->def[i] = (unsigned int*)malloc(fb->width*sizeof(unsigned int));
		memset(fb->def[i], 0, fb->width*sizeof(unsigned int));
	}
	fb->use = (unsigned int**)malloc(fb->bb_num*sizeof(unsigned int*));
	for (i = 0; i < fb->bb_num; i++)
	{
		fb->use[i] = (unsigned int*)malloc(fb->width*sizeof(unsigned int));
		memset(fb->use[i], 0, fb->width*sizeof(unsigned int));
	}
	def_use = (int*)malloc(fb->uni_item_num*sizeof(int));
	for (i = 0, bb = fb->start; i < fb->bb_num; i++, bb = bb->next)
	{
		for (j = 0; j < fb->uni_item_num; j++)
			def_use[j] = 0;
		for (j = bb->begin; j <= bb->end; j++)
		{
			if (triple_list[index_index[j]].is_deleted)
				continue;
			if (triple_list[index_index[j]].tmp_uni != -1 && def_use[triple_list[index_index[j]].tmp_uni] == 0)
				def_use[triple_list[index_index[j]].tmp_uni] = 1;
			if (triple_list[index_index[j]].op == assign_op)
			{
				if (def_use[triple_list[index_index[j]].arg1_uni] == 0)
					def_use[triple_list[index_index[j]].arg1_uni] = 1;
			}
			else
			{
				if (triple_list[index_index[j]].arg1_uni != -1 && def_use[triple_list[index_index[j]].arg1_uni] == 0)
					def_use[triple_list[index_index[j]].arg1_uni] = -1;
			}
			if (triple_list[index_index[j]].arg2_uni != -1 && def_use[triple_list[index_index[j]].arg2_uni] == 0)
				def_use[triple_list[index_index[j]].arg2_uni] = -1;
		}
		for (j = 0; j < fb->uni_item_num; j++)
		{
			if (def_use[j] == 1)
				fb->def[i][j/32] = fb->def[i][j/32] | (1 << (31-j%32));
			if (def_use[j] == -1)
				fb->use[i][j/32] = fb->use[i][j/32] | (1 << (31-j%32));
		}
		/*for (j = 0; j < fb->uni_item_num; j++)
			printf("%d", def_use[j]);
		printf("\n");
		for (j = 0; j < fb->width; j++)
			printf("%x", fb->use[i][j]);
		printf("\n");*/
	}
	return 0;
}

int solve_in_out(func_block *fb)
{
	int i, j, change;
	unsigned int *newin = (unsigned int*)malloc(fb->width*sizeof(unsigned int));
	basic_block *bb;
	change = 1;
	while (change)
	{
		change = 0;
		for (i = fb->bb_num-1, bb = fb->over; i >= 0; i--, bb = bb->prev)
		{
			if (i + 1 < fb->bb_num && bb->follow != NULL)
			{
				for (j = 0; j < fb->width; j++)
					fb->v_out[i][j] = fb->v_in[bb->follow->m][j];
			}
			if (bb->jump != NULL)
			{
				for (j = 0; j < fb->width; j++)
					fb->v_out[i][j] = fb->v_out[i][j] | fb->v_in[bb->jump->m][j];
			}
			for (j = 0; j < fb->width; j++)
			{			
				newin[j] = (fb->v_out[i][j] - (fb->v_out[i][j]&fb->def[i][j])) | fb->use[i][j];
				//printf("%u\n", fb->use[i][j]);
				//assert((int)fb->use[i][j] >= 0);
				//assert((int)newin[j] >= 0);
			}
			for (j = 0; j < fb->width; j++)
			{
				if (fb->v_in[i][j] != newin[j])
				{
					change = 1;
					fb->v_in[i][j] = newin[j];
				}
			}
		}
	}	
	return 0;
}

int analyze_live(func_block *fb)
{
	basic_block *bb;
	int i, j, base, tmp, arg1, arg2;
	base = fb->start->begin;
	for (bb = fb->over; ; bb = bb->prev)
	{
		for (j = 0; j < fb->width; j++)
			fb->live_status[bb->end-base][j] = fb->v_out[bb->m][j];
		for (i = bb->end; i >= bb->begin; i--)
		{
			if (!triple_list[index_index[i]].is_deleted)
			{
				tmp = triple_list[index_index[i]].tmp_uni;
				arg1 = triple_list[index_index[i]].arg1_uni;
				arg2 = triple_list[index_index[i]].arg2_uni;
				if (tmp != -1)
					change_live(fb, i-base, 1, tmp);
				if (arg1 != -1 && triple_list[index_index[i]].op != address_op)
				{
					if (triple_list[index_index[i]].op == assign_op)
						change_live(fb, i-base, 1, arg1);
					else
						change_live(fb, i-base, 0, arg1);
				}
				if (arg2 != -1)
					change_live(fb, i-base, 0, arg2);
			}
			if (i > bb->begin)
			{
				for (j = 0; j < fb->width; j++)
					fb->live_status[i-1-base][j] = fb->live_status[i-base][j];
			}
		}
		if (bb == fb->start)
			break;
	}
	return 0;
}

int change_live(func_block *fb, int code_k, int def_or_use, int uni_k)
{
	if (def_or_use == 1)
		fb->live_status[code_k][uni_k/32] = fb->live_status[code_k][uni_k/32] & (0xffffffff - (1 << (31 - uni_k%32)));
	else		
		fb->live_status[code_k][uni_k/32] = fb->live_status[code_k][uni_k/32] | (1 << (31 - uni_k%32));
	return 0;
}

/*int check_para_ptr(func_block *fb, int i, int base)
{
	int j = 1, l;
	symtbl_hdr *p;
	while (1)
	{
		if (triple_list[index_index[i+j]].op == call)
			break;
		j++;
	}
	p = func_query(tree_root->symtbl, triple_list[index_index[i+j]].arg2.var_name);
	if (p->item[j-1].star_num != 0)
	{
		while (j >= 0)
		{
			for (l = 0; l < fb->uni_item_num; l++)
			{
				if (fb->mapping[l].isTmp != 1)
					change_live(fb, i+j-base, 0, l);
			}
			j--;
		}
	}
	return 0;
}*/
