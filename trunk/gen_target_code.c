#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "basic_block.h"
#include "register.h"
#include "gen_intermediate_code.h"
extern triple *triple_list;
extern int *index_index;
extern int triple_list_index;
extern func_block *fblist;
FILE *out;
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
int char_to_int_code(func_block *fb, int i);
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
int adds_code(func_block *fb, int i);
int int_to_char_code(func_block *fb, int i);
int (*g[33])(func_block*, int) = {
	if_goto_code, if_goto_code, goto_code, negative_code, not_code, address_code, star_code, positive_code, assign_code, star_assign_code, add_code, 	minus_code, multiply_code, char_to_int_code, equal_code, less_code, larger_code, eqlarger_code, eqless_code, noteq_code, or_code, and_code, 	get_rb_code, set_rb_code, call_code, param_code, enterF_code, enterS_code, leaveF_code, leaveS_code, rtn_code, adds_code, int_to_char_code
};
int initial();
int memory_allocation();
int setLabel();
int convert();
int add_assemble(char *func_name, int label, enum instruction ins, int Rn, int Rd, int Rs_or_Imm, int Rs_Imm, int Rm_or_Imm, int Rm_Imm);
int check_live(func_block *fb, int i, int isTmp);
int load_operator(func_block *fb, int u, int r, int _r);
int add_std_assemble(func_block *fb, enum instruction ins, int u0, int u1, int u2);
int add_imm_assemble(func_block *fb, enum instruction ins, int u0, int u1, int imm);
int check_bool_use(func_block *fb, int i);
int assign_bool_value(enum instruction ins, func_block *fb, int i);
int compare_operation(func_block *fb, int i, int cond, int m);
int search_label(func_block *fb, int i, int m, int not_flag);
enum instruction map_ins[6][2] = {{bsl, beg}, {bel, bsg}, {beq, bne}, {bne, beq}, {beg, bsl}, {bsg, bel}};

