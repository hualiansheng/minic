#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "basic_block.h"
#include "register.h"
#include "register_stats.h"
#include "gen_intermediate_code.h"

extern AST_NODE* tree_root;
extern triple *triple_list;
extern int *index_index;
extern int triple_list_index;
extern func_block *fblist;

assemble *assemble_list;
int assemble_size;
int assemble_num;
int temp_reg_var[32];
int if_goto_code(func_block *fb, int i);
int goto_code(func_block *fb, int i);
int negative_code(func_block *fb, int i);
int not_code(func_block *fb, int i);
int address_code(func_block *fb, int i);
int star_code(func_block *fb, int i);
int positive_code(func_block *fb, int i);
int assign_code(func_block *fb, int i);
int star_assign_code(func_block *fb, int i);
int add_code(func_block *fb, int i);
int minus_code(func_block *fb, int i);
int multiply_code(func_block *fb, int i);
int char_int_code(func_block *fb, int i);
int equal_code(func_block *fb, int i);
int less_code(func_block *fb, int i);
int larger_code(func_block *fb, int i);
int eqlarger_code(func_block *fb, int i);
int eqless_code(func_block *fb, int i);
int noteq_code(func_block *fb, int i);
int or_code(func_block *fb, int i);
int and_code(func_block *fb, int i);
int get_rb_code(func_block *fb, int i);
int set_rb_code(func_block *fb, int i);
int call_code(func_block *fb, int i);
int param_code(func_block *fb, int i);
int enterF_code(func_block *fb, int i);
int enterS_code(func_block *fb, int i);
int leaveF_code(func_block *fb, int i);
int leaveS_code(func_block *fb, int i);
int rtn_code(func_block *fb, int i);
int array_shift_code(func_block *fb, int i);
int Imm_code(func_block *fb, int i);
int r_shift_code(func_block *fb, int i);
int c_str_code(func_block *fb, int i);
int (*g[37])(func_block*, int) = {
	if_goto_code, if_goto_code, goto_code, negative_code, not_code, address_code, star_code, positive_code, assign_code, star_assign_code, add_code, minus_code, multiply_code, char_int_code, equal_code, less_code, larger_code, eqlarger_code, eqless_code, noteq_code, or_code, and_code, get_rb_code, set_rb_code, call_code, param_code, enterF_code, enterS_code, leaveF_code, leaveS_code, rtn_code, array_shift_code, char_int_code, Imm_code, array_shift_code, r_shift_code, c_str_code
};
enum instruction map_ins[6][2] = {{bsl, beg}, {bel, bsg}, {beq, bne}, {bne, beq}, {beg, bsl}, {bsg, bel}};
char **c_str_list;
int c_str_num;
int c_str_maxSize;
int initial();
int memory_allocation();
int setLabel();
int convert();
int search_const_str(char *target);
int set_cstr_used(func_block *fb, int j);
int func_assem_init(func_block *fb);
int func_assem_end(func_block *fb);
int load_live(func_block *fb, int i);
int add_assemble(int label, enum instruction ins, int shift_direct, int Rn, int Rd, int Rs_or_Imm, int Rs_Imm, int Rm_or_Imm, int Rm_Imm);
int add_special(char *content, enum instruction ins);
int check_live(func_block *fb, int i, int isTmp);
int load_operator(func_block *fb, int u, int r, int _r);
int store_result(func_block *fb, int i, enum instruction ins, int u, int Rs, int Rd, int shift_direct, int Rs_or_Imm, int Rs_Imm, int Rm_or_Imm, int Rm_Imm);
int add_std_assemble(func_block *fb, int i, enum instruction ins, int u0, int u1, int u2);
int add_imm_assemble(func_block *fb, int i, enum instruction ins, int u0, int u1, int imm);
int check_bool_use(func_block *fb, int i);
int assign_bool_value(enum instruction ins, func_block *fb, int i);
int compare_operation(func_block *fb, int i, int cond, int m);
int search_label(func_block *fb, int i, int m, int not_flag);
int check_tail_recursion(func_block *fb, int i);
int tail_recursion(func_block *fb, int i);

int gen_target_code()
{
	initial();
	memory_allocation();
	setLabel();
	convert();
	return 0;
}

int initial()
{
	int i;
	func_block *fb;
	for (fb = fblist; fb != NULL; fb = fb->next)
	{
		for (i = 0; i < 32; i++)
			fb->reg_var[i] = -1;
	}
	return 0;
}

int memory_allocation()
{
	int i, off, para_num;
	func_block *fb;
	for (fb = fblist; fb != NULL; fb = fb->next)
	{
		para_num = (triple_list[index_index[fb->start->begin]].symtbl)->para_num;
		for (i = 4, off = 4; i < para_num; i++, off += 4)
			fb->uni_table[i]->offset = off;
		if (fb->reg_used < 10)
			off = -16;
		else
			off = -16 - (fb->reg_used - 9) * 4;
		for (i = 0; i < 4 && i < para_num; i++)
		{
			fb->uni_table[i]->offset = off;
			off -= 4;
		}
		for (i = para_num; i < fb->uni_item_num; i++)
		{
			if (fb->uni_table[i]->isGlobal)
				continue;
			if (!(fb->mapping[i].isTmp == 1 && fb->reg_alloc[i] != -1))
			{
				if (fb->uni_table[i]->size != -1)
				{
					if (fb->uni_table[i]->type == INT_T)
						off -= 4*fb->uni_table[i]->size;
					else
						off -= (fb->uni_table[i]->size+3)/4;
				}
				fb->uni_table[i]->offset = off;
				off -= 4;
			}
		}
		fb->min_stack_size = -off;
	}
	return 0;
}

