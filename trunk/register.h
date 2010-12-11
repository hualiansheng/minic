enum instruction {
	label, ldw=5000, stw, b_l, mov, add, sub, jump
};

int (*g[33])(func_block*, int) = {
	if_code, if_not_code, goto_code, negative_code, not_code, address_code, star_code, positive_code, assign_code, star_assign_code, add_code, 	minus_code, multiply_code, char_to_int_code, equal_code, less_code, larger_code, eqlarger_code, eqless_code, noteq_code, or_code, and_code, 	get_rb_code, set_rb_code, call_code, param_code, enterF_code, enterS_code, leaveF_code, leaveS_code, return_code, adds_code, int_to_char_code
};

typedef struct var_vertex {
	int n;
	struct var_vertex* con_vexs;
	int* adj_vexs;
	int deg;
}var_vertex;

typedef struct assemble {
	char* label;
	enum instruction ins;
	int Rn;
	int Rd;
	int Rs_or_Imm;
	int Rs_Imm;
	int Rm_or_Imm;
	int Rm_Imm;
}assemble;