int gen_target_code()
{
	out = fopen("assemble_code.s", "w");
	initial();
	memory_allocation();
	setLabel();
	convert();
	fclose(out);
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
		for (i = para_num-5, off = 4; i >= 0; i--, off += 4)
			fb->uni_table[i]->offset = off;
		if (fb->reg_used < 10)
			off = -16;
		else
			off = -16 - (fb->reg_used - 9) * 4;
		for (i = (para_num<4)?0:para_num-4; i < fb->uni_item_num; i++)
		{
			if (!(fb->mapping[i].isTmp == 1 && fb->reg_alloc[i] != -1))
			{
				fb->uni_table[i]->offset = off;
				if (fb->uni_table[i]->size == -1)
					off -= 4;
				else
					off -= 4*fb->uni_table[i]->size;
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
		triple_list[index_index[i]].label = 0;
	for (i = 0, l = 1; i < triple_list_index; i++)
	{
		if (triple_list[index_index[i]].op == goto_op)
		{
			j = triple_list[index_index[i]].arg1.temp_index;
			if (triple_list[index_index[j]].label == 0)		
				triple_list[index_index[j]].label = l++;
		}
		if (triple_list[index_index[i]].op == if_op || triple_list[index_index[i]].op == if_not_op)
		{
			if (!(triple_list[index_index[i+1]].op == set_rb && triple_list[index_index[i+2]].op == get_rb))
			{
				if (triple_list[index_index[i+1]].label == 0)		
					triple_list[index_index[i+1]].label = l++;
			}
			j = triple_list[index_index[i]].arg2.temp_index;
			if (triple_list[index_index[j]].op != get_rb && triple_list[index_index[j]].label == 0)		
				triple_list[index_index[j]].label = l++;
		}
	}
	return 0;
}

int convert()
{
	func_block *fb;
	int i;
	assemble_list = (assemble*)malloc(64*sizeof(assemble));
	assemble_size = 64*sizeof(assemble);
	assemble_num = 0;
	for (fb = fblist; fb != NULL; fb = fb->next)
	{
		add_assemble((triple_list[index_index[fb->start->begin]].symtbl)->func_name, -1, func, -1, -1, 0, -1, 0, -1);
		//fprintf(out, "%s:\n", (triple_list[index_index[fb->start->begin]].symtbl)->func_name);
		for (i = fb->start->begin; i <= fb->over->end; i++)
		{
			if (triple_list[index_index[i]].label != 0)
				add_assemble(NULL, triple_list[index_index[i]].label, label, -1, -1, 0, -1, 0, -1);
			g[triple_list[index_index[i]].op-3000](fb, i);
		}
	}
	return 0;
}

int add_assemble(char *func_name, int label, enum instruction ins, int Rn, int Rd, int Rs_or_Imm, int Rs_Imm, int Rm_or_Imm, int Rm_Imm)
{
	if ((assemble_num+1)*sizeof(assemble) > assemble_size)
		adjustSize((void**)&assemble_list, &assemble_size);
	assemble_list[assemble_num].func_name = func_name;	
	assemble_list[assemble_num].label = label;
	assemble_list[assemble_num].ins = ins;
	assemble_list[assemble_num].Rn = Rn;
	assemble_list[assemble_num].Rd = Rd;
	assemble_list[assemble_num].Rs_or_Imm = Rs_or_Imm;
	assemble_list[assemble_num].Rs_Imm = Rs_Imm;
	assemble_list[assemble_num].Rm_or_Imm = Rm_or_Imm;
	assemble_list[assemble_num].Rm_Imm = Rm_Imm;
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
		add_assemble(NULL, -1, ldw, 27, r, 0, -1, 1, fb->uni_table[u]->offset);
	}
	else
	{
		if (fb->reg_var[r] != u)
		{
			fb->reg_var[r] = u;
			add_assemble(NULL, -1, ldw, 27, r, 0, -1, 1, fb->uni_table[u]->offset);
		}
	}
	return r;
}

int store_result(func_block *fb, enum instruction ins, int u, int Rs, int Rd, int Rs_or_Imm, int Rs_Imm, int Rm_or_Imm, int Rm_Imm)
{
	if (Rd == -1)
	{
		Rd = 3;
		add_assemble(NULL, -1, ins, Rs, Rd, Rs_or_Imm, Rs_Imm, Rm_or_Imm, Rm_Imm);
		add_assemble(NULL, -1, stw, 27, Rd, 0, -1, 1, fb->uni_table[u]->offset);
	}
	else
	{
		fb->reg_var[Rd] = u;
		add_assemble(NULL, -1, ins, Rs, Rd, Rs_or_Imm, Rs_Imm, Rm_or_Imm, Rm_Imm);
	}
	return 0;
}

int add_std_assemble(func_block *fb, enum instruction ins, int u0, int u1, int u2)
{
	int r0, r1, r2;
	r0 = fb->reg_alloc[u0];
	r1 = fb->reg_alloc[u1];
	r2 = fb->reg_alloc[u2];
	r1 = load_operator(fb, u1, r1, 1);
	r2 = load_operator(fb, u2, r2, 2);
	store_result(fb, ins, u0, r1, r0, 0, -1, 0, r2);
	return 0;
}

int add_imm_assemble(func_block *fb, enum instruction ins, int u0, int u1, int imm)
{
	int r0, r1;
	r0 = fb->reg_alloc[u0];
	r1 = fb->reg_alloc[u1];
	r1 = load_operator(fb, u1, r1, 1);
	store_result(fb, ins, u0, r1, r0, 0, -1, 1, imm);
	return 0;
}

int check_bool_use(func_block *fb, int i)
{
	int j, base, l, u1, u2;
	enum operator op;
	base = fb->start->begin;
	l = triple_list[index_index[i]].tmp_uni;
	for (j = i+1-base; j < fb->code_num; j++)
	{
		if ((fb->live_status[j][l/32] >> (31-l%32)) % 2 != 1)
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
	int u0, u1, u2, r0, r1, r2;
	u0 = triple_list[index_index[i]].tmp_uni;
	u1 = triple_list[index_index[i]].arg1_uni;
	u2 = triple_list[index_index[i]].arg2_uni;
	r0 = fb->reg_alloc[u0];
	r1 = fb->reg_alloc[u1];
	r2 = fb->reg_alloc[u2];
	r1 = load_operator(fb, u1, r1, 1);
	r2 = load_operator(fb, u2, r2, 2);
	if (r0 == -1)
	{
		add_assemble(NULL, -1, mov, -1, 3, 0, -1, 1, 0);
		add_assemble(NULL, -1, cmpsub_a, r1, -1, 0, -1, 0, r2);
		add_assemble(NULL, -1, ins, -1, 3, 0, -1, 1, 1);
		add_assemble(NULL, -1, stw, 27, 3, 0, -1, 1, fb->uni_table[u0]->offset);
	}
	else
	{
		fb->reg_var[r0] = u0;
		add_assemble(NULL, -1, mov, -1, r0, 0, -1, 1, 0);
		add_assemble(NULL, -1, cmpsub_a, r1, -1, 0, -1, 0, r2);
		add_assemble(NULL, -1, ins, -1, r0, 0, -1, 1, 1);
	}
	return 0;
}

int compare_operation(func_block *fb, int i, int cond, int m)
{
	int u1, u2, r1, r2, t1, t2;
	if (cond == 1)
	{
		u1 = triple_list[index_index[i]].arg1_uni;
		r1 = fb->reg_alloc[u1];
		r1 = load_operator(fb, u1, r1, 1);
		add_assemble(NULL, -1, cmpsub_a, r1, -1, 0, -1, 1, 0);			
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
			add_assemble(NULL, -1, cmpsub_a, r1, -1, 0, -1, 0, r2);
		}
		else
		{
			if (t1 < 2)
			{
				r1 = fb->reg_alloc[u1];
				r1 = load_operator(fb, u1, r1, 1);
				add_assemble(NULL, -1, cmpsub_a, r1, -1, 0, -1, 1, triple_list[index_index[i]].arg2.imm_value);
			}
			if (t2 < 2)
			{
				r2 = fb->reg_alloc[u2];
				r2 = load_operator(fb, u2, r2, 1);
				add_assemble(NULL, -1, cmpsub_a, r2, -1, 0, -1, 1, triple_list[index_index[i]].arg1.imm_value);
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
		add_assemble(NULL, -1, cmpsub_a, r1, -1, 0, -1, 1, 0);			
	}
	return m;
}

int search_label(func_block *fb, int i, int m, int not_flag)
{
	enum operator op;
	int j, flag;
	op = triple_list[index_index[i]].op;
	j = i;
	flag = 0;
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
			j++;
		}
	}
	if (triple_list[index_index[i]].op == if_op)
		add_assemble(NULL, triple_list[index_index[j]].label, map_ins[m][1-flag], -1, -1, 0, -1, 0, -1);
	else
		add_assemble(NULL, triple_list[index_index[j]].label, map_ins[m][flag], -1, -1, 0, -1, 0, -1);
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
	add_assemble(NULL, lb, b, -1, -1, 0, -1, 0, -1);
	return 0;
}

int negative_code(func_block *fb, int i)
{
	return 0;
}

int not_code(func_block *fb, int i)
{
	/*int u0, u1, r0, r1;
	if (check_bool_use(fb, i))
	{
		u0 = triple_list[index_index[i]].tmp_uni;
		u1 = triple_list[index_index[i]].arg1_uni;
		r0 = fb->reg_alloc[u0];
		r1 = fb->reg_alloc[u1];
		r1 = load_operator(fb, u1, r1, 1);
		if (r0 == -1)
		{
			add_assemble(NULL, -1, mov, -1, 3, 0, -1, 1, 0);
			add_assemble(NULL, -1, cmpsub_a, r1, -1, 0, -1, 1, 0);
			add_assemble(NULL, -1, cmoveq, -1, 3, 0, -1, 1, 1);
			add_assemble(NULL, -1, stw, 27, 3, 0, -1, 1, fb->uni_table[u0]->offset);
		}
		else
		{
			fb->reg_var[r0] = u0;
			add_assemble(NULL, -1, mov, -1, r0, 0, -1, 1, 0);
			add_assemble(NULL, -1, cmpsub_a, r1, -1, 0, -1, 1, 0);
			add_assemble(NULL, -1, cmoveq, -1, r0, 0, -1, 1, 1);
		}
	}*/
	return 0;
}

int address_code(func_block *fb, int i)
{
	return 0;
}

int star_code(func_block *fb, int i)
{
	return 0;
}

int positive_code(func_block *fb, int i)
{
	return 0;
}

int assign_code(func_block *fb, int i)
{
	int base = fb->start->begin;
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
		if ((fb->live_status[i+1-base][u1/32] >> (31-u1%32)) % 2 == 1 && r1 != r2)
			store_result(fb, mov, u1, -1, r1, 0, -1, 0, r2);
		if ((fb->live_status[i+1-base][u0/32] >> (31-u0%32)) % 2 == 1 && r0 != r2)
			store_result(fb, mov, u0, -1, r0, 0, -1, 0, r2);
	}
	else
	{
		if ((fb->live_status[i+1-base][u1/32] >> (31-u1%32)) % 2 == 1)
			store_result(fb, mov, u1, -1, r1, 0, -1, 1, triple_list[index_index[i]].arg2.imm_value);
		if ((fb->live_status[i+1-base][u0/32] >> (31-u0%32)) % 2 == 1)
			store_result(fb, mov, u0, -1, r0, 0, -1, 1, triple_list[index_index[i]].arg2.imm_value);
	}
	return 0;
}

