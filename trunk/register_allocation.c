#include  <stdlib.h>
#include <memory.h>
#include "basic_block.h"
#include "register.h"

extern func_block *fblist;
var_vertex *var_vexs;
int *vex_stack;
int top;

int register_allocation_func(func_block *fb, int k);
int gen_var_graph(func_block *fb);
int del_vex(func_block *fb, int k);
int isEmpty(func_block *fb);
int union_first(func_block *fb);
int union_second(func_block *fb);
int union_node(func_block *fb, int i, int j);
int del_max_node(func_block *fb);
int color(func_block *fb, int k);

int register_allocation(int k)
{
	func_block *fb;
	for (fb = fblist; fb != NULL; fb = fb->next)
		register_allocation_func(fb, k);
	return 0;
}

int register_allocation_func(func_block *fb, int k)
{
	fb->reg = (int*)malloc(fb->uni_item_num*sizeof(int));
	var_vexs = (var_vertex*)malloc(fb->uni_item_num*sizeof(var_vertex));
	gen_var_graph(fb);
	vex_stack = (int*)malloc(fb->uni_item_num*sizeof(int));
	top = 0;
	while (1)
	{
		del_vex(fb, k);
		if (!isEmpty(fb))
		{
			if (union_first(fb))
				continue;
			else
			{
				if (union_second(fb))
					continue;
				else
					del_max_node(fb);
			}
		}
		else
			break;
	}
	color(fb, k);
	free(vex_stack);
	free(var_vexs);
	return 0;
}

int gen_var_graph(func_block *fb)
{
	int i, j, l;
	for (i = 0; i < fb->uni_item_num; i++)
	{
		var_vexs[i].n = i;
		var_vexs[i].con_vexs = NULL;
		var_vexs[i].adj_vexs = (int*)malloc((fb->width)*sizeof(int));
		for (j = 0; j < fb->width; j++)
			var_vexs[i].adj_vexs[j] = 0;
		fb->reg[i] = -1;
	}
	for (i = 0; i < fb->code_num; i++)
	{
		for (j = 0; j < fb->uni_item_num; j++)
		{
			if ((fb->live_status[i][j/32] & (1<<(31-j%32))) == (1<<(31-j%32)))
			{
				for (l = 0; l < fb->width; l++)
					var_vexs[j].adj_vexs[l] = var_vexs[j].adj_vexs[l] | fb->live_status[i][l];
			}
		}
	}
	for (i = 0; i < fb->uni_item_num; i++)
	{
		var_vexs[i].adj_vexs[i/32] = var_vexs[i].adj_vexs[i/32] & (0xffffffff - (1<<(31-i%32)));
		var_vexs[i].deg = 0;
		for (j = 0; j < fb->uni_item_num; j++)
		{
			if ((var_vexs[i].adj_vexs[j/32] & (1<<(31-j%32))) == (1<<(31-j%32)))
				var_vexs[i].deg++;
		}
	}
	return 0;
}

int del_vex(func_block *fb, int k)
{
	int i, j, change = 1;
	while (change)
	{
		change = 0;
		for (i = 0; i < fb->uni_item_num; i++)
		{
			if (var_vexs[i].deg != -1 && var_vexs[i].deg < k)
			{
				//printf("%d\n", var_vexs[i].deg);
				for (j = 0; j < fb->uni_item_num; j++)
				{
					if (((var_vexs[i].adj_vexs[j/32]) & (1<<(31-j%32))) == (1<<(31-j%32)) && var_vexs[j].deg != -1)
					{
						var_vexs[j].adj_vexs[i/32] = var_vexs[j].adj_vexs[i/32] - (1 << (31-i%32));
						var_vexs[j].deg--;
					}
				}
				var_vexs[i].deg = -1;
				vex_stack[top++] = i;
				change = 1;
			}
		}
	}
	return 0;
}

int isEmpty(func_block *fb)
{
	int i;
	for (i = 0; i < fb->uni_item_num; i++)
		if (var_vexs[i].deg != -1)
			return 0;
	return 1;
}

