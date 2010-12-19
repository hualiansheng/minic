#ifndef REGISTER_H_
#define REGISTER_H_
enum instruction {
	special = 5000, label, ldw, stw, ldb, stb, b_l, mov, add, sub, rsub, mul, jump, cmpsub_a, bne, beq, bsl, beg, bsg, bel, b, cmoveq, cmovsl, cmoveg, cmovel, cmovne, cmovsg
};



typedef struct var_vertex {
	int n;
	struct var_vertex* con_vexs;
	int* adj_vexs;
	int deg;
}var_vertex;

typedef struct assemble {
	char* content;
	int label;
	enum instruction ins;
	int Rn;
	int Rd;
	int shift_direct;
	int Rs_or_Imm;
	int Rs_Imm;
	int Rm_or_Imm;
	int Rm_Imm;
}assemble;

#endif
