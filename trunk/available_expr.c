/**
 * Joint iterative dataflow analysis for available expressions
 * and copy propagation for MiniC
 *  
 * Author: Brills (brillsp@gmail.com)
 *
 */
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <assert.h>
#include "basic_block.h"
#include "gen_intermediate_code.h"
//Main basic_block linked list, including ALL blocks
extern basic_block *bblist;
//Main function block linked list, including ALL functions
extern func_block * fblist;
//triple list
extern triple * triple_list;
//triple list independent list
extern int *index_index;
//total number of triples
extern int triple_list_index;
/**
 *alloc_mem the GEN, KILL, IN, OUT bit vector for each function block
 */
#define TARGET_EXPR_NUM 14
enum operator target_expr[]={negative_op, address_op, positive_op, add_op, minus_op, multiply_op, char_to_int_op, adds_op, int_to_char_op, minuss_op, r_shift};
int alloc_mem();
int init_value();
int solve_each_triple(func_block *fb);
int solve_each_function(func_block*fb);
int delete_redundent_triple(func_block* fb);

int deleted(triple t)
{
	return t.is_deleted;
}
int is_target_expr(enum operator op)
{
	int i;
	for(i = 0; i < TARGET_EXPR_NUM; i++)
		if (op == target_expr[i])
			return 1;
	return 0;
}

int is_assign_expr(enum operator op)
{
	return op == assign_op;
}

int is_address_expr(enum operator op)
{
	return op == address_op;
}
/**
 *return if the given expression is killed
 *within the given basic block
 */
int live_in_block(int idx, basic_block* bb, func_block* fb)
{
	int i;
	for(i = idx + 1; i <= bb->end; i++)
	{
		if(!deleted(triple_list[i])&&(is_assign_expr(triple_list[i].op) || is_address_expr(triple_list[i].op)) && (triple_list[i].arg1_uni == triple_list[idx].arg1_uni || triple_list[i].arg1_uni == triple_list[idx].arg2_uni))
			return 0;
	}
	return 1;
}
/*
 * 外层迭代过程：
 * 先进行可用表达式分析
 * 再进行复写传播
 * 然后迭代
 * 直到没有代码被删除
 */
int available_expr()
{
	func_block* fb;
	int has_deleted_triple, iteration_count;
	//内存分配只需1次
	alloc_mem();
	has_deleted_triple = 1;
	iteration_count = 0;
	while(has_deleted_triple)
	{
		has_deleted_triple = 0;
		init_value();
		for(fb = fblist; fb != NULL; fb = fb->next)
		{
			solve_each_function(fb);
			solve_each_triple(fb);
			if(delete_redundent_triple(fb))
				has_deleted_triple = 1;
		}
		iteration_count++;
	}

	return iteration_count;
}

/**
 *allocate memory for gen, kill, in, out for each basic block and available_status for each triple
 */
int alloc_mem()
{
	func_block* fb;
	int i;
	for(fb = fblist; fb != NULL; fb = fb->next)
	{
		fb -> gen = (unsigned int**)malloc(fb->bb_num * sizeof(int*));
		fb -> kill = (unsigned int**)malloc(fb->bb_num * sizeof(int*));
		fb -> a_in = (unsigned int**)malloc(fb->bb_num * sizeof(int*));
		fb -> a_out = (unsigned int**)malloc(fb->bb_num * sizeof(int*));
		for(i = 0; i < fb -> bb_num; i++)
		{
			fb -> gen[i] = (unsigned int*)malloc((fb->code_num/32+1) * sizeof(int));
			fb -> kill[i] = (unsigned int*)malloc((fb->code_num/32 +1) * sizeof(int));
			fb -> a_in[i] = (unsigned int*)malloc((fb->code_num/32 +1) * sizeof(int));
			fb -> a_out[i] = (unsigned int*)malloc((fb->code_num/32 +1) * sizeof(int));
		}
		/**
		 * each triple has an In[] 
		 */
		fb -> available_status = (unsigned int**) malloc(fb->code_num*sizeof(int*));
		for(i = 0; i < fb->code_num; i++)
		{
			fb->available_status[i] = (unsigned int*) malloc((fb->code_num/32+1)*sizeof(int));
		}
	}
	return 0;
}
/**
 *Calculate Gen and Kill as initialization
 */