int setLabel()
{
	int i, j, l;
	for (i = 0; i < triple_list_index; i++)
		triple_list[index_index[i]].label = -1;
	for (i = 0, l = 1; i < triple_list_index; i++)
	{
		if (triple_list[index_index[i]].is_deleted)
			continue;
		if (triple_list[index_index[i]].op == enterF)
			triple_list[index_index[i+1]].label = l++;
		if (triple_list[index_index[i]].op == goto_op)
		{
			j = triple_list[index_index[i]].arg1.temp_index;
			if (triple_list[index_index[j]].label == -1)		
				triple_list[index_index[j]].label = l++;
		}
		if (triple_list[index_index[i]].op == if_op || triple_list[index_index[i]].op == if_not_op)
		{
			if (!(triple_list[index_index[i+1]].op == set_rb && triple_list[index_index[i+2]].op == get_rb))
			{
				if (triple_list[index_index[i+1]].label == -1)
					triple_list[index_index[i+1]].label = l++;
			}
			j = triple_list[index_index[i]].arg2.temp_index;
			if (triple_list[index_index[j]].op != get_rb && triple_list[index_index[j]].label == -1)
					triple_list[index_index[j]].label = l++;
		}
		if (triple_list[index_index[i]].op == get_rb || triple_list[index_index[i]].op == not_op)
		{
			if (check_bool_use((triple_list[index_index[i]].block)->fb, i))
			{
				if (triple_list[index_index[i]].label == -1)
				{
					triple_list[index_index[i]].label = l;
					l += 2;
				}
				if (triple_list[index_index[i+1]].label == -1)
					triple_list[index_index[i+1]].label = l++;
			}
		}
		if (triple_list[index_index[i]].op == leaveF)
		{
			triple_list[index_index[i]].label = l++;
			(triple_list[index_index[i]].block)->fb->global_label = l++;
		}
	}
	for (i = triple_list_index-2; i >= 0; i--)
	{
		if (triple_list[index_index[i]].is_deleted && triple_list[index_index[i]].label != -1)
		{
			for (j = i+1; triple_list[index_index[j]].is_deleted; j++)
				;
			if (triple_list[index_index[j]].label != -1)
				triple_list[index_index[i]].label = triple_list[index_index[j]].label;
			else
				triple_list[index_index[j]].label = triple_list[index_index[i]].label;
		}
	}
	//for (i = 0; i < triple_list_index; i++)
	//	fprintf(stderr, "(%d) %d\n", i, triple_list[index_index[i]].label);
	return 0;
}

int convert()
{
	int i, t_size, p_size;
	char *temp;
	func_block *fb;
	symtbl_hdr *p = tree_root->symtbl;
	assemble_list = (assemble*)malloc(64*sizeof(assemble));
	assemble_size = 64*sizeof(assemble);
	assemble_num = 0;
	c_str_list = (char**)malloc(4*sizeof(char*));
	c_str_maxSize = 4 * sizeof(char*);
	c_str_num = 0;
	for (i = 0; i < p->item_num; i++)
	{
		if (p->item[i].type != FUNCTION_DEF)
		{
			if (p->item[i].type == CHAR_T)
			{
				if (p->item[i].star_num == 0 && p->item[i].size == -1)
					p_size = 1;
				else
					p_size = 4;
				t_size = 1;
			}
			else
				p_size = t_size = 4;
			if (p->item[i].size != -1)
				t_size *= p->item[i].size;
			temp = (char*)malloc(32*sizeof(char));
			sprintf(temp, "\t.comm\t%s, %d, %d", p->item[i].name, t_size, p_size);
			add_special(temp, special);
		}
	}
	for (fb = fblist; fb != NULL; fb = fb->next)
	{
		func_assem_init(fb);
		for (i = fb->start->begin; i <= fb->over->end; i++)
		{
			if (triple_list[index_index[i]].is_deleted)
				continue;
			if (!triple_list[index_index[i]].is_deleted && triple_list[index_index[i]].label != -1)
				add_assemble(triple_list[index_index[i]].label, label, -1, -1, 0, 0, -1, 0, -1);
			if (triple_list[index_index[i]].op != enterF)
				load_live(fb, i);
			g[triple_list[index_index[i]].op-3000](fb, i);
		}
		func_assem_end(fb);
	}
	return 0;
}

int search_const_str(char *target)
{
	int j;
	for (j = 0; j < c_str_num; j++)
		if (!strcmp(target, c_str_list[j]))
			return j;
	return -1;
}

int set_cstr_used(func_block *fb, int j)
{
	CStrList *p;
	if (fb->c_str_used == NULL)
	{
		fb->c_str_used = (CStrList*)malloc(sizeof(CStrList));
		fb->c_str_used->c_str = j;
		fb->c_str_used->next = NULL;
	}
	else
	{
		for (p = fb->c_str_used; p->next != NULL; p = p->next)
			if (j == p->c_str)
				return 0;
		if (j == p->c_str)
			return 0;
		p->next = (CStrList*)malloc(sizeof(CStrList));
		p->next->c_str = j;
		p->next->next = NULL;
	}
	return 0;
}

int func_assem_init(func_block *fb)
{
	int i, j, off, first_flag = 1;
	char *temp;
	fb->c_str_used = NULL;
	for (i = fb->start->begin; i <= fb->over->end; i++)
	{
		if (triple_list[index_index[i]].op == c_str)
		{
			j = search_const_str(triple_list[index_index[i]].arg1.var_name);
			if (j == -1)
			{
				if ((c_str_num+1)*sizeof(char*) > c_str_maxSize)
					adjustSize((void**)&c_str_list, &c_str_maxSize);
				j = c_str_num++;
				c_str_list[j] = triple_list[index_index[i]].arg1.var_name;
				if (first_flag)
				{
					temp = (char*)malloc(32*sizeof(char));
					sprintf(temp, "\t.section	.rodata");
					add_special(temp, special);
					first_flag = 0;
				}
				temp = (char*)malloc(16*sizeof(char));
				sprintf(temp, ".LC%d", j);
				add_special(temp, label);
				temp = (char*)malloc((strlen(c_str_list[j])+16)*sizeof(char));
				sprintf(temp, "\t.ascii	\"%s\\000\"", c_str_list[j]);
				add_special(temp, special);								
			}
			set_cstr_used(fb, j);
		}
	}
	for (i = 0, off = 0; i < fb->uni_item_num; i++)
	{
		if (fb->uni_table[i]->isGlobal)
		{
			fb->uni_table[i]->offset = off;
			off += 4;
		}
	}
	fb->const_off = off;
	temp = (char*)malloc(6*sizeof(char));
	sprintf(temp, "\t.text");
	add_special(temp, special);
	temp = (char*)malloc(32*sizeof(char));
	sprintf(temp, "\t.global\t%s", (triple_list[index_index[fb->start->begin]].symtbl)->func_name);
	add_special(temp, special);
	temp = (char*)malloc(48*sizeof(char));
	sprintf(temp, "\t.type\t%s,function", (triple_list[index_index[fb->start->begin]].symtbl)->func_name);
	add_special(temp, special);
	add_special((triple_list[index_index[fb->start->begin]].symtbl)->func_name, label);
	return 0;
}

int func_assem_end(func_block *fb)
{
	int i;
	char *temp;
	CStrList *p;
	add_assemble(fb->global_label, label, -1, -1, 0, 0, -1, 0, -1);
	for (i = 0; i < fb->uni_item_num; i++)
	{
		if (fb->uni_table[i]->isGlobal == 1)
		{
			temp = (char*)malloc(32*sizeof(char));
			sprintf(temp, "\t.word\t%s", fb->uni_table[i]->name);
			add_special(temp, special);
		}
	}
	for (p = fb->c_str_used; p != NULL; p = p->next)
	{
		temp = (char*)malloc(32*sizeof(char));
		sprintf(temp, "\t.word\t.LC%d", p->c_str);
		add_special(temp, special);
	}
	return 0;
}