int star_assign_code(func_block *fb, int i)
{
	return 0;
}

int add_code(func_block *fb, int i)
{
	int type1, type2, tmp_uni, arg1_uni, arg2_uni;
	if (check_live(fb, i, 1))
	{
		type1 = triple_list[index_index[i]].arg1_type;
		type2 = triple_list[index_index[i]].arg2_type;
		tmp_uni = triple_list[index_index[i]].tmp_uni;
		arg1_uni = triple_list[index_index[i]].arg1_uni;
		arg2_uni = triple_list[index_index[i]].arg2_uni;
		if (type1 < 2 && type2 < 2)
			add_std_assemble(fb, add, tmp_uni, arg1_uni, arg2_uni);
		else
		{
			if (type1 < 2)
				add_imm_assemble(fb, add, tmp_uni, arg1_uni, triple_list[index_index[i]].arg2.imm_value);
			if (type2 < 2)
				add_imm_assemble(fb, add, tmp_uni, arg2_uni, triple_list[index_index[i]].arg1.imm_value);
		}
	}
	return 0;
}

int minus_code(func_block *fb, int i)
{
	int type1, type2, tmp_uni, arg1_uni, arg2_uni;
	if (check_live(fb, i, 1))
	{
		type1 = triple_list[index_index[i]].arg1_type;
		type2 = triple_list[index_index[i]].arg2_type;
		tmp_uni = triple_list[index_index[i]].tmp_uni;
		arg1_uni = triple_list[index_index[i]].arg1_uni;
		arg2_uni = triple_list[index_index[i]].arg2_uni;
		if (type1 < 2 && type2 < 2)
			add_std_assemble(fb, sub, tmp_uni, arg1_uni, arg2_uni);
		else
		{
			if (type1 < 2)
				add_imm_assemble(fb, sub, tmp_uni, arg1_uni, triple_list[index_index[i]].arg2.imm_value);
			if (type2 < 2)
				add_imm_assemble(fb, rsub, tmp_uni, arg2_uni, triple_list[index_index[i]].arg1.imm_value);
		}
	}
	return 0;
}