int init_value()
{
	func_block* fb;
	
	basic_block* bb;
	int i,j;
	//for each function block:
	for(fb = fblist; fb != NULL; fb = fb->next)
	{
		//clear all arrays for each basic block
		for(i = 0; i < fb->bb_num; i++)
		{
			memset(fb->gen[i], 0, (fb->code_num/32+1)*sizeof(int));
			memset(fb->kill[i], 0, (fb->code_num/32+1)*sizeof(int));
			memset(fb->a_in[i], 0, (fb->code_num/32+1)*sizeof(int));
			memset(fb->a_out[i], 0, (fb->code_num/32+1)*sizeof(int));
		}
		//clear all In[] arrays for each triple
		for(i = 0; i < fb->code_num; i++)
			memset(fb->available_status[i], 0, (fb->code_num/32+1)*sizeof(int));
		//for each basic block:
		for(bb = fblist -> start; bb != fblist -> over -> next; bb = bb->next)
		{
			//for each triple in basic block:
			/**
			 * Caution: 
			 * basic_block.begin is abs position!
			 * use begin - fb.start
			 */
			for(i = bb->begin; i <= bb->end; i++)
			{
				/**
				 *最外层迭代会删除代码，
				 *被删除的三元式不算
				 */
				if(deleted(triple_list[i]))
					continue;
				triple cur_triple = triple_list[i];
				int cur_index = i - fb->start->begin;
				//judge if the current triple can be added in Gen:
				//1. expression
				//2. not killed in block (no def followed)
				if(is_target_expr(cur_triple.op) && live_in_block(i , bb, fb))
					fb -> gen[bb->m][cur_index / 32] |= 1 << (31 - cur_index % 32);
				/*judge if the current triple can be added in Kill
				 *only assign expr and address expr will do this
				 *get veriable assigned and scan the whole list to add Kill
				 */
				if(is_assign_expr(cur_triple.op) || is_address_expr(cur_triple.op))
				{
					for(j = fb->start->begin; j <= fb->over->end; j++)
					{
						//arg1_uni is the index to joint variable table,
						//if two veriables is the same, the the arg1_uni equals
						if(is_target_expr(triple_list[j].op)
								&&(cur_triple.arg1_uni == triple_list[j].arg1_uni || cur_triple.arg1_uni == triple_list[j].arg2_uni))
						{
							cur_index = j - fb->start->begin;
							fb -> kill[bb->m][cur_index / 32] |= 1 << (31 - cur_index % 32);
						}
					}
				}
			}
			//Out的初值：
			//对于一个基本块为Gen[B1]，其它为U-KILL[Bn]
			if(bb == fblist->start)
				for(j = 0; j <fb->code_num/32+1; j++)
					fb->a_out[bb->m][j] = fb->gen[bb->m][j];
			else
				for(j = 0; j <fb->code_num/32+1; j++)
					fb -> a_out[bb->m][j] = ~(fb->kill[bb->m][j]);

		}
	}
	return 0;
}