int load_live(func_block *fb, int i)
{
	int j, r;
	unsigned int *live = fb->live_status[i-fb->start->begin];
	for (j = 0; j < fb->uni_item_num; j++)
	{
		if (fb->mapping[j].isTmp != 1 && (live[j/32] >> (31-j%32)) % 2 == 1)
		{
			r = fb->reg_alloc[j];
			if (r != -1 && fb->reg_var[r] != j)
			{
				fb->reg_var[r] = j;
				if (!fb->uni_table[j]->isGlobal)
					add_assemble(-1, ldw, 27, r, 0, 0, -1, 1, fb->uni_table[j]->offset);
				else
				{
					if (fb->uni_table[j]->size != -1)
						add_assemble(fb->global_label, ldw, -1, r, 0, 0, -1, 1, fb->uni_table[j]->offset);
					else
					{
						add_assemble(fb->global_label, ldw, -1, 1, 0, 0, -1, 1, fb->uni_table[j]->offset);
						add_assemble(-1, ldw, 1, r, 0, 0, -1, 1, 0);
					}
				}
			}
		}
	}
	return 0;
}

int add_assemble(int label, enum instruction ins, int Rn, int Rd, int shift_direct, int Rs_or_Imm, int Rs_Imm, int Rm_or_Imm, int Rm_Imm)
{
	if ((assemble_num+1)*sizeof(assemble) > assemble_size)
		adjustSize((void**)&assemble_list, &assemble_size);
	assemble_list[assemble_num].content = NULL;	
	assemble_list[assemble_num].label = label;
	assemble_list[assemble_num].ins = ins;
	assemble_list[assemble_num].Rn = Rn;
	assemble_list[assemble_num].Rd = Rd;
	assemble_list[assemble_num].shift_direct = shift_direct;	
	assemble_list[assemble_num].Rs_or_Imm = Rs_or_Imm;
	assemble_list[assemble_num].Rs_Imm = Rs_Imm;
	assemble_list[assemble_num].Rm_or_Imm = Rm_or_Imm;
	assemble_list[assemble_num].Rm_Imm = Rm_Imm;
	assemble_num++;
	return 0;
}

/*int add_assemble_imm(enum instruction ins, int Rn, int Rd, int Imm)
{
	if (ins == ldw || ins == stw || ins == ldb || ins == stb)
	{
		if (Imm < )
	}
	else
	{
	}
	return 0;
}*/

int add_special(char *content, enum instruction ins)
{	
	if ((assemble_num+1)*sizeof(assemble) > assemble_size)  
		adjustSize((void**)&assemble_list, &assemble_size);
	assemble_list[assemble_num].content = content;	
	assemble_list[assemble_num].label = -1;
	assemble_list[assemble_num].ins = ins;
	assemble_list[assemble_num].Rn = -1;
	assemble_list[assemble_num].Rd = -1;
	assemble_list[assemble_num].shift_direct = 0;	
	assemble_list[assemble_num].Rs_or_Imm = 0;
	assemble_list[assemble_num].Rs_Imm = -1;
	assemble_list[assemble_num].Rm_or_Imm = 0;
	assemble_list[assemble_num].Rm_Imm = -1;
	assemble_num++;
	return 0;
}

int check_live(func_block *fb, int i, int isTmp)
{
	int u, base = fb->start->begin;
	if (isTmp == 1)
		u = triple_list[index_index[i]].tmp_uni;
	else
		u = triple_list[index_index[i]].arg1_uni;
	if ((fb->live_status[i+1-base][u/32] >> (31-u%32)) % 2 == 1)
		return 1;
	return 0;
}

int load_operator(func_block *fb, int u, int r, int _r)
{
	if (r == -1)
	{
		r = _r;
		if (fb->uni_table[u]->isGlobal != 1)
			add_assemble(-1, ldw, 27, r, 0, 0, -1, 1, fb->uni_table[u]->offset);
		else
		{
			if (fb->uni_table[u]->size != -1)
				add_assemble(fb->global_label, ldw, -1, r, 0, 0, -1, 1, fb->uni_table[u]->offset);
			else
			{
				add_assemble(fb->global_label, ldw, -1, 1, 0, 0, -1, 1, fb->uni_table[u]->offset);
				add_assemble(-1, ldw, 1, r, 0, 0, -1, 1, 0);
			}
		}
	}
	return r;
}

int store_result(func_block *fb, int i, enum instruction ins, int u, int Rs, int Rd, int shift_direct, int Rs_or_Imm, int Rs_Imm, int Rm_or_Imm, int Rm_Imm)
{
	if (Rd == -1)
	{
		Rd = 3;
		add_assemble(-1, ins, Rs, Rd, shift_direct, Rs_or_Imm, Rs_Imm, Rm_or_Imm, Rm_Imm);
		if (fb->uni_table[u]->isGlobal != 1)
			add_assemble(-1, stw, 27, Rd, 0, 0, -1, 1, fb->uni_table[u]->offset);
		else
		{
			add_assemble(fb->global_label, ldw, -1, 1, 0, 0, -1, 1, fb->uni_table[u]->offset);
			add_assemble(-1, stw, 1, Rd, 0, 0, -1, 1, 0);
		}
	}
	else
	{
		if (fb->uni_table[u]->isGlobal)
		{
			if ((fb->live_status[i+1-fb->start->begin][u/32]>>(31-u%32)) % 2 == 1)
				fb->reg_var[Rd] = u;
			else
				Rd = 3;
			add_assemble(-1, ins, Rs, Rd, shift_direct, Rs_or_Imm, Rs_Imm, Rm_or_Imm, Rm_Imm);
			add_assemble(fb->global_label, ldw, -1, 1, 0, 0, -1, 1, fb->uni_table[u]->offset);
			add_assemble(-1, stw, 1, Rd, 0, 0, -1, 1, 0);
		}
		else
		{
			fb->reg_var[Rd] = u;
			if (!(ins == mov && Rm_or_Imm == 0 && Rd == Rm_Imm && Rs_or_Imm == 0 && Rs_Imm == -1))
				add_assemble(-1, ins, Rs, Rd, shift_direct, Rs_or_Imm, Rs_Imm, Rm_or_Imm, Rm_Imm);
		}
	}
	return 0;
}

int add_std_assemble(func_block *fb, int i, enum instruction ins, int u0, int u1, int u2)
{
	int r0, r1, r2;
	r0 = fb->reg_alloc[u0];
	r1 = fb->reg_alloc[u1];
	r2 = fb->reg_alloc[u2];
	r1 = load_operator(fb, u1, r1, 1);
	r2 = load_operator(fb, u2, r2, 2);
	store_result(fb, i, ins, u0, r1, r0, 0, 0, -1, 0, r2);
	return 0;
}

int add_imm_assemble(func_block *fb, int i, enum instruction ins, int u0, int u1, int imm)
{
	int r0, r1;
	r0 = fb->reg_alloc[u0];
	r1 = fb->reg_alloc[u1];
	r1 = load_operator(fb, u1, r1, 1);
	store_result(fb, i, ins, u0, r1, r0, 0, 0, -1, 1, imm);
	return 0;
}