int multiply_code(func_block *fb, int i)
{
	return 0;
}

int char_to_int_code(func_block *fb, int i)
{
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
	/*int u0, r0;
	u0 = triple_list[index_index[i]].tmp_uni;
	r0 = fb->reg_alloc[u0];
	if (r0 == -1)
		add_assemble(NULL, -1, stw, 27, 3, 0, -1, 1, fb->uni_table[u0]->offset);
	else
	{
		fb->reg_var[r0] = u0;
		add_assemble(NULL, -1, mov, -1, r0, 0, -1, 0, 3);
	}*/
	return 0;
}

int set_rb_code(func_block *fb, int i)
{
	return 0;
}

int call_code(func_block *fb, int i)
{
	int j, k, tmp, m, rtn_reg;
	int tmp_reg_var[32];
	symtbl_hdr *ptr = triple_list[index_index[triple_list[index_index[i]].arg1.temp_index]].symtbl;
	func_block *q = (triple_list[index_index[triple_list[index_index[i]].arg1.temp_index]].block)->fb;
	unsigned int *live = fb->live_status[i-fb->start->begin];
	for (j = 7, k = 0; j <= 15 && j <= q->reg_used+3; j++)
	{
		if (fb->reg_var[j] != -1 && (live[fb->reg_var[j]/32] >> (31-fb->reg_var[j]%32)) % 2 == 1)
		{
			k++;
			add_assemble(NULL, -1, stw, 29, j, 0, -1, 1, -k*4);
		}
		tmp_reg_var[j] = fb->reg_var[j];
	}
	if (ptr->para_num > 4)
		m = (k + ptr->para_num - 4) * 4;
	else
		m = k * 4;
	if (m != 0)
		add_assemble(NULL, -1, sub, 29, 29, 0, -1, 1, m);
	for (j = ptr->para_num-1, k = 0; j >= 0; j--)
	{
		if (triple_list[index_index[i-ptr->para_num+j]].arg1_uni != -1)
		{
			tmp = fb->reg_alloc[triple_list[index_index[i-ptr->para_num+j]].arg1_uni];
			tmp = load_operator(fb, triple_list[index_index[i-ptr->para_num+j]].arg1_uni, tmp, 2);
			if (j >= ptr->para_num-4)
				add_assemble(NULL, -1, mov, -1, ptr->para_num-1-j, 0, -1, 0, tmp);
			else
			{
				add_assemble(NULL, -1, stw, 29, tmp, 0, -1, 1, k);
				k += 4;
			}
		}
		else
		{
			if (j >= ptr->para_num-4)
				add_assemble(NULL, -1, mov, -1, ptr->para_num-1-j, 0, -1, 1, triple_list[index_index[i-ptr->para_num+j]].arg1.imm_value);
			else
			{
				add_assemble(NULL, -1, mov, -1, 3, 0, -1, 1, triple_list[index_index[i-ptr->para_num+j]].arg1.imm_value);
				add_assemble(NULL, -1, stw, 29, 3, 0, -1, 1, k);
				k += 4;
			}
		}
	}
	add_assemble(ptr->func_name, -1, b_l, -1, -1, 0, -1, 0, -1);
	if (m != 0)
		add_assemble(NULL, -1, add, 29, 29, 0, -1, 1, m);
	for (j = 7, k = 0; j <= 15 && j <= q->reg_used+3; j++)
	{
		fb->reg_var[j] = tmp_reg_var[j];
		if (fb->reg_var[j] != -1 && (live[fb->reg_var[j]/32] >> (31-fb->reg_var[j]%32)) % 2 == 1)
		{
			k++;
			add_assemble(NULL, -1, ldw, 29, j, 0, -1, 1, -k*4);
		}
	}
	tmp = triple_list[index_index[i]].tmp_uni;
	rtn_reg = fb->reg_alloc[tmp];
	if ((fb->live_status[i+1-fb->start->begin][tmp/32] >> (31-tmp%32)) % 2 == 1)
	{
		if (rtn_reg != -1)
		{
			fb->reg_var[rtn_reg] = tmp;
			add_assemble(NULL, -1, mov, -1, rtn_reg, 0, -1, 0, 0);
		}
		else
			add_assemble(NULL, -1, stw, 27, 0, 0, -1, 1, fb->uni_table[tmp]->offset);
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
	add_assemble(NULL, -1, stw, 29, 31, 0, -1, 1, -4);
	add_assemble(NULL, -1, stw, 29, 30, 0, -1, 1, -8);
	add_assemble(NULL, -1, stw, 29, 29, 0, -1, 1, -12);
	add_assemble(NULL, -1, stw, 29, 27, 0, -1, 1, -16);
	add_assemble(NULL, -1, sub, 29, 27, 0, -1, 1, 4);
	add_assemble(NULL, -1, sub, 29, 29, 0, -1, 1, fb->min_stack_size);
	for (j = 0; j < fb->reg_used-12; j++)
	{
		add_assemble(NULL, -1, stw, 27, 17+j, 0, -1, 1, -(16+j*4));
		temp_reg_var[17+j] = fb->reg_var[17+j];
	}
	for (j = 0; j < 4 && j < ptr->para_num; j++)
		add_assemble(NULL, -1, stw, 27, j, 0, -1, 1, ptr->item[ptr->para_num-1-j].offset);
	return 0;
}

int enterS_code(func_block *fb, int i)
{
	return 0;
}

int leaveF_code(func_block *fb, int i)
{
//	int j;
//	//symtbl_hdr *ptr = triple_list[index_index[i]].symtbl;
//	for (j = 0; j < fb->reg_used-12; j++)
//	{
//		fb->reg_var[17+j] = temp_reg_var[17+j];
//		add_assemble(NULL, -1, ldw, 27, 17+j, 0, -1, 1, -(16+j*4));
//	}
//	add_assemble(NULL, -1, ldw, 27, 30, 0, -1, 1, -4);
//	add_assemble(NULL, -1, ldw, 27, 29, 0, -1, 1, -8);
//	add_assemble(NULL, -1, ldw, 27, 27, 0, -1, 1, -12);
//	add_assemble(NULL, -1, jump, -1, -1, 0, -1, 0, 30);
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
				add_assemble(NULL, -1, mov, -1, 0, 0, -1, 0, fb->reg_alloc[j]);
			else
				add_assemble(NULL, -1, ldw, 27, 0, 0, -1, 1, fb->uni_table[j]->offset);
		}
	}
	else
		add_assemble(NULL, -1, mov, -1, 0, 0, -1, 1, triple_list[index_index[i]].arg1.imm_value);
	for (j = 0; j < fb->reg_used-12; j++)
	{
		fb->reg_var[17+j] = temp_reg_var[17+j];
		add_assemble(NULL, -1, ldw, 27, 17+j, 0, -1, 1, -(16+j*4));
	}
	add_assemble(NULL, -1, ldw, 27, 30, 0, -1, 1, -4);
	add_assemble(NULL, -1, ldw, 27, 29, 0, -1, 1, -8);
	add_assemble(NULL, -1, ldw, 27, 27, 0, -1, 1, -12);
	add_assemble(NULL, -1, jump, -1, -1, 0, -1, 0, 30);
	return 0;
}

int adds_code(func_block *fb, int i)
{
	return 0;
}

int int_to_char_code(func_block *fb, int i)
{
	return 0;
}

