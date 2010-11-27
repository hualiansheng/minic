#include "AST.h"
#include "gen_intermediate_code.h"
#include "basic_block.h"
#include <stdio.h>
#include <memory.h>
char name[][30] = {"PROGRAM","EXTERNAL_DECLS","DECLARATION","FUNCTION_LIST","TYPE_NAME","VAR_LIST","VAR_ITEM","ARRAY_VAR","SCALAR_VAR","FUNCTION_DEF","FUNCTION_HDR","PARM_TYPE_LIST","PARM_LIST","PARM_DECL","FUNCTION_BODY","INTERNAL_DECLS","STATEMENT_LIST","STATEMENT","COMPOUNDSTMT","NULLSTMT","EXPRESSION_STMT","IFSTMT","FOR_STMT","WHILE_STMT","RETURN_STMT","EXPRESSION","ASSIGNMENT_EXPRESSION","LVALUE","RVALUE","OP","CONSTANT","ARGUMENT_LIST","EXTERN_T","REGISTER_T","VOID_T","INT_T","CHAR_T","IF_T","ELSE_T","FOR_T","WHILE_T","RETURN_T","BOOLEAN_OP_T","REL_OP_T","DOUBLE_OP_T","ICONSTANT_T","CHAR_CONSTANT_T","STRING_CONSTANT_T","SEMICOLON","COMMA","STAR","LEFT_SQUARE_BRACKET","RIGHT_SQUARE_BRACKET","LEFT_PARENTHESE","RIGHT_PARENTHESE","LEFT_BRACE","RIGHT_BRACE","EQUALITY_SIGN","MINUS_SIGN","PLUS_SIGN","MULTIPLY_SIGN","POSITIVE_SIGN","NEGATIVE_SIGN","NOT_SIGN","ADDRESS_SIGN","IDENT_T","EPSILON"};
// use this function to print the AST tree (with terminal/non-terminal names)
int counter[100];

extern triple* triple_list;
extern int triple_list_size;
extern int triple_list_index;
char *operator_name[]={"if,goto","if_not,goto","goto","-","not","bit_and","!*","+","=","*=","+","-","*","char to int","eq","lt","gt","geq","leq","neq","or","and","get rb","set rb","call", "param", "enter function", "enter statements", "leave function", "leave statements","return"};

extern basic_block *bblist;
extern func_block *fblist;
extern int block_num;

void print_AST_dot_core(AST_NODE* ptr);
void print_AST(AST_NODE* ptr, int level)
{
	int i;
	AST_NODE* p;
	if (ptr == NULL)
		return;
	for (i = 0; i < level; i++)
		printf("|    ");
	printf("%s", name[ptr->nodeType-1001]);
	
	// additional node information
	if( ptr->nodeType == COMPOUNDSTMT )
		printf(": scope_number=%d",ptr->content.i_content);
	// additional node information end
	printf("\n");
	for (p = ptr->leftChild; p != NULL; p = p->rightSibling)
		print_AST(p, level+1);


}

void print_AST_dot(AST_NODE* ptr)
{
	memset(counter,0,100*sizeof(int));
	printf("digraph G \{\n");
	print_AST_dot_core(ptr);
	printf("}\n");
}

void print_AST_dot_core(AST_NODE* ptr)
{
	AST_NODE *p;
	int temp = counter[ptr->nodeType-1001]++;
	if (ptr->leftChild == NULL)
	{
		printf("%s%d;\n", name[ptr->nodeType-1001],counter[ptr->nodeType-1001]);
		return;
	}
	for (p = ptr->leftChild; p != NULL; p = p->rightSibling)
	{
		printf("%s%d -> ", name[ptr->nodeType-1001], temp);
		print_AST_dot_core(p);
	}
	return;
}