int check_bool_use(func_block *fb, int i)
{
	int j, base, l, u1, u2;
	enum operator op;
	base = fb->start->begin;
	l = triple_list[index_index[i]].tmp_uni;
	if (l == -1)
		return 0;
	for (j = i+1-base; j < fb->code_num; j++)
	{
		if ((fb->live_status[j][l/32] >> (31-l%32)) % 2 == 0)
			break;
		u1 = triple_list[index_index[j+base]].arg1_uni;
		u2 = triple_list[index_index[j+base]].arg2_uni;
		op = triple_list[index_index[j+base]].op;
		if ((u1 == l || u2 == l) && op != if_op && op != if_not_op && op != not_op)
			return 1;
	}
	return 0;
}

int assign_bool_value(enum instruction ins, func_block *fb, int i)
{
	int u0, u1, u2, r0, r1, r2, isImm;
	u0 = triple_list[index_index[i]].tmp_uni;
	u1 = triple_list[index_index[i]].arg1_uni;
	u2 = triple_list[index_index[i]].arg2_uni;
	r0 = fb->reg_alloc[u0];
	if (u1 == -1)
	{
		r1 = 1;
		add_assemble(-1, mov, -1, r1, 0, 0, -1, 1, triple_list[index_index[i]].arg1.imm_value);
	}
	else
		r1 = load_operator(fb, u1, fb->reg_alloc[u1], 1);
	if (u2 == -1)
	{
		isImm = 1;
		r2 = triple_list[index_index[i]].arg2.imm_value;
	}
	else
		r2 = load_operator(fb, u2, fb->reg_alloc[u2], 2);
	if (r0 == -1)
	{
		add_assemble(-1, cmpsub_a, r1, -1, 0, 0, -1, isImm, r2);
		add_assemble(-1, mov, -1, 3, 0, 0, -1, 1, 0);
		add_assemble(-1, ins, -1, 3, 0, 0, -1, 1, 1);
		add_assemble(-1, stw, 27, 3, 0, 0, -1, 1, fb->uni_table[u0]->offset);
	}
	else
	{
		add_assemble(-1, cmpsub_a, r1, -1, 0, 0, -1, isImm, r2);
		fb->reg_var[r0] = u0;
		add_assemble(-1, mov, -1, r0, 0, 0, -1, 1, 0);
		add_assemble(-1, ins, -1, r0, 0, 0, -1, 1, 1);
	}
	return 0;
}

int compare_operation(func_block *fb, int i, int cond, int m)
{
	int u1, u2, r1, r2, t1, t2;
	if (cond == 1)
	{
		u1 = triple_list[index_index[i]].arg1_uni;
		if (u1 != -1)
		{
			r1 = fb->reg_alloc[u1];
			r1 = load_operator(fb, u1, r1, 1);
		}
		else
		{
			r1 = 1;
			add_assemble(-1, mov, -1, r1, 0, 0, -1, 1, triple_list[index_index[i]].arg1.imm_value);
		}
		add_assemble(-1, cmpsub_a, r1, -1, 0, 0, -1, 1, 0);			
	}
	if (cond == 2)
	{
		t1 = triple_list[index_index[i]].arg1_type;
		t2 = triple_list[index_index[i]].arg2_type;
		u1 = triple_list[index_index[i]].arg1_uni;
		u2 = triple_list[index_index[i]].arg2_uni;
		if (t1 < 2 && t2 < 2)
		{
			r1 = fb->reg_alloc[u1];
			r2 = fb->reg_alloc[u2];
			r1 = load_operator(fb, u1, r1, 1);
			r2 = load_operator(fb, u2, r2, 2);
			add_assemble(-1, cmpsub_a, r1, -1, 0, 0, -1, 0, r2);
		}
		else
		{
			if (t1 < 2)
			{
				r1 = fb->reg_alloc[u1];
				r1 = load_operator(fb, u1, r1, 1);
				add_assemble(-1, cmpsub_a, r1, -1, 0, 0, -1, 1, triple_list[index_index[i]].arg2.imm_value);
			}
			if (t2 < 2)
			{
				r2 = fb->reg_alloc[u2];
				r2 = load_operator(fb, u2, r2, 2);
				add_assemble(-1, cmpsub_a, r2, -1, 0, 0, -1, 1, triple_list[index_index[i]].arg1.imm_value);
				if (m != 2 && m != 3)
					m = 5 - m;
			}
		}
	}
	if (cond == 3)
	{
		u1 = triple_list[index_index[i]].tmp_uni;
		r1 = fb->reg_alloc[u1];
		r1 = load_operator(fb, u1, r1, 1);
		add_assemble(-1, cmpsub_a, r1, -1, 0, 0, -1, 1, 0);
	}
	return m;
}

int search_label(func_block *fb, int i, int m, int not_flag)
{
	enum operator op;
	int j, flag, bool_assign_flag, l;
	op = triple_list[index_index[i]].op;
	j = i;
	flag = 0;
	bool_assign_flag = 0;
	while (1)
	{
		if (!(triple_list[index_index[j+1]].op == set_rb && triple_list[index_index[j+2]].op == get_rb))
		{
			if ((op != triple_list[index_index[j]].op && not_flag == 0) || (op == triple_list[index_index[j]].op && not_flag == 1))
			{
				if (flag == 0 && op == triple_list[index_index[i]].op)
					op = (op == if_op)?if_not_op:if_op;
				else
				{
					j++;
					break;
				}
			}
			else
			{
				if (op == triple_list[index_index[i]].op)
					flag = 1;
			}
		}
		j = triple_list[index_index[j]].arg2.temp_index;
		if (triple_list[index_index[j]].op != get_rb)
			break;
		while (triple_list[index_index[j]].op != if_op && triple_list[index_index[j]].op != if_not_op)
		{
			if (triple_list[index_index[j]].op == not_op)
				not_flag = 1 - not_flag;
			if (check_bool_use(fb, j))
			{
				bool_assign_flag = 1;
				break;
			}			
			j++;
		}
		if (bool_assign_flag)
			break;
	}
	if (!bool_assign_flag)
	{
		if (triple_list[index_index[i]].op == if_op)
			add_assemble(triple_list[index_index[j]].label, map_ins[m][1-flag], -1, -1, 0, 0, -1, 0, -1);
		else
			add_assemble(triple_list[index_index[j]].label, map_ins[m][flag], -1, -1, 0, 0, -1, 0, -1);
	}
	else
	{
		l = triple_list[index_index[j]].label;
		if ((op == if_not_op && not_flag == 0) || (op == if_op && not_flag == 1))
		{
			if (op == triple_list[index_index[i]].op)
				flag = 1;
			l++;
		}
		else
		{
			if (flag == 0 && op == triple_list[index_index[i]].op)
				l++;
		}
		if (triple_list[index_index[i]].op == if_op)
			add_assemble(l, map_ins[m][1-flag], -1, -1, 0, 0, -1, 0, -1);
		else
			add_assemble(l, map_ins[m][flag], -1, -1, 0, 0, -1, 0, -1);
	}
	return 0;
}