int solve_each_function(func_block* fb)
{
	int i,j,change, bit_length;
	//newout is used to judge whether the iteration finishes
	unsigned int *newout;
	bit_length = fb->code_num/32 + 1;
	newout = (unsigned int*)malloc(bit_length * sizeof(int));
	memset(newout,0,sizeof(int) * bit_length);
	basic_block *bb;
	//a predecessor of a basic block
	PreList* predecessor;
	change = 1;
	while(change)
	{
		change = 0;
		//iteration direction: FORWARD
		for( i = 0, bb = fb->start; i < fb->bb_num; i++, bb = bb->next)
		{
			for( j = 0; j < bit_length; j++)
				fb->a_in[i][j] = 0;
			//IN[n] = Intersection(each OUT[P])
			if(bb->predecessor != NULL)
			{
				for (j = 0; j < bit_length; j++)
					fb->a_in[i][j] = fb->a_out[bb->predecessor->m][j];
				for(predecessor = bb->predecessor; predecessor != NULL; predecessor = predecessor->next)
					for(j = 0; j < bit_length; j++)
						fb->a_in[i][j] &= fb->a_out[predecessor->m][j];
			}
			//OUT[n] = GEN[n] U (In[n] - KILL[n])
			for( j = 0; j < bit_length; j++)
			{
				newout[j] = fb->a_in[i][j] - (fb->kill[i][j]&fb->a_in[i][j]);
				newout[j] |= fb->gen[i][j];
			}
			for( j = 0; j< bit_length; j++)
			{
				if(fb->a_out[i][j] != newout[j])
				{
					change = 1;
					fb->a_out[i][j] = newout[j];
				}
			}
		}
	}
	free(newout);
	return 0;
}
/**
 * After solving one function block,
 * we can get each triple's In and Out
 */
int solve_each_triple(func_block *fb)
{
	basic_block *bb;
	int i, j, base, bit_length;
	base = fb->start->begin;
	bit_length = fb->code_num / 32 + 1;
	for(bb = fb->start; bb != fb->over->next; bb = bb->next)
	{
		for(j = 0; j < bit_length; j++)
			fb->available_status[bb->begin-base][j] = fb -> a_in[bb->m][j];
		//generate each triple's In[]
		//如果当前是赋值或指针操作，那么将所有可用的，被影响到的
		//语句全部置为不可用
		//仅求In，不求Out
		for(i = bb->begin; i < bb->end; i++)
		{
			//需要跳过被删除的三元式
			for(j = 0;j < bit_length; j++)
				fb->available_status[i-base+1][j] = fb->available_status[i-base][j];
			if(deleted(triple_list[i]))
				continue;
			//受影响
			if(is_assign_expr(triple_list[i].op) || is_address_expr(triple_list[i].op))
			{
				
				for(j = 0; j < fb->code_num; j++)
				{
					//被删除的不用判断了
					if(deleted(triple_list[j]))
						continue;
						//在当前语句前可用
						if((1<<(31-j%32)) & fb->available_status[i-base][j/32])
						{
							int available_index = base + j;
							//当前语句的赋值影响到了当前语句的可用表达式
							//变为不可用
							if(triple_list[i].arg1_uni == triple_list[available_index].arg1_uni || triple_list[i].arg1_uni == triple_list[available_index].arg2_uni)
							{
								fb->available_status[i-base+1][j/32] -= 1<<(31-j%32);
							}
						}
				}
			}
			if(is_target_expr(triple_list[i].op))
				fb->available_status[i-base+1][(i-base)/32] |= 1<<(31-(i-base)%32);
		}
	}
	return 0;
}