void print_symtbl(symtbl_hdr* p)
{
	int i;
	symtbl_hdr* ptr;
	if (p == NULL)
		return;
	printf("=======================================\n");
	//printf("%s\n", name[p->nodeType-FUNC_OFFSET]);
	printf("ret_type: %s\n", name[p->ret_type-FUNC_OFFSET]);
	printf("ret_star: %d\n", p->ret_star);
	printf("para_num: %d\n", p->para_num);
	printf("func_def: %d\n", p->func_def);
	printf("item_num: %d\n", p->item_num);
	printf("maxSize: %d\n", p->maxSize);
	for (i = 0; i < p->item_num; i++)
		printf("%s  %d  %d  %s  %d\n", name[p->item[i].type-FUNC_OFFSET], p->item[i].star_num, p->item[i].writable, p->item[i].name, p->item[i].size);
	printf("\n");
	for (ptr = p->leftChild_tbl; ptr != NULL; ptr = ptr->rightSibling_tbl)
		print_symtbl(ptr);
}
void print_intermediate_arg(int arg_type,union arg a)
{
	switch(arg_type)
	{
		case 0:
			printf("%s ",a.var_name);
			break;
		case 1:
			printf("(%d) ",a.temp_index);
			break;
		case 2:
			printf("%d ",a.imm_value);
			break;
		case 3:
			printf("%s ",a.var_name);
	}
}
void print_intermediate_code()
{
	int i;
	for(i = 0 ; i < triple_list_index ; i ++)
	{	
		printf("(%d) : ", i);
		printf("%s ",operator_name[triple_list[i].op - 3000]);
		//print_intermediate_arg(triple_list[i].arg1_type,triple_list[i].arg1);
		//print_intermediate_arg(triple_list[i].arg2_type,triple_list[i].arg2);
		if(triple_list[i].arg1_type == 0 || triple_list[i].arg1_type == 3) printf("%s ", triple_list[i].arg1.var_name);
		else	if (triple_list[i].arg1_type == 1) printf("(%d) ", triple_list[i].arg1.temp_index);
			else	printf("%d ", triple_list[i].arg1.temp_index);
		if(triple_list[i].arg2_type == 0 || triple_list[i].arg2_type == 3) printf("%s ", triple_list[i].arg2.var_name);
		else	if (triple_list[i].arg2_type == 1) printf("(%d) ", triple_list[i].arg2.temp_index);
			else	printf("%d ", triple_list[i].arg2.temp_index);
		printf("\t%d %d %d", triple_list[i].result_type, triple_list[i].arg1_type, triple_list[i].arg2_type);
		printf("\t symtbl_dbg: %d \n", triple_list[i].symtbl->item_num);
		printf("\n");
	}
}

void print_basic_block()
{
	int i, j;
	basic_block *bb;
	printf("digraph G {\nnode[shape=record];\n");
	for (i = 0, bb = bblist; i < block_num; i++, bb = bb->next)
	{
		printf("Block%d[label = \"{", i);
		for (j = bb->begin; j <= bb->end; j++)
		{
			printf("(%d) : ", j);
			printf("%s ",operator_name[triple_list[j].op - 3000]);
			if(triple_list[j].arg1_type == 0 || triple_list[j].arg1_type == 3) printf("%s ", triple_list[j].arg1.var_name);
			else	if (triple_list[j].arg1_type == 1) printf("(%d) ", triple_list[j].arg1.temp_index);
				else	printf("%d ", triple_list[j].arg1.temp_index);
			if(triple_list[j].arg2_type == 0 || triple_list[j].arg2_type == 3) printf("%s ", triple_list[j].arg2.var_name);
			else	if (triple_list[j].arg2_type == 1) printf("(%d) ", triple_list[j].arg2.temp_index);
				else	printf("%d ", triple_list[j].arg2.temp_index);
			if (j != bb->end) printf("| ");
		}
		printf("}\"];\n");
	}
	for (i = 0, bb = bblist; i < block_num; i++, bb = bb->next)
	{
		if (bb->follow != NULL)
			printf("Block%d -> Block%d;\n", i, i + (bb->follow->m - bb->m));
		if (bb->jump != NULL)
			printf("Block%d -> Block%d;\n", i, i + (bb->jump->m - bb->m));
	}
	printf("}\n");
}

void print_live_var()
{
	FILE *out = fopen("live_var.debug", "w");
	func_block *fb;
	int i, j;
	for (fb = fblist; fb != NULL; fb = fb->next)
	{
		for (i = 0; i < fb->uni_item_num; i++)
		{
			if (fb->mapping[i].isTmp == 1)
				fprintf(out, "(%d)\t", fb->mapping[i].tmp_k);
			else
				fprintf(out, "%s\t", fb->mapping[i].var_name);
		}
		fprintf(out, "\n");
		for (i = fb->start->begin; i <= fb->over->end; i++)
		{
			for (j = 0; j < fb->uni_item_num; j++)
				fprintf(out, "%d\t", (fb->live_status[i-fb->start->begin][j/32] << j%32) >> 31);
			fprintf(out, "(%d) : ", i);
			fprintf(out, "%s ",operator_name[triple_list[i].op - 3000]);
			if(triple_list[i].arg1_type == 0 || triple_list[i].arg1_type == 3) 
				fprintf(out, "%s ", triple_list[i].arg1.var_name);
			else	if (triple_list[i].arg1_type == 1)
					fprintf(out, "(%d) ", triple_list[i].arg1.temp_index);
				else	fprintf(out, "%d ", triple_list[i].arg1.temp_index);
			if(triple_list[i].arg2_type == 0 || triple_list[i].arg2_type == 3) 
				fprintf(out, "%s ", triple_list[i].arg2.var_name);
			else	if (triple_list[i].arg2_type == 1) 
					fprintf(out, "(%d) ", triple_list[i].arg2.temp_index);
				else	fprintf(out, "%d ", triple_list[i].arg2.temp_index);
			fprintf(out, "\n");
		}
		fprintf(out, "---------------------------------------------------------------------------------------------------\n");
	}
	fclose(out);
}