int if_goto_code(func_block *fb, int i)
{
	int j, m, cond, not_flag;
	enum operator op;
	not_flag = 0;
	j = i;
	while (triple_list[index_index[j]].arg1_type == 1)
	{
		j = triple_list[index_index[j]].arg1.temp_index;
		op = triple_list[index_index[j]].op;
		if (op != not_op)
			break;
		not_flag = 1 - not_flag;
	}
	op = triple_list[index_index[j]].op;
	if ((op == if_op || op == if_not_op || op == not_op) && triple_list[index_index[j]].arg1_type != 1)
	{
		compare_operation(fb, i, 1, 3);
		search_label(fb, i, 3, not_flag);
	}
	else
	{
		if (op != get_rb)
		{
			cond = 2;
			switch (op)
			{
				case equal_op:
					m = 2;
					break;
				case less_op:
					m = 0;
					break;
				case larger_op:
					m = 5;
					break;
				case eqlarger_op:
					m = 4;
					break;
				case eqless_op:
					m = 1;
					break;
				case noteq_op:
					m = 3;
					break;
				default:
					m = 3;
					cond = 3;
			}
			m = compare_operation(fb, j, cond, m);
			search_label(fb, i, m, not_flag);
		}
	}
	return 0;
}

int goto_code(func_block *fb, int i)
{
	int lb;
	lb = triple_list[index_index[triple_list[index_index[i]].arg1.temp_index]].label;
	add_assemble(lb, b, -1, -1, 0, 0, -1, 0, -1);
	return 0;
}

int negative_code(func_block *fb, int i)
{
	int u0, u1, r0, r1;
	u0 = triple_list[index_index[i]].tmp_uni;
	r0 = fb->reg_alloc[u0];
	u1 = triple_list[index_index[i]].arg1_uni;
	if (u1 == -1)
		store_result(fb, i, mov, u0, -1, r0, 0, 0, -1, 1, -triple_list[index_index[i]].arg1.imm_value);
	else
	{
		r1 = load_operator(fb, u1, fb->reg_alloc[u1], 1);
		store_result(fb, i, rsub, u0, r1, r0, 0, 0, -1, 1, 0);
	}
	return 0;
}

int not_code(func_block *fb, int i)
{
	int j, u0, u1, r0, r1, not_flag, imm, v;
	if (check_bool_use(fb, i))
	{
		
		for (j = i, not_flag = 1; ; )
		{
			if (!(triple_list[index_index[j]].arg1_type == 1 
				&& triple_list[index_index[triple_list[index_index[j]].arg1.temp_index]].op == not_op))
				break;
			not_flag = 1 - not_flag;
			j = triple_list[index_index[j]].arg1.temp_index;
		} 
		u0 = triple_list[index_index[i]].tmp_uni;
		r0 = fb->reg_alloc[u0];
		if (triple_list[index_index[j]].arg1_type == 1 
				&& triple_list[index_index[triple_list[index_index[j]].arg1.temp_index]].op == get_rb)
		{
			store_result(fb, i, mov, u0, -1, r0, 0, 0, -1, 1, 1);
			add_assemble(triple_list[index_index[i+1]].label, b, -1, -1, 0, 0, -1, 0, -1);
			add_assemble(triple_list[index_index[i]].label+1, label, -1, -1, 0, 0, -1, 0, -1);
			store_result(fb, i, mov, u0, -1, r0, 0, 0, -1, 1, 0);
		}
		else
		{
			if (triple_list[index_index[j]].arg1_type < 2)
			{
				u1 = triple_list[index_index[j]].arg1_uni;
				r1 = fb->reg_alloc[u1];
				r1 = load_operator(fb, u1, r1, 1);
				add_assemble(-1, cmpsub_a, r1, -1, 0, 0, -1, 1, 0);
				if (r0 == -1)
				{
					add_assemble(-1, mov, -1, 3, 0, 0, -1, 1, 0);
					if (not_flag)
						add_assemble(-1, cmoveq, -1, 3, 0, 0, -1, 1, 1);
					else
						add_assemble(-1, cmovne, -1, 3, 0, 0, -1, 1, 1);
					add_assemble(-1, stw, 27, 3, 0, 0, -1, 1, fb->uni_table[u0]->offset);
				}
				else
				{
					fb->reg_var[r0] = u0;
					if (not_flag)
						add_assemble(-1, cmoveq, -1, r0, 0, 0, -1, 1, 1);
					else
						add_assemble(-1, cmovne, -1, r0, 0, 0, -1, 1, 1);
				}
			}
			else
			{
				imm = (triple_list[index_index[j]].arg1.imm_value == 0)?0:1;
				if ((imm + not_flag) % 2 == 1)
					v = 1;
				else
					v = 0;
				store_result(fb, i, mov, u0, -1, r0, 0, 0, -1, 1, v);
			}
		}
	}
	return 0;
}

int address_code(func_block *fb, int i)
{
	int u0, u1, r0;
	if (check_live(fb, i, 1))
	{
		u0 = triple_list[index_index[i]].tmp_uni;
		u1 = triple_list[index_index[i]].arg1_uni;
		r0 = fb->reg_alloc[u0];
		if (!fb->uni_table[u1]->isGlobal)
			store_result(fb, i, add, u0, 27, r0, 0, 0, -1, 1, fb->uni_table[u1]->offset);
		else
		{
			if (r0 != -1)
			{
				fb->reg_var[r0] = u0;
				add_assemble(fb->global_label, ldw, -1, r0, 0, 0, -1, 1, fb->uni_table[u1]->offset);
			}
			else
			{
				r0 = 3;
				add_assemble(fb->global_label, ldw, -1, r0, 0, 0, -1, 1, fb->uni_table[u1]->offset);
				add_assemble(-1, stw, 27, r0, 0, 0, -1, 1, fb->uni_table[u0]->offset);
			}
		}
	}
	return 0;
}

int star_code(func_block *fb, int i)
{
	int u0, u1, r0, r1;
	if (check_live(fb, i, 1))
	{
		u0 = triple_list[index_index[i]].tmp_uni;
		r0 = fb->reg_alloc[u0];
		u1 = triple_list[index_index[i]].arg1_uni;
		r1 = fb->reg_alloc[u1];
		r1 = load_operator(fb, u1, r1, 1);
		if (triple_list[index_index[i]].result_type != 0)
			store_result(fb, i, ldw, u0, r1, r0, 0, 0, -1, 1, 0);
		else
			store_result(fb, i, ldb, u0, r1, r0, 0, 0, -1, 1, 0);
	}
	return 0;
}

int positive_code(func_block *fb, int i)
{
	return 0;
}

