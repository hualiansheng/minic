#include "basic_block.h"

int live_var_anal(func_block *fb)
{
	gen_uni_table(fb);
	return 0;
}

int gen_uni_table(func_block *fb)
{
	int begin, end, i;
	begin = fb->start->begin;
	if (fb->over == NULL)
		end = triple_list_index;
	else
		end = fb->end->begin;
	for (i = begin; i < end; i++)
	{
		if 
	}
}