int is_same_expr(triple t1, triple t2)
{
	if(t1.op != t2.op ||t1.op == c_str || t2.op == c_str)
		return 0;
	if(t1.arg1_uni != t2.arg1_uni || t1.arg2_uni != t2.arg2_uni)
		return 0;
	if(t1.arg1_uni ==-1 && t2.arg1_uni == -1)
	{
		if (t1.arg1.imm_value != t2.arg1.imm_value)
			return 0;
	}
	if(t1.arg2_uni ==-1 && t2.arg2_uni == -1)
	{
		if (t1.arg2.imm_value != t2.arg2.imm_value)
			return 0;
	}
	return 1;
	//注意，union arg的三个成员大小相同才能如此比较
}
//取得在最深前驱交汇的可用表达式
int get_convergence(int* same_list, basic_block* bb, func_block* fb, int triple_pos)
{
	int base,i,deepest_in_block,pre_convergence;
	PreList* bb_pre;
	base = fb->start->begin;
	//找到块内最深前驱
	deepest_in_block = -1;
	for( i = (triple_pos==-1 ? bb->end:triple_pos - 1) ; i >= bb->begin; i-- )
		if(same_list[i - base])
			deepest_in_block = i;
	//深搜本块的所有前驱，如果这些前驱都交汇到一点，返回这一点；
	//交汇到多点，或存在一点没有可用前驱则返回块内最深前驱；
	//遇到环则不搜；
	
	//上一个交汇点，-1为找不到
	bb_pre = bb->predecessor;
	if(bb_pre == NULL)
		return deepest_in_block;
	//找到第一个不为环的前驱
	while(bb_pre->ptr->begin > bb->end)
	{
		bb_pre = bb_pre->next;
		assert(bb_pre != NULL);
	}
	//找到该前驱的最深交汇点
	pre_convergence = get_convergence(same_list, bb_pre->ptr, fb, -1);
	for(bb_pre = bb_pre->next; bb_pre != NULL; bb_pre = bb_pre->next)
	{
		//判环
		if(bb_pre->ptr->begin > bb->end)
			continue;
		//有一个不等就返回块内最深
		if(pre_convergence != get_convergence(same_list, bb_pre->ptr, fb , -1))
			return deepest_in_block;
	}
	if(pre_convergence != -1)
		return pre_convergence;
	return deepest_in_block;
}
int delete_redundent_triple(func_block* fb)
{
	int i,j,base,has_available, triple_deleted_count=0;
	basic_block* bb;
	int *propagated, *same_list;
	//标记成员被替换的语句
	propagated = (int*)malloc(fb->code_num*sizeof(int));
	memset(propagated,0,sizeof(int)*(fb->code_num));
	//标记和一个表达式相同的所有可用表达式
	same_list = (int*)malloc(fb->code_num*(sizeof(int)));
	base = fb->start->begin;
	//逐条遍历每条基本块的每条语句
	for (bb = fb->start; bb != fb->over->next; bb = bb->next)
	{
		for(i = bb->begin; i <= bb->end; i++)
		{
			if(propagated[i-base] == 1 || deleted(triple_list[i]))
				continue;
			memset(same_list,0,sizeof(int)*fb->code_num);
			has_available = 0;
			//如果某语句计算的表达式在此刻活跃且没被删除，就标记下来
			for(j = 0; j < fb->code_num; j++)
				if((fb->available_status[i - base][j / 32] >> (31 - j % 32) & 1) 
						&& !deleted(triple_list[j+base]) 
						&& is_same_expr(triple_list[i], triple_list[j+base]))
				{
					same_list[j] = 1;
					has_available = 1;
				}
			//如果存在这样的表达式，就考虑它们是否具有交汇点
			if(has_available)
			{
				int converge_at;
				converge_at = get_convergence(same_list,bb,fb, i);
				//有交汇点
				if(converge_at != -1)
				{
					//替换所有用到当前三元式的结果
					for(j = i + 1; j <fb->code_num; j++)
					{
						if(triple_list[j].arg1_type == 1 
								&& triple_list[j].arg1.temp_index == i)
						{
							triple_list[j].arg1.temp_index = converge_at;
							triple_list[j].arg1_uni = triple_list[converge_at].tmp_uni;
							propagated[j-base] = 1;
						}
						if(triple_list[j].arg2_type == 1 
								&& triple_list[j].arg2.temp_index == i)
						{
							triple_list[j].arg2.temp_index = converge_at;
							triple_list[j].arg2_uni = triple_list[converge_at].tmp_uni;
							propagated[j-base] = 1;
						}
					}
					//删除本三元式
					triple_list[i].is_deleted = 1;
					triple_deleted_count ++;
				}
			}
		}
			
	}
	free(propagated);
	free(same_list);
	return triple_deleted_count;
}