int assign_code(func_block *fb, int i)
{
	int u0, u1, u2, r0, r1, r2;
	u0 = triple_list[index_index[i]].tmp_uni;
	u1 = triple_list[index_index[i]].arg1_uni;
	u2 = triple_list[index_index[i]].arg2_uni;
	r0 = fb->reg_alloc[u0];
	r1 = fb->reg_alloc[u1];
	if (u2 != -1)
	{
		r2 = fb->reg_alloc[u2];
		r2 = load_operator(fb, u2, r2, 2);
		if (check_live(fb, i, 0) || fb->uni_table[u1]->isGlobal)
			store_result(fb, i, mov, u1, -1, r1, 0, 0, -1, 0, r2);
		if (check_live(fb, i, 1))
			store_result(fb, i, mov, u0, -1, r0, 0, 0, -1, 0, r2);
	}
	else
	{
		if (check_live(fb, i, 0) || fb->uni_table[u1]->isGlobal)
			store_result(fb, i, mov, u1, -1, r1, 0, 0, -1, 1, triple_list[index_index[i]].arg2.imm_value);
		if (check_live(fb, i, 1))
			store_result(fb, i, mov, u0, -1, r0, 0, 0, -1, 1, triple_list[index_index[i]].arg2.imm_value);
	}
	return 0;
}

int star_assign_code(func_block *fb, int i)
{
	int u0, u1, u2, r0, r1, r2;
	u0 = triple_list[index_index[i]].tmp_uni;
	u1 = triple_list[index_index[i]].arg1_uni;
	u2 = triple_list[index_index[i]].arg2_uni;
	r1 = fb->reg_alloc[u1];
	r1 = load_operator(fb, u1, r1, 1);
	if (u2 != -1)
	{
		r2 = fb->reg_alloc[u2];
		r2 = load_operator(fb, u2, r2, 3);
	}
	else
	{
		r2 = 3;
		add_assemble(-1, mov, -1, r2, 0, 0, -1, 1, triple_list[index_index[i]].arg2.imm_value);
	}
	//fprintf(stderr,"result_type: %d\n",triple_list[index_index[i]].result_type );
	if (triple_list[index_index[i]].result_type != 0)
		add_assemble(-1, stw, r1, r2, 0, 0, -1, 1, 0);
	else
		add_assemble(-1, stb, r1, r2, 0, 0, -1, 1, 0);
	if (check_live(fb, i, 1))
	{
		r0 = fb->reg_alloc[u0];
		store_result(fb, i, mov, u0, -1, r0, 0, 0, -1, 0, r2);
	}
	return 0;
}

int add_code(func_block *fb, int i)
{
	int type1, type2, u0, u1, u2;
	if (check_live(fb, i, 1))
	{
		type1 = triple_list[index_index[i]].arg1_type;
		type2 = triple_list[index_index[i]].arg2_type;
		u0 = triple_list[index_index[i]].tmp_uni;
		u1 = triple_list[index_index[i]].arg1_uni;
		u2 = triple_list[index_index[i]].arg2_uni;
		if (type1 < 2 && type2 < 2)
			add_std_assemble(fb, i, add, u0, u1, u2);
		else
		{
			if (type1 < 2)
				add_imm_assemble(fb, i, add, u0, u1, triple_list[index_index[i]].arg2.imm_value);
			if (type2 < 2)
				add_imm_assemble(fb, i, add, u0, u2, triple_list[index_index[i]].arg1.imm_value);
		}
	}
	return 0;
}

int minus_code(func_block *fb, int i)
{
	int type1, type2, u0, u1, u2;
	if (check_live(fb, i, 1))
	{
		u0 = triple_list[index_index[i]].tmp_uni;
		u1 = triple_list[index_index[i]].arg1_uni;
		u2 = triple_list[index_index[i]].arg2_uni;
		type1 = triple_list[index_index[i]].arg1_type;
		type2 = triple_list[index_index[i]].arg2_type;
		if (type1 < 2 && type2 < 2)
			add_std_assemble(fb, i, sub, u0, u1, u2);
		else
		{
			if (type1 < 2)
				add_imm_assemble(fb, i, sub, u0, u1, triple_list[index_index[i]].arg2.imm_value);
			if (type2 < 2)
				add_imm_assemble(fb, i, rsub, u0, u2, triple_list[index_index[i]].arg1.imm_value);
		}
	}
	return 0;
}

int multiply_code(func_block *fb, int i)
{
	int type1, type2, u0, u1, u2, r0, r1, r2;
	if (check_live(fb, i, 1))
	{
		type1 = triple_list[index_index[i]].arg1_type;
		type2 = triple_list[index_index[i]].arg2_type;
		u0 = triple_list[index_index[i]].tmp_uni;
		u1 = triple_list[index_index[i]].arg1_uni;
		u2 = triple_list[index_index[i]].arg2_uni;
		r0 = fb->reg_alloc[u0];
		if (type1 < 2 && type2 < 2)
			add_std_assemble(fb, i, mul, u0, u1, u2);
		else
		{
			if (type1 < 2)
			{
				r1 = load_operator(fb, u1, fb->reg_alloc[u1], 1);
				r2 = 2;
				add_assemble(-1, mov, -1, r2, 0, 0, -1, 1, triple_list[index_index[i]].arg2.imm_value);
			}
			if (type2 < 2)
			{
				r1 = 1;
				r2 = load_operator(fb, u1, fb->reg_alloc[u2], 2);
				add_assemble(-1, mov, -1, r1, 0, 0, -1, 1, triple_list[index_index[i]].arg1.imm_value);
			}
			store_result(fb, i, mul, u0, r1, r0, 0, 0, -1, 0, r2);
		}
	}
	return 0;
}

int char_int_code(func_block *fb, int i)
{
	int u0, u1, r0, r1;
	u0 = triple_list[index_index[i]].tmp_uni;
	u1 = triple_list[index_index[i]].arg1_uni;
	r0 = fb->reg_alloc[u0];
	if (u1 != -1)
	{
		r1 = load_operator(fb, u1, fb->reg_alloc[u1], 2);
		store_result(fb, i, mov, u0, -1, r0, 0, 0, -1, 0, r1);
	}
	else
		store_result(fb, i, mov, u0, -1, r0, 0, 0, -1, 1, triple_list[index_index[i]].arg1.imm_value);
	return 0;
}

int equal_code(func_block *fb, int i)
{
	if (check_bool_use(fb, i))
		assign_bool_value(cmoveq, fb, i);
	return 0;
}

int less_code(func_block *fb, int i)
{
	if (check_bool_use(fb, i))
		assign_bool_value(cmovsl, fb, i);
	return 0;
}

int larger_code(func_block *fb, int i)
{
	if (check_bool_use(fb, i))
		assign_bool_value(cmovsg, fb, i);
	return 0;
}

int eqlarger_code(func_block *fb, int i)
{
	if (check_bool_use(fb, i))
		assign_bool_value(cmoveg, fb, i);
	return 0;
}