int union_first(func_block *fb)
{
	int i, j, l, flag, *temp = (int*)malloc(fb->width*sizeof(int));
	flag = 0;
	for (i = 0; i < fb->uni_item_num-1; i++)
	{
		for (j = i+1; j < fb->uni_item_num; j++)
		{
			if (var_vexs[i].deg != -1 && var_vexs[j].deg != -1)
			{
				int diff;
				for (l = 0; l < fb->width; l++)
					temp[l] = var_vexs[i].adj_vexs[l] & var_vexs[j].adj_vexs[l];
				for (diff = 0, l = 0; l < fb->width; l++)
				{
					if (temp[l] != var_vexs[i].adj_vexs[l])
					{
						diff = 1;
						break;
					}
				}
				if (diff)
				{
					for (diff = 0, l = 0; l < fb->width; l++)
					{
						if (temp[l] != var_vexs[j].adj_vexs[l])
						{
							diff = 1;
							break;
						}
					}
				}
				if (!diff)
				{
					union_node(fb, i, j);
					flag = 1;
					break;
				}
				
			}
		}
	}
	free(temp);
	return flag;
}

int union_second(func_block *fb)
{
	int i, j, l, flag, *temp = (int*)malloc(fb->width*sizeof(int));
	flag = 0;
	for (i = 0; i < fb->uni_item_num-1; i++)
	{
		for (j = i+1; j < fb->uni_item_num; j++)
		{
			if (var_vexs[i].deg != -1 && var_vexs[j].deg != -1 && ((var_vexs[i].adj_vexs[j/32] & (1<<(31-j%32))) != (1<<(31-j%32))))
			{
				int z;
				for (l = 0; l < fb->width; l++)
					temp[l] = var_vexs[i].adj_vexs[l] & var_vexs[j].adj_vexs[l];
				for (z = 1, l = 0; l < fb->width; l++)
				{
					if (temp[l] != 0)
					{
						z = 0;
						break;
					}
				}
				if (!z)
				{
					union_node(fb, i, j);
					flag = 1;
					break;
				}
				
			}
		}
	}
	free(temp);
	return flag;
}

int union_node(func_block *fb, int i, int j)
{
	int l, new_deg;
	var_vertex *ptr;
	new_deg = var_vexs[i].deg;
	for (l = 0; l < fb->uni_item_num; l++)
	{
		if ((var_vexs[j].adj_vexs[l/32] & (1<<(31-l%32))) == (1<<(31-l%32)))
		{
			var_vexs[l].adj_vexs[j/32] = var_vexs[l].adj_vexs[j/32] - (1<<(31-j%32));
			if ((var_vexs[i].adj_vexs[l/32] & (1<<(31-l%32))) != (1<<(31-l%32)))
			{
				var_vexs[l].adj_vexs[i/32] = var_vexs[l].adj_vexs[j/32] | (1<<(31-i%32));
				var_vexs[i].adj_vexs[l/32] = var_vexs[i].adj_vexs[l/32] | (1<<(31-l%32));
				new_deg++;
			}
			else
				var_vexs[l].deg--;
		}
	}
	var_vexs[i].deg = new_deg;
	var_vexs[j].deg = -1;
	for (ptr = &var_vexs[i]; ptr->con_vexs != NULL; ptr = ptr->con_vexs)
		;
	ptr->con_vexs = &var_vexs[j];
	return 0;
}

int del_max_node(func_block *fb)
{
	int i, max_deg, max_n;
	max_deg = -1;
	for (i = 0; i < fb->uni_item_num; i++)
	{
		if (var_vexs[i].deg > max_deg)
		{
			max_deg = var_vexs[i].deg;
			max_n = i;
		}
	}
	for (i = 0; i < fb->uni_item_num; i++)
	{
		if ((var_vexs[max_n].adj_vexs[i/32] & (1<<(31-i%32))) == (1<<(31-i%32)))
		{
			var_vexs[i].adj_vexs[max_n/32] = var_vexs[i].adj_vexs[max_n/32] - (1<<(31-max_n%32));
			var_vexs[i].deg--;
		}
	}
	var_vexs[max_n].deg = -1;
	return 0;
}

int color(func_block *fb, int k)
{
	int i, j, *flag = (int*)malloc(k*sizeof(int));
	var_vertex *ptr;
	while (top--)
	{
		i = vex_stack[top];
		memset(flag, 0, k*sizeof(int));
		for (j = 0; j < fb->uni_item_num; j++)
		{
			if ((var_vexs[i].adj_vexs[j/32] & (1<<(31-j%32))) == (1<<(31-j%32)) && fb->reg[j] != -1)
				flag[fb->reg[j]] = 1;
		}
		for (j = 0; j < k; j++)
		{
			if (!flag[j])
			{
				for (ptr = &var_vexs[i]; ptr != NULL; ptr = ptr->con_vexs)
					fb->reg[ptr->n] = j;
				break;
			}
		}
	}
	free(flag);
	return 0;
}

