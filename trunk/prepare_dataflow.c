#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "symtbl.h"
#include "basic_block.h"
#include "gen_intermediate_code.h"
#define INIT_ITEM_NUM 64

extern func_block* fblist;
extern triple* triple_list;
extern int triple_list_index;
extern int* index_index;

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

int gen_uni_table()
{
	func_block* fb;
	int begin, end, i;
	enum operator tmp_op;
	for (fb = fblist ; fb != NULL; fb = fb->next)
	{
		begin = fb->start->begin;
		end = fb->over->end;
		fb->uni_table = (symtbl_item**)malloc(INIT_ITEM_NUM*sizeof(symtbl_item));
		fb->uni_item_num = 0;
		fb->code_num = end - begin + 1;
		fb->uni_table_size = INIT_ITEM_NUM*sizeof(symtbl_item);
		fb->mapping = (map_table*)malloc(INIT_ITEM_NUM*sizeof(map_table));
		fb->map_table_size = INIT_ITEM_NUM*sizeof(map_table);
		symtbl_hdr *ptr = triple_list[index_index[begin]].symtbl;
		for (i = ptr->para_num-1; i >= 0; i--)
		{
			//if (i >= ptr->para_num-4)
			//	ptr->item[i].rable = 0;
			get_uni_item(fb, begin, ptr->item[i].name);
		}
		for (i = begin; i <= end; i++)
		{
			if (triple_list[index_index[i]].is_deleted)
				continue;
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
	}
	return 0;
}