int eqless_code(func_block *fb, int i)
{
	if (check_bool_use(fb, i))
		assign_bool_value(cmovel, fb, i);
	return 0;
}

int noteq_code(func_block *fb, int i)
{
	if (check_bool_use(fb, i))
		assign_bool_value(cmovne, fb, i);
	return 0;
}

int or_code(func_block *fb, int i)
{
	return 0;
}

int and_code(func_block *fb, int i)
{
	return 0;
}

int get_rb_code(func_block *fb, int i)
{
	int u0, r0;
	if (check_bool_use(fb, i))
	{
		u0 = triple_list[index_index[i]].tmp_uni;
		r0 = fb->reg_alloc[u0];
		store_result(fb, i, mov, u0, -1, r0, 0, 0, -1, 1, 1);
		add_assemble(triple_list[index_index[i+1]].label, b, -1, -1, 0, 0, -1, 0, -1);
		add_assemble(triple_list[index_index[i]].label+1, label, -1, -1, 0, 0, -1, 0, -1);
		store_result(fb, i, mov, u0, -1, r0, 0, 0, -1, 1, 0);
	}
	return 0;
}

int set_rb_code(func_block *fb, int i)
{
	return 0;
}

int check_tail_recursion(func_block *fb, int i)
{
	if (strcmp(triple_list[index_index[i]].arg2.var_name, (triple_list[index_index[fb->start->begin]].symtbl)->func_name))
		return 0;
	if (triple_list[index_index[i+1]].op == leaveF)
		return 1;
	if (triple_list[index_index[i+1]].op == return_op && triple_list[index_index[i+2]].op == leaveF)
	{
		if (triple_list[index_index[i+1]].arg1_type == -1)
			return 1;
	}
	return 0;
}

int tail_recursion(func_block *fb, int i)
{
	int j, k, r, u;
	int para_num = (triple_list[index_index[fb->start->begin]].symtbl)->para_num;
	for (j = 0, k = 4; j < para_num; j++)
	{
		if (!((fb->live_status[1][j/32]>>(31-j%32)) & 1))
			continue;
		u = triple_list[index_index[i-para_num+j]].arg1_uni;
		if (u != -1)
		{
			r = load_operator(fb, u, fb->reg_alloc[u], 28);
			if (j < 4)
			{
				if (u != j)
					store_result(fb, i, mov, j, -1, fb->reg_alloc[j], 0, 0, -1, 0, r);
				//else
				//	add_assemble(-1, stw, 27, r, 0, 0, -1, 1, fb->uni_table[j]->offset);
			}
			else
			{
				add_assemble(-1, stw, 27, r, 0, 0, -1, 1, k);
				k += 4;
			}
		}
		else
		{
			if (j < 4)
				store_result(fb, i, mov, j, -1, fb->reg_alloc[j], 0, 0, -1, 1, triple_list[index_index[i-para_num+j]].arg1.imm_value);
			else
			{
				add_assemble(-1, mov, -1, 28, 0, 0, -1, 1, triple_list[index_index[i-para_num+j]].arg1.imm_value);
				add_assemble(-1, stw, 27, 28, 0, 0, -1, 1, k);
				k += 4;
			}
		}
	}
	add_assemble(triple_list[index_index[fb->start->begin+1]].label, b, -1, -1, 0, 0, -1, 0, -1);
	return 0;
}

int call_code(func_block *fb, int i)
{
	if (check_tail_recursion(fb, i))
	{
		tail_recursion(fb, i);
		return 0;
	}
	int j, k, tmp, m, rtn_reg, idx, para_num, u;
	int tmp_reg_var[32];
	unsigned int *live = fb->live_status[i+1-fb->start->begin];
	idx = triple_list[index_index[i]].arg1.temp_index;
	for (j = CALLER_REG_START, k = 0; j <= CALLER_REG_END; j++)
	{
		if (idx != -1 && j > (triple_list[index_index[idx]].block)->fb->reg_used+CALLER_REG_START-1)
			break;
		u = fb->reg_var[j];
		if (u != -1 && (live[u/32] >> (31-u%32)) % 2 == 1)
		{
			if (fb->mapping[u].isTmp == 1)
			{
				k++;
				add_assemble(-1, stw, 29, j, 0, 0, -1, 1, -k*4);
			}
			else
			{
				if (!fb->uni_table[u]->isGlobal)
					add_assemble(-1, stw, 27, j, 0, 0, -1, 1, fb->uni_table[u]->offset);
				fb->reg_var[j] = -1;
			}
		}
		tmp_reg_var[j] = fb->reg_var[j];
	}
	for (para_num = 0; triple_list[index_index[i-1-para_num]].op == param; para_num++)
		;
	//printf("para_num : %d\n", para_num);
	if (para_num > 4)
		m = (k + para_num - 4) * 4;
	else
		m = k * 4;
	if (m != 0)
		add_assemble(-1, sub, 29, 29, 0, 0, -1, 1, m);
	for (j = 0, k = 0; j < para_num; j++)
	{
		if (triple_list[index_index[i-para_num+j]].arg1_uni != -1)
		{
			tmp = fb->reg_alloc[triple_list[index_index[i-para_num+j]].arg1_uni];
			tmp = load_operator(fb, triple_list[index_index[i-para_num+j]].arg1_uni, tmp, 28);
			if (j < 4)
				add_assemble(-1, mov, -1, j, 0, 0, -1, 0, tmp);
			else
			{
				add_assemble(-1, stw, 29, tmp, 0, 0, -1, 1, k);
				k += 4;
			}
		}
		else
		{
			if (j < 4)
				add_assemble(-1, mov, -1, j, 0, 0, -1, 1, triple_list[index_index[i-para_num+j]].arg1.imm_value);
			else
			{
				add_assemble(-1, mov, -1, 28, 0, 0, -1, 1, triple_list[index_index[i-para_num+j]].arg1.imm_value);
				add_assemble(-1, stw, 29, 28, 0, 0, -1, 1, k);
				k += 4;
			}
		}
	}
	add_special(triple_list[index_index[i]].arg2.var_name, b_l);
	if (m != 0)
		add_assemble(-1, add, 29, 29, 0, 0, -1, 1, m);
	for (j = CALLER_REG_START, k = 0; j <= CALLER_REG_END; j++)
	{
		if (idx != -1 && j > (triple_list[index_index[idx]].block)->fb->reg_used+CALLER_REG_START-1)
			break;
		fb->reg_var[j] = tmp_reg_var[j];
		u = fb->reg_var[j];
		if (u != -1 && fb->mapping[u].isTmp == 1 && (live[u/32] >> (31-u%32)) % 2 == 1)
		{
			k++;
			add_assemble(-1, ldw, 29, j, 0, 0, -1, 1, -k*4);
		}
	}
	tmp = triple_list[index_index[i]].tmp_uni;
	rtn_reg = fb->reg_alloc[tmp];
	if (check_live(fb, i, 1))
	{
		if (rtn_reg != -1)
		{
			fb->reg_var[rtn_reg] = tmp;
			add_assemble(-1, mov, -1, rtn_reg, 0, 0, -1, 0, 0);
		}
		else
			add_assemble(-1, stw, 27, 0, 0, 0, -1, 1, fb->uni_table[tmp]->offset);
	}
	return 0;
}

int param_code(func_block *fb, int i)
{
	return 0;
}

int enterF_code(func_block *fb, int i)
{
	int j;
	symtbl_hdr *ptr = triple_list[index_index[i]].symtbl;
	add_assemble(-1, stw, 29, 31, 0, 0, -1, 1, -4);
	add_assemble(-1, stw, 29, 30, 0, 0, -1, 1, -8);
	add_assemble(-1, stw, 29, 29, 0, 0, -1, 1, -12);
	add_assemble(-1, stw, 29, 27, 0, 0, -1, 1, -16);
	add_assemble(-1, sub, 29, 27, 0, 0, -1, 1, 4);
	add_assemble(-1, sub, 29, 29, 0, 0, -1, 1, fb->min_stack_size);
	for (j = 0; j < fb->reg_used-(CALLER_REG_END-CALLER_REG_START+1); j++)
	{
		add_assemble(-1, stw, 27, CALLEE_REG_START+j, 0, 0, -1, 1, -(16+j*4));
		temp_reg_var[CALLEE_REG_START+j] = fb->reg_var[CALLEE_REG_START+j];
	}
	for (j = 0; j < 4 && j < ptr->para_num; j++)
	{
		if (((fb->live_status[1][j/32] >> (31-j%32)) & 1) && fb->reg_alloc[j] != -1)
		{
			fb->reg_var[fb->reg_alloc[j]] = j;
			add_assemble(-1, mov, -1, fb->reg_alloc[j], 0, 0, -1, 0, j);
		}
		else
			add_assemble(-1, stw, 27, j, 0, 0, -1, 1, ptr->item[j].offset);
	}
	for (j = ptr->para_num; j < fb->uni_item_num; j++)
	{
		if (!fb->uni_table[j]->isGlobal && fb->uni_table[j]->size != -1)
		{
			add_assemble(-1, add, 27, 3, 0, 0, -1, 1, fb->uni_table[j]->offset+4);
			add_assemble(-1, stw, 27, 3, 0, 0, -1, 1, fb->uni_table[j]->offset);
		}
	}
	return 0;
}

int enterS_code(func_block *fb, int i)
{
	return 0;
}

int leaveF_code(func_block *fb, int i)
{
	int j;
	for (j = 0; j < fb->reg_used-(CALLER_REG_END-CALLER_REG_START+1); j++)
	{
		fb->reg_var[CALLEE_REG_START+j] = temp_reg_var[CALLEE_REG_START+j];
		add_assemble(-1, ldw, 27, CALLEE_REG_START+j, 0, 0, -1, 1, -(16+j*4));
	}
	add_assemble(-1, ldw, 27, 30, 0, 0, -1, 1, -4);
	add_assemble(-1, ldw, 27, 29, 0, 0, -1, 1, -8);
	add_assemble(-1, ldw, 27, 27, 0, 0, -1, 1, -12);
	add_assemble(-1, jump, -1, -1, 0, 0, -1, 0, 30);
	return 0;
}

int leaveS_code(func_block *fb, int i)
{
	return 0;
}

int rtn_code(func_block *fb, int i)
{
	int j;
	if (triple_list[index_index[i]].arg1_type < 2)
	{
		j = triple_list[index_index[i]].arg1_uni;
		if (j != -1)
		{
			if (fb->reg_alloc[j] != -1)
				add_assemble(-1, mov, -1, 0, 0, 0, -1, 0, fb->reg_alloc[j]);
			else
				add_assemble(-1, ldw, 27, 0, 0, 0, -1, 1, fb->uni_table[j]->offset);
		}
	}
	else
		add_assemble(-1, mov, -1, 0, 0, 0, -1, 1, triple_list[index_index[i]].arg1.imm_value);
	if (triple_list[index_index[i+1]].op != leaveF)
	{
		for (j = i+1; triple_list[index_index[j]].op != leaveF; j++)
			;
		add_assemble(triple_list[index_index[j]].label, b, -1, -1, 0, 0, -1, 0, -1);
	}
	return 0;
}

int array_shift_code(func_block *fb, int i)
{
	int u0, u1, u2, r0, r1, r2;
	enum instruction ins;
	if (check_live(fb, i, 1))
	{
		if (triple_list[index_index[i]].op == adds_op)
			ins = add;
		else
			ins = sub;
		u0 = triple_list[index_index[i]].tmp_uni;
		u1 = triple_list[index_index[i]].arg1_uni;
		u2 = triple_list[index_index[i]].arg2_uni;
		r0 = fb->reg_alloc[u0];
		r1 = fb->reg_alloc[u1];
		r1 = load_operator(fb, u1, r1, 1);
		if (u2 != -1)
		{
			r2 = fb->reg_alloc[u2];
			r2 = load_operator(fb, u2, r2, 2);
			store_result(fb, i, ins, u0, r1, r0, 0, 1, 2, 0, r2);
		}
		else
			store_result(fb, i, ins, u0, r1, r0, 0, 0, -1, 1, triple_list[index_index[i]].arg2.imm_value*4);
	}
	return 0;
}

int Imm_code(func_block *fb, int i)
{
	return 0;
}

int r_shift_code(func_block *fb, int i)
{
	int u0, u1, r0, r1;
	if (check_live(fb, i, 1))
	{
		u0 = triple_list[index_index[i]].tmp_uni;
		u1 = triple_list[index_index[i]].arg1_uni;
		r0 = fb->reg_alloc[u0];
		r1 = load_operator(fb, u1, fb->reg_alloc[u1], 1);
		store_result(fb, i, mov, u0, -1, r0, 1, 1, 2, 0, r1);
	}
	return 0;
}

int c_str_code(func_block *fb, int i)
{
	int j, off, u0, r0;
	CStrList *p;
	if (check_live(fb, i, 1))
	{
		j = search_const_str(triple_list[index_index[i]].arg1.var_name);
		for (p = fb->c_str_used, off = fb->const_off; p != NULL && p->c_str != j; p = p->next, off += 4)
			;
		u0 = triple_list[index_index[i]].tmp_uni;
		r0 = fb->reg_alloc[u0];
		if (r0 != -1)
			add_assemble(fb->global_label, ldw, -1, r0, 0, 0, -1, 1, off);
		else
		{
			r0 = 3;
			add_assemble(fb->global_label, ldw, -1, r0, 0, 0, -1, 1, off);
			add_assemble(-1, stw, 27, r0, 0, 0, -1, 1, fb->uni_table[u0]->offset);
		}
	}
	return 0;
}
