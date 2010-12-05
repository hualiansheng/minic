#include "gen_intermediate_code.h"
#include "symtbl.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
extern char name[][30];
int adjustSize(void** p_old, int* max);

int rb;//全局变量，布尔表达式的专用寄存器
const int initsize = 100;
extern AST_NODE* tree_root;

int const_value;
char* const_string;
char* temp_ID;
//brills modified here:
int *alist1,*alist2;
int (*gen_triple_code[67])(AST_NODE*);

symtbl_hdr **scope_stack;
stack_item *stack;//记录statement中的++ --，statement完了之后弹栈到空
triple *triple_list;
int *index_index;//

int stack_top=0;
int stack_size;
int triple_list_size;
int triple_list_index=0;
int scope_top=0;
int scope_size;

void gen_intermediate_code(AST_NODE *root);
int statement_code(AST_NODE *p);
void resume_doubleop(AST_NODE *p);
void gen_code_initial()
{
	int i;
	for(i = 0 ; i < 67 ;i++){
		gen_triple_code[i] = null_code;
	}
	gen_triple_code[STATEMENT-FUNC_OFFSET] = statement_code;
	gen_triple_code[IFSTMT-FUNC_OFFSET] = if_code;
	gen_triple_code[COMPOUNDSTMT-FUNC_OFFSET] = compound_code;
	gen_triple_code[NULLSTMT-FUNC_OFFSET] = null_code;
	gen_triple_code[FOR_STMT-FUNC_OFFSET] = for_code;
	gen_triple_code[WHILE_STMT-FUNC_OFFSET] = while_code;
	gen_triple_code[EXPRESSION_STMT-FUNC_OFFSET] = expression_code;
	gen_triple_code[EXPRESSION-FUNC_OFFSET] = expression_code;
	gen_triple_code[RVALUE-FUNC_OFFSET] = rvalue_code;
	gen_triple_code[LVALUE-FUNC_OFFSET] = lvalue_code;
	gen_triple_code[ASSIGNMENT_EXPRESSION-FUNC_OFFSET] = assignment_expression_code;
	gen_triple_code[RETURN_STMT-FUNC_OFFSET] = return_code;	
}
//scope stack
void push_scope(symtbl_hdr *scope)
{
	int i;
	//printf("push_scope_dbg: %d\n", scope_size);
	if(scope_top == scope_size){
		i = scope_size * sizeof(symtbl_hdr*);
		adjustSize((void**)&scope_stack, &i);
		scope_size = i /sizeof(symtbl_hdr*);
	}
	scope_stack[scope_top++] = scope;
	//add_triple(enter, -1, -1, -1, -1, -1);
}
void pop_scope()
{
	//add_triple(leave,-1,-1,-1,-1,-1);
	scope_top--;
}

//++ -- stack
void push(stack_item a)
{
	int i;
	if(stack_top == stack_size){
		i = stack_size * sizeof(stack_item);
		adjustSize((void**)&stack, &i);
		stack_size = i / sizeof(stack_item);
	}
	stack[stack_top++] = a;	
}
stack_item pop()
{
	return stack[--stack_top];
}

//add a triple item
void add_triple(enum operator op, int arg1, int arg2, int result_type, int arg1_type, int arg2_type)
{
	int i;
	if(triple_list_index == triple_list_size){
		i = triple_list_size * sizeof(triple);
		adjustSize((void**)&triple_list, &i);
		triple_list_size = i/sizeof(triple);
		
		i = triple_list_size * sizeof(int);
		adjustSize((void**)&index_index,&i);
	}
	triple_list[triple_list_index].op = op;
	triple_list[triple_list_index].arg1 = (union arg)arg1;
	triple_list[triple_list_index].arg2 = (union arg)arg2;
	triple_list[triple_list_index].result_type = result_type;
	triple_list[triple_list_index].arg1_type = arg1_type;
	triple_list[triple_list_index].arg2_type = arg2_type;
	//Brills modified here:
	//Bind triple with symtbl
	triple_list[triple_list_index].symtbl = scope_stack[scope_top - 1];
	index_index[triple_list_index] = triple_list_index;
	triple_list_index ++;
}

void initialize()
{
	stack = malloc(initsize*sizeof(stack_item));
	stack_size = initsize;
	triple_list = malloc(initsize*sizeof(triple));
	index_index = malloc(initsize*sizeof(int));	
	triple_list_size = initsize;
	scope_stack = malloc(initsize*sizeof(symtbl_hdr*));
	scope_size = initsize;
	//temporary workround
	alist1 = malloc(100*sizeof(int));
	alist2 = malloc(100*sizeof(int));
}

void intermediate_code(AST_NODE* root)
{
	gen_code_initial();
	initialize();
	gen_intermediate_code(root);
}
void gen_intermediate_code(AST_NODE *root)
{
	AST_NODE* p = root;
	AST_NODE* ptr;
	symtbl_item* temp_symtbl;
	//fprintf(stderr,"processing: %d %s\n",root->nodeType,name[root->nodeType-FUNC_OFFSET]);
	//initialize();
	if(root -> nodeType == FUNCTION_HDR){
		ptr = root->leftChild->rightSibling;
		if (ptr->nodeType == STAR)
			ptr = ptr->rightSibling;
		temp_symtbl = symtbl_query(ptr->symtbl, ptr->content.s_content, 0);
		temp_symtbl->addr_off = triple_list_index;
	}
	if(p->nodeType == STATEMENT){
		//do something
		gen_triple_code[p->nodeType-FUNC_OFFSET](p);		
		return;
	}
	if(root->nodeType == FUNCTION_BODY){
		push_scope(root->symtbl);
		add_triple(enterF, -1, -1, -1, -1, -1);		
	}
	for(p = p->leftChild ; p != NULL; p = p->rightSibling)
		gen_intermediate_code(p);
	if(root->nodeType == FUNCTION_BODY){
		add_triple(leaveF, -1, -1, -1, -1, -1);
		pop_scope();
	}

}


int statement_code(AST_NODE *p)
{
	gen_triple_code[p->leftChild->nodeType-FUNC_OFFSET](p->leftChild);
	if(p->leftChild->nodeType == IFSTMT || p->leftChild->nodeType == FOR_STMT || p->leftChild->nodeType == WHILE_STMT) ;
	else resume_doubleop(p);
	return triple_list_index - 1;	
}

int if_code(AST_NODE *p)
{
	AST_NODE *ptr;
	int childNum = 0;
	int exp;
	int backpatch;
	int last;
	for(ptr = p->leftChild ; ptr!=NULL ; ptr = ptr->rightSibling)
		childNum++;
	if(childNum == 5)//if without else
	{
		ptr = p->leftChild->rightSibling->rightSibling;
		exp =gen_triple_code[ptr->nodeType-FUNC_OFFSET](ptr);
		/** Brills modified here:
		 * argument 2 of if-goto should be an index.
		 */
		switch(exp){
		case -1:add_triple(if_op, (int)temp_ID, triple_list_index + 2, 0, 0, 1);break;
		case -2:add_triple(if_op, const_value, triple_list_index + 2, 0, 2, 1);break;
		case -3:add_triple(if_op, const_value, triple_list_index + 2, 0, 2, 1);break;
		case -4:add_triple(if_op, (int)const_string, triple_list_index + 2, 0, 3, 1);break;
		default: add_triple(if_op, exp, triple_list_index + 2, 0, 1, 1);
		}
		backpatch = triple_list_index;
		add_triple(goto_op, -1, -1, 0, 1, -1); //goto 第二个操作数是一个整型常数
		resume_doubleop(p);
		ptr = ptr->rightSibling->rightSibling;
		gen_triple_code[ptr->nodeType-FUNC_OFFSET](ptr);
		last = triple_list_index - 1;
		triple_list[backpatch].arg1 = (union arg)(last + 1);
		return triple_list_index - 1;
	}
	if(childNum == 7)
	{
		ptr = p->leftChild->rightSibling->rightSibling;
		exp =gen_triple_code[ptr->nodeType-FUNC_OFFSET](ptr);
		switch(exp){
			/**
			 * Brills modified here:
			 * the argument 2 of if-goto should be an index.
			 */
		case -1:add_triple(if_op, (int)temp_ID, triple_list_index + 2, 0, 0, 1);break;
		case -2:add_triple(if_op, const_value, triple_list_index + 2, 0, 2, 1);break;
		case -3:add_triple(if_op, const_value, triple_list_index + 2, 0, 2, 1);break;
		case -4:add_triple(if_op, (int)const_string, triple_list_index + 2, 0, 3, 1);break;
		default: add_triple(if_op, (int)exp, triple_list_index + 2, 0, 1, 1);
		}
		backpatch = triple_list_index;
		add_triple(goto_op, -1, -1, 0, 1, -1);
		resume_doubleop(p);
		ptr = ptr->rightSibling->rightSibling;
		gen_triple_code[ptr->nodeType-FUNC_OFFSET](ptr);
		last = triple_list_index - 1;
		triple_list[backpatch].arg1 = (union arg)(last + 2);
		backpatch = triple_list_index;
		add_triple(goto_op, -1, -1, 0, 1, -1);
		ptr = ptr->rightSibling->rightSibling;
		gen_triple_code[ptr->nodeType-FUNC_OFFSET](ptr);
		last = triple_list_index - 1;
		triple_list[backpatch].arg1 = (union arg)(last + 1);
		return triple_list_index - 1;
	}
	return triple_list_index -1;
}

int compound_code(AST_NODE *p)
{
	AST_NODE *ptr;
	add_triple(enterS, -1, -1, -1, -1, -1);
	push_scope(p->symtbl);
	ptr = p->leftChild->rightSibling->rightSibling;
	/**
	 * Brills modified here:
	 * Compound_code act abnormally: without generating statements.
	 */
	//last = gen_triple_code[ptr->nodeType-FUNC_OFFSET](ptr);
	gen_intermediate_code(ptr);
	add_triple(leaveS, -1, -1, -1, -1, -1);
	pop_scope();
	return triple_list_index - 1;
}

int null_code(AST_NODE *p)
{
	return triple_list_index-1;
}
int for_code(AST_NODE *p)
{
	int exp;
	int backpatch;
	int begin;
	int last;
	AST_NODE *ptr;
	gen_triple_code[p->leftChild->nodeType-FUNC_OFFSET](p->leftChild);
	ptr = p->leftChild->rightSibling->rightSibling;
	gen_triple_code[ptr->nodeType-FUNC_OFFSET](ptr);
	ptr = ptr->rightSibling->rightSibling;
	begin = triple_list_index;
	exp = gen_triple_code[ptr->nodeType-FUNC_OFFSET](ptr);
	switch(exp){
		/**
		 * Brills modified here:
		 * argument 2 of if-goto should be an index
		 */
	case -1:add_triple(if_op, (int)temp_ID, triple_list_index + 2, 0, 0, 1);break;
	case -2:add_triple(if_op, const_value, triple_list_index + 2, 0, 2, 1);break;
	case -3:add_triple(if_op, const_value, triple_list_index + 2, 0, 2, 1);break;
	case -4:add_triple(if_op, (int)const_string, triple_list_index + 2, 0, 3, 1);break;
	default: add_triple(if_op, exp, triple_list_index + 2, 0, 1, 1);
	}
	add_triple(goto_op, -1, -1, 0, 1, -1);
	backpatch = triple_list_index - 1;
	ptr = ptr->rightSibling->rightSibling->rightSibling->rightSibling;
	gen_triple_code[ptr->nodeType-FUNC_OFFSET](ptr);
	ptr = p->leftChild->rightSibling->rightSibling->rightSibling->rightSibling->rightSibling->rightSibling;
	gen_triple_code[ptr->nodeType-FUNC_OFFSET](ptr);
	//printf("%d\n", last);	
	resume_doubleop(p);
	last = triple_list_index-1;	
	triple_list[backpatch].arg1 = (union arg)(last+2);
	add_triple(goto_op, begin, -1, 0, 1, -1);
	return triple_list_index -1;
}

int while_code(AST_NODE *p)
{
	int begin = triple_list_index;
	int exp;
	int backpatch;
	int last;

	AST_NODE *ptr;
	ptr = p->leftChild->rightSibling->rightSibling;
	exp = gen_triple_code[ptr->nodeType-FUNC_OFFSET](ptr);
	switch(exp){
		/**
		 * Brills modified here:
		 * arg2_type of add_triple below should be 1 (temp_var)
		 */
	case -1:add_triple(if_op, (int)temp_ID, triple_list_index + 2, 0, 0, 1);break;
	case -2:add_triple(if_op, const_value, triple_list_index + 2, 0, 2, 1);break;
	case -3:add_triple(if_op, const_value, triple_list_index + 2, 0, 2, 1);break;
	case -4:add_triple(if_op, (int)const_string, triple_list_index + 2, 0, 3, 1);break;
	default: add_triple(if_op, exp, triple_list_index + 2, 0, 1, 1);
	}
	backpatch = triple_list_index;
	add_triple(goto_op, -1, -1, 0, 1, -1);

	ptr = ptr->rightSibling->rightSibling;
	resume_doubleop(p);
	gen_triple_code[ptr->nodeType-FUNC_OFFSET](ptr);
	last = triple_list_index - 1;
	triple_list[backpatch].arg1 = (union arg)(last+2);
	add_triple(goto_op, begin, -1, 0, 1, -1);
	return triple_list_index -1;
}


int expression_code(AST_NODE *p)
{
	p = p -> leftChild;
	return gen_triple_code[p->nodeType-FUNC_OFFSET](p);
}

int rvalue_code(AST_NODE *p)
{
	AST_NODE *ptr;
	AST_NODE *ptr2;
	int childNum = 0;
	int temp_rvalue=0;
	int temp_rvalue2=0;
	symtbl_item* temp_symtbl;
	symtbl_hdr* temp_hdr;
	int temp_index;
	stack_item temp_item;
	int *arg_list;
	int* arg_type_list;
	//int* arg_size_list;不一定需要
	int i, arg_num;
	//union arg temp1,temp2,dest;
	for(ptr = p->leftChild ; ptr!=NULL ; ptr = ptr->rightSibling)
		childNum++;
	switch(childNum){
	case 1:{
		if(p->leftChild->nodeType == LVALUE){
			temp_rvalue = gen_triple_code[p->leftChild->nodeType-FUNC_OFFSET](p->leftChild);
			if(temp_rvalue != -1){
				add_triple(star_op, temp_rvalue, -1, triple_list[temp_rvalue].result_type, 1, -1);
				return triple_list_index-1;
			}
			else{
				return -1; //return -1 : rvalue 此处应该被替换成一个id
			}
		}
		else{
			if(p->leftChild->leftChild->nodeType == STRING_CONSTANT_T){
				const_string = p->leftChild->leftChild->content.s_content;
				return -4;//return -4: 替换成一个string constant
			}
			else if(p->leftChild->leftChild->nodeType == CHAR_CONSTANT_T){
				const_value = p->leftChild->leftChild->content.c_content;
				return -3;//return -3 : rvalue 应该被替换成一个constant
			}
			else if(p->leftChild->leftChild->nodeType == ICONSTANT_T){
				const_value = p->leftChild->leftChild->content.i_content;
				return -2;//return -2 : rvalue 应该被替换成一个constant
			}
		}
	}
	case 2: {
		if(p->leftChild->rightSibling->nodeType != DOUBLE_OP_T && p->leftChild->nodeType != DOUBLE_OP_T){
			ptr = p->leftChild->rightSibling;
			temp_rvalue = gen_triple_code[ptr->nodeType-FUNC_OFFSET](ptr);
			switch(p->leftChild->nodeType){
			case POSITIVE_SIGN:
			{
				add_triple_single_op(temp_rvalue,positive_op,ptr);
				return triple_list_index -1;
			}
			case NEGATIVE_SIGN:
			{
				add_triple_single_op(temp_rvalue,negative_op,ptr);
				return triple_list_index -1;
			}
			case NOT_SIGN:
			{
				add_triple_single_op(temp_rvalue,not_op,ptr);
				return triple_list_index -1;
			}
			case ADDRESS_SIGN:
			{
				if(temp_rvalue == -1){
					add_triple_single_op(temp_rvalue,address_op,ptr);
					return triple_list_index - 1;
				}
				else{ //& 后面是 *rvalue
					return temp_rvalue;			
				}
			}
			}
		}
		else if(p->leftChild->nodeType == DOUBLE_OP_T){//++ a		
			ptr = p ->leftChild -> rightSibling;
			temp_rvalue = gen_triple_code[ptr->nodeType-FUNC_OFFSET](ptr);
			if(temp_rvalue == -1){
				/**
				 * Brills modified here:
				 * currently, ptr is LVALUE,not IDENT, so without s_content
				 * ptr->leftChild is IDENT
				 */
				temp_symtbl = symtbl_query(ptr->leftChild->symtbl, ptr->leftChild->content.s_content, 0);
				assert(temp_symtbl != NULL);
				/**
				 * Brills modified here:
				 * what is the difference between CHAR_T and INT_T here??
				 */
				//if(temp_symtbl->type == CHAR_T){
				if(temp_symtbl->type == CHAR_T&& temp_symtbl->star_num ==0){
					if(p->leftChild->content.i_content == PLUSPLUS) add_triple(add_op, (int)ptr->leftChild->content.s_content, 1, 0, 0, 2);
					else add_triple(minus_op, (int)ptr->leftChild->content.s_content, 1, 0, 0, 2);
					temp_index = triple_list_index-1;
					add_triple(assign_op, (int)ptr->leftChild->content.s_content, temp_index, 0, 0, 1);
				}
				else{					
					if(p->leftChild->content.i_content == PLUSPLUS) add_triple(add_op, (int)ptr->leftChild->content.s_content, 1, 1, 0, 2);
					else add_triple(minus_op, (int)ptr->leftChild->content.s_content, 1, 1, 0, 2);
					//add_triple(add_op, ptr->leftChild->content.s_content, 1, 1, 0, 2);
					temp_index = triple_list_index-1;
					add_triple(assign_op, (int)ptr->leftChild->content.s_content, temp_index, 1, 0, 1);
				}					
			}
			else{
				add_triple(star_op,temp_rvalue,-1,triple_list[temp_rvalue].result_type,1,0);
				temp_index = triple_list_index-1;
				add_triple(add_op,temp_index,1,triple_list[temp_index].result_type,1,2);
				temp_index = triple_list_index-1;
				add_triple(star_assign_op, temp_rvalue, temp_index, triple_list[temp_index].result_type, 1, 1);				
			}
			return triple_list_index-2;
		}
		else{//a ++
			ptr = p ->leftChild;
			temp_rvalue = gen_triple_code[ptr->nodeType-FUNC_OFFSET](ptr);
			if(temp_rvalue == -1){
				//fprintf(stderr,"%s\n",ptr->leftChild->content.s_content);
				temp_symtbl = symtbl_query(ptr->symtbl,ptr->leftChild->content.s_content,  0);
				assert(temp_symtbl != NULL);
				temp_item.item = (union arg)(ptr->leftChild->content.s_content);
				//fprintf(stderr,"%s\n",temp_item.item.var_name);
				if(ptr->rightSibling->content.i_content == PLUSPLUS) temp_item.flag = 1;
				else temp_item.flag = 0;
				temp_item.item_type = 0;
				push(temp_item);
				return temp_rvalue;
			}
			else{
				temp_item.item = (union arg)temp_rvalue;
				if(ptr->rightSibling->content.i_content == PLUSPLUS) temp_item.flag = 1;
				else temp_item.flag = 0;
				temp_item.item_type = 1;
				push(temp_item);
				add_triple(star_op,temp_rvalue,-1,triple_list[temp_rvalue].result_type,1,0);
				return triple_list_index - 1;
			}
			
		}
	}//end case 2;
	case 3:
	{		
		if(p->leftChild->nodeType == LEFT_PARENTHESE){
			ptr = p -> leftChild -> rightSibling;
			temp_rvalue = gen_triple_code[ptr->nodeType-FUNC_OFFSET](ptr);
			return temp_rvalue;
		}
		else if(p->leftChild->rightSibling->nodeType == LEFT_PARENTHESE){
			//函数
			temp_symtbl = symtbl_query(p->leftChild->symtbl, p->leftChild->content.s_content, 0);
			temp_hdr = func_query(tree_root->symtbl, (p->leftChild->content).s_content);
			if( temp_hdr->ret_type == CHAR_T && temp_hdr->ret_star ==0) add_triple(call,temp_symtbl->addr_off, -1, 0, 1, -1);
			else add_triple(call,temp_symtbl->addr_off, -1, 1, 1, -1);
			return triple_list_index - 1;
		}
		else{
			ptr = p->leftChild;
			ptr2 = p->leftChild->rightSibling->rightSibling;
			//temp_rvalue = gen_triple_code[ptr->nodeType-FUNC_OFFSET](ptr);
			//temp_rvalue2 = gen_triple_code[ptr2->nodeType-FUNC_OFFSET](ptr2);
			switch(p->leftChild->rightSibling->nodeType){
			case PLUS_SIGN:{
				add_triple_double_op(temp_rvalue, temp_rvalue2, add_op, ptr, ptr2);
				return triple_list_index -1;						
			}
			case MINUS_SIGN:{
				add_triple_double_op(temp_rvalue, temp_rvalue2, minus_op, ptr, ptr2);
				return triple_list_index -1;						
			}
			case MULTIPLY_SIGN:{
				add_triple_double_op(temp_rvalue, temp_rvalue2, multiply_op, ptr, ptr2);
				return triple_list_index -1;						
			}
			case OP:{
				switch(p->leftChild->rightSibling->leftChild->content.i_content){
				case ANDAND:{
					add_triple_double_op(temp_rvalue, temp_rvalue2, and_op, ptr, ptr2);
					return triple_list_index -1;				
				}
				case OROR:{
					add_triple_double_op(temp_rvalue, temp_rvalue2, or_op, ptr, ptr2);
					return triple_list_index -1;				
				}
				case EQEQ:{
					add_triple_double_op(temp_rvalue, temp_rvalue2, equal_op, ptr, ptr2);
					return triple_list_index -1;				
				}
				case NOTEQ:{
					add_triple_double_op(temp_rvalue, temp_rvalue2, noteq_op, ptr, ptr2);
					return triple_list_index -1;				
				}
				case LARGERTHAN:{
					add_triple_double_op(temp_rvalue, temp_rvalue2, larger_op, ptr, ptr2);
					return triple_list_index -1;				
				}
				case SMALLERTHAN:{
					add_triple_double_op(temp_rvalue, temp_rvalue2, less_op, ptr, ptr2);
					return triple_list_index -1;				
				}
				case LTEQ:{
					add_triple_double_op(temp_rvalue, temp_rvalue2, eqlarger_op, ptr, ptr2);
					return triple_list_index -1;				
				}
				case STEQ:{
					add_triple_double_op(temp_rvalue, temp_rvalue2, eqless_op, ptr, ptr2);
					return triple_list_index -1;				
				}
				}
										
			}
			}		
		}

	}//end case 3;
	case 4:{
		arg_num = 0;
		/**
		 *Brills modified here: 
		 *maybe memory leak?? you should free the malloced memory everytime...
		 */
		arg_list = alist1; //malloc(100*sizeof(int));
		arg_type_list = alist2; //malloc(100*sizeof(int));
		ptr = p -> leftChild -> rightSibling -> rightSibling;
		while(1){
			if(ptr->leftChild->nodeType == EXPRESSION){
				temp_rvalue = gen_triple_code[ptr->leftChild->nodeType-FUNC_OFFSET](ptr->leftChild);
				if(temp_rvalue == -1){
					arg_list[arg_num] =(int)temp_ID;
					arg_type_list[arg_num] = 0;
					arg_num ++;		
				}
				else if(temp_rvalue == -2){
					if(ptr->leftChild->leftChild->leftChild->leftChild->nodeType == ICONSTANT_T){
						arg_list[arg_num] = const_value;
						arg_type_list[arg_num] = 2;
					}
					else if(ptr->leftChild->leftChild->leftChild->nodeType == CHAR_CONSTANT_T){
						arg_list[arg_num] = const_value;
						arg_type_list[arg_num] = 2;
					}
					else if(ptr->leftChild->leftChild->leftChild->nodeType == STRING_CONSTANT_T){
						arg_list[arg_num] = (int)const_string;
						arg_type_list[arg_num] = 3;
					}
					arg_num ++;
				}
				else{
					arg_list[arg_num] = temp_rvalue;
					arg_type_list[arg_num] = 1;
					arg_num++;	
				}
				break;
			}
			else{

				temp_rvalue = gen_triple_code[ptr->leftChild->rightSibling->rightSibling->nodeType-FUNC_OFFSET](ptr->leftChild->rightSibling->rightSibling);

				if(temp_rvalue == -1){
					arg_list[arg_num] = (int)temp_ID;
					arg_type_list[arg_num] = 0;
					arg_num ++;		
				}
				else if(temp_rvalue == -2){
					if(ptr->leftChild->rightSibling->leftChild->leftChild->leftChild->nodeType == ICONSTANT_T){
						arg_list[arg_num] = const_value;
						arg_type_list[arg_num] = 2;
					}
					else if(ptr->leftChild->rightSibling->leftChild->leftChild->nodeType == CHAR_CONSTANT_T){
						arg_list[arg_num] = const_value;
						arg_type_list[arg_num] = 2;
					}
					else if(ptr->leftChild->rightSibling->leftChild->leftChild->nodeType == STRING_CONSTANT_T){
						arg_list[arg_num] = (int)const_string;
						arg_type_list[arg_num] = 3;
					}
					arg_num ++;
				}
				else{
					arg_list[arg_num] = temp_rvalue;
					arg_type_list[arg_num] = 1;
					arg_num++;	
				}
				ptr = ptr->leftChild;
			}
		}//end while
		for(i = 0 ; i < arg_num ; i++){
			add_triple(param,arg_list[i],-1,1,arg_type_list[i],-1);
		}
		temp_symtbl = symtbl_query(p->leftChild->symtbl, p->leftChild->content.s_content, 0);
		temp_hdr = func_query(tree_root->symtbl, (p->leftChild->content).s_content);
		if(temp_hdr -> ret_type == CHAR_T && temp_hdr ->ret_star ==0) add_triple(call,temp_symtbl->addr_off,-1,0, 1, -1);
		else add_triple(call,temp_symtbl->addr_off,-1,1, 1, -1);
		return triple_list_index - 1;
	}//end case 4
	}
	return triple_list_index -1;
}
int lvalue_code(AST_NODE *p)
{
	int temp_rvalue = 0;
	char* b_ident;
	int b_sizetype;
	symtbl_item *temp_symtbl;
	AST_NODE *ptr;
	if(p->leftChild->nodeType == IDENT_T && p->leftChild->rightSibling == NULL)
	{
		temp_ID = p->leftChild->content.s_content;
		return -1;
	}
	else if(p->leftChild->nodeType == STAR){
		temp_rvalue = gen_triple_code[p->leftChild->rightSibling->nodeType-FUNC_OFFSET](p->leftChild->rightSibling);
		if(temp_rvalue == -1){
			ptr = p->leftChild->rightSibling;
			temp_symtbl = symtbl_query(ptr->symtbl, temp_ID, 0);
			assert(temp_symtbl != NULL);
			if(temp_symtbl->type == CHAR_T&& temp_symtbl->star_num ==0)	add_triple(add_op, (int)temp_ID,0,1,0,2);
			else 	add_triple(add_op, (int)temp_ID,0,1,0,2);
			return triple_list_index - 1;
		}
		else return temp_rvalue;
	}
	else{
//b_ident: get the basic address	
		ptr = p->leftChild;
		temp_symtbl = symtbl_query(p->leftChild->symtbl, p->leftChild->content.s_content, 0);
		b_ident = p->leftChild->content.s_content;
		assert(temp_symtbl != NULL);
		if(temp_symtbl->type == CHAR_T ) b_sizetype = 0;
		else b_sizetype = 1;

		ptr = p->leftChild->rightSibling->rightSibling;
		temp_rvalue = gen_triple_code[ptr->nodeType-FUNC_OFFSET](ptr);

		if(b_sizetype == 0){
			switch(temp_rvalue){
			case -1:{
				temp_symtbl = symtbl_query(ptr->symtbl,temp_ID, 0);
				if(temp_symtbl->type == CHAR_T && temp_symtbl->star_num == 0){
					add_triple(char_to_int_op, (int)temp_ID,-1,1,0,-1);
					add_triple(add_op, (int)b_ident, triple_list_index-1,b_sizetype,0,1);
				}
				else add_triple(add_op, (int)b_ident, (int)temp_ID , b_sizetype,0,0);
				break;
			}
			case -2:{
				add_triple(add_op,(int)b_ident,const_value,b_sizetype,0,2);
				break;
			}
			case -3:{
				add_triple(add_op,(int)b_ident,const_value,b_sizetype,0,2);
				break;
			}
			case -4:{
				add_triple(add_op,(int)b_ident,(int)const_string,b_sizetype,0,3);
				break;
			}
			default:{
				if(triple_list[temp_rvalue].result_type == 0){
					add_triple(char_to_int_op, temp_rvalue,-1,1,1,-1);
					add_triple(add_op, (int)b_ident, triple_list_index-1,b_sizetype,0,1);
				}
				else add_triple(add_op,(int)b_ident,temp_rvalue,b_sizetype,0,1);
				break;
			}			
			}
		}

		else{
			switch(temp_rvalue){
			case -1:{
				temp_symtbl = symtbl_query(ptr->symtbl, temp_ID, 0);
				if(temp_symtbl->type == CHAR_T && temp_symtbl->star_num == 0){
					add_triple(char_to_int_op, (int)temp_ID,-1,1,0,-1);
					add_triple(adds_op, (int)b_ident, triple_list_index-1,b_sizetype,0,1);
				}
				else
				add_triple(adds_op, (int)b_ident, (int)temp_ID , b_sizetype,0,0);
				break;
			}
			case -2:{
				add_triple(adds_op,(int)b_ident,const_value,b_sizetype,0,2);
				break;
			}
			case -3:{
				add_triple(adds_op,(int)b_ident,const_value,b_sizetype,0,2);
				break;
			}
			case -4:{
				add_triple(adds_op,(int)b_ident,(int)const_string,b_sizetype,0,3);
				break;
			}
			default:{
				if(triple_list[temp_rvalue].result_type == 0){
					add_triple(char_to_int_op, temp_rvalue,-1,1,1,-1);
					add_triple(adds_op, (int)b_ident, triple_list_index-1,b_sizetype,0,1);
				}
				else add_triple(adds_op,(int)b_ident,temp_rvalue,b_sizetype,0,1);
				break;
			}			
			}
		}
		return triple_list_index - 1;
/*
		ptr = p->leftChild->rightSibling->rightSibling;
		temp_rvalue = gen_triple_code[ptr->nodeType-FUNC_OFFSET](ptr);
		temp_symtbl = symtbl_query(p->leftChild->symtbl, p->leftChild->content.s_content, 0);
		assert(temp_symtbl != NULL);
		if(temp_symtbl->type == CHAR_T){
			switch(temp_rvalue){
			case -1:{
				add_triple(multiply_op, (int)temp_ID,1,1,0,2);
				break;
			}
			case -2:{
				add_triple(multiply_op,const_value,1,1,2,2);
				break;
			}
			case -3:{
				add_triple(multiply_op,const_value,1,1,2,2);
				break;
			}
			case -4:{
				add_triple(multiply_op,(int)const_string,1,1,3,2);
				break;
			}
			default:{
				add_triple(multiply_op,temp_rvalue,1,1,1,2);
				break;
			}
			}
		}
		else{
			switch(temp_rvalue){
			case -1:{
				add_triple(multiply_op, (int)temp_ID,4,1,0,2);
				break;
			}
			case -2:{
				add_triple(multiply_op,const_value,4,1,2,2);
				break;
			}
			case -3:{
				add_triple(multiply_op,const_value,4,1,2,2);
				break;
			}
			case -4:{
				add_triple(multiply_op,(int)const_string,4,1,3,2);
				break;
			}
			default:{
				add_triple(multiply_op,temp_rvalue,4,1,1,2);
				break;
			}
			}
		}

		
		ptr = p->leftChild;
		temp_symtbl = symtbl_query(p->leftChild->symtbl, p->leftChild->content.s_content, 0);
		assert (temp_symtbl != NULL);
		if(temp_symtbl->type == CHAR_T && temp_symtbl->star_num ==0)	add_triple(add_op, (int)ptr->content.s_content, triple_list_index - 1, 0, 0, 1);
		else 	add_triple(add_op, (int)ptr->content.s_content, triple_list_index - 1, 1, 0, 1);
		return triple_list_index - 1;
		*/
	}
}
int assignment_expression_code(AST_NODE *p)
{
	int temp_rvalue = 0;
	int temp_lvalue = 0;
	if(p->leftChild->leftChild->nodeType == IDENT_T && p->leftChild->leftChild->rightSibling == NULL) temp_lvalue = -1;
	if(temp_lvalue == -1) add_triple_double_op(temp_lvalue, temp_rvalue, assign_op, p->leftChild, p->leftChild->rightSibling->rightSibling);
	else add_triple_double_op(temp_lvalue, temp_rvalue, star_assign_op, p->leftChild, p->leftChild->rightSibling->rightSibling);
	return triple_list_index - 1;
}
int return_code(AST_NODE *p)
{
	int temp_rvalue;
	symtbl_item* temp_symtbl;
	if(p->leftChild->rightSibling->nodeType == SEMICOLON){
		add_triple(return_op,-1,-1,-1,-1,-1);
		return triple_list_index-1;
	}		
	temp_rvalue = gen_triple_code[p->leftChild->rightSibling->nodeType-FUNC_OFFSET](p->leftChild->rightSibling);
	switch(temp_rvalue){
	case -1:{
		temp_symtbl = symtbl_query(p->symtbl,temp_ID,  0);
		assert(temp_symtbl != NULL);
		if(temp_symtbl->type == CHAR_T && temp_symtbl->star_num ==0)	add_triple(return_op,(int)temp_ID, -1, 0, 0,-1);
		else add_triple(return_op,(int)temp_ID, -1, 1, 0,-1);
		break;
	}
	case -2:{
		add_triple(return_op, const_value, -1, 1, 2, -1);
		break;

	}
	case -3:{
		add_triple(return_op, const_value, -1, 0, 2, -1);
		break;
	}
	case -4:{
		add_triple(return_op, (int)const_string, -1, 1, 3, -1);
		break;
	}
	default:{
		add_triple(return_op, temp_rvalue, -1, triple_list[temp_rvalue].result_type, 1, -1);
		break;
	}
	}
	return triple_list_index - 1;	
}


void add_triple_single_op(int temp_rvalue, enum operator op, AST_NODE *ptr)
{
	symtbl_item* temp_symtbl;
	temp_rvalue = gen_triple_code[ptr->nodeType-FUNC_OFFSET](ptr);
	switch(temp_rvalue){
	case -1:{
		temp_symtbl = symtbl_query(ptr->symtbl, temp_ID, 0);
		assert(temp_symtbl != NULL);
		if(temp_symtbl->type == CHAR_T&& temp_symtbl->star_num ==0)	add_triple(op,(int)temp_ID, -1, 0, 0,-1);
		else add_triple(op,(int)temp_ID, -1, 1, 0,-1);
		break;
	}
	case -2:{
		add_triple(op, const_value, -1, 1, 2, -1);
		break;

	}
	case -3:{
		add_triple(op, const_value, -1, 0, 2, -1);
		break;
	}
	case -4:{
		add_triple(op, (int)const_string, -1, 1, 3, -1);
		break;
	}
	default:{
		add_triple(op, temp_rvalue, -1, triple_list[temp_rvalue].result_type, 1, -1);
		break;
	}
	}
}
void add_triple_double_op(int temp_rvalue1, int temp_rvalue2, enum operator op, AST_NODE *ptr1, AST_NODE *ptr2)//1, 2分别对应两个操作数
{
	//union arg var1,var2;
	int var1, var2;
	int size_type1,size_type2;//char or int
	int var_type1,var_type2;
	int temp_index;
	symtbl_item *temp_symtbl;
//var 1;
	temp_rvalue1 = gen_triple_code[ptr1->nodeType-FUNC_OFFSET](ptr1);
	switch(temp_rvalue1){
	case -1:{
		var1 = (int)temp_ID;
		temp_symtbl = symtbl_query(ptr1->symtbl, temp_ID, 0);
		assert(temp_symtbl != NULL);
		if(temp_symtbl->type == CHAR_T && temp_symtbl->star_num == 0) size_type1 = 0;
		else size_type1 = 1;
		var_type1 = 0;
		break;
	}
	case -2:{
		var1 = const_value;
		size_type1 = 1;
		var_type1 = 2;
		break;
	}
	case -3:{
		var1 = const_value;
		size_type1 = 0;
		var_type1 = 2;	
		break;	
	}
	case -4:{
		var1 = (int)const_string;
		size_type1 = 1;
		var_type1 = 2;
		break;
	}
	default:{
		var1 = temp_rvalue1;
		size_type1 = triple_list[temp_rvalue1].result_type;
		var_type1 = 1;
	}
	}
//var2
	temp_rvalue2 = gen_triple_code[ptr2->nodeType-FUNC_OFFSET](ptr2);
	switch(temp_rvalue2){
	case -1:{
		var2 = (int)temp_ID;
		temp_symtbl = symtbl_query(ptr2->symtbl, temp_ID, 0);
		assert(temp_symtbl != NULL);
		if(temp_symtbl->type == CHAR_T && temp_symtbl->star_num == 0) size_type2 = 0;
		else size_type2 = 1;
		var_type2 = 0;
		break;
	}
	case -2:{
		var2 = const_value;
		size_type2 = 1;
		var_type2 = 2;
		break;
	}
	case -3:{
		var2 = const_value;
		size_type2 = 0;
		var_type2 = 2;	
		break;	
	}
	case -4:{
		var2 = (int)const_string;
		size_type2 = 1;
		var_type2 = 2;
		break;
	}
	default:{
		var2 = temp_rvalue2;
		size_type2 = triple_list[temp_rvalue2].result_type;
		var_type2 = 1;
	}
	}
	if(op == add_op || op == minus_op || op == multiply_op || op == assign_op || op == star_assign_op){
		if(size_type1 == 1 && size_type2 == 0){
			/**Brills modified here: -1 for var_type2??*/
			add_triple(char_to_int_op, var2,-1,1,var_type2,-1);
			
			temp_index = triple_list_index-1;
			add_triple(op, var1, temp_index, 1, var_type1, 1);	
		}
		else if(size_type1 == 0 && size_type2 == 1 && op != assign_op){
			/**Brills modified here: -1 for var_type1??*/			
			add_triple(char_to_int_op, var1,-1,1,var_type1,-1);
			temp_index = triple_list_index-1;
			add_triple(op, temp_index, var2, 1, 1, var_type2);	
		}
		else add_triple(op, var1, var2, size_type1, var_type1, var_type2);
	}
	else if (op == equal_op || op == less_op || op == larger_op || op == eqless_op || op == eqlarger_op || op == noteq_op){
		add_triple(op, var1, var2, 1, var_type1, var_type2);
	}
	else if (op == or_op){
		add_triple(set_rb,1,-1, 1,2,-1);
		/**
		 * Brills modified here:
		 * argument 2 of if-goto should be an index
		 */
		add_triple(if_op, var1, triple_list_index + 3, 1, var_type1, 1);//if跳转指令 第三个操作数是一个立即数
		add_triple(if_op, var2, triple_list_index + 2, 1, var_type2, 1);
		add_triple(set_rb,0,-1,1,2,-1);
		add_triple(get_rb,-1,-1,1,-1,-1);
		//return triple_list_index - 1;
	}
	else if (op == and_op){
		add_triple(set_rb,0,-1, 1,2,-1);
		add_triple(if_not_op, var1, triple_list_index + 3, 1, var_type1, 2);//if_not跳转指令 第三个操作数是一个立即数
		add_triple(if_not_op, var2, triple_list_index + 2, 1, var_type2, 2);
		add_triple(set_rb,1,-1,1,2,-1);
		add_triple(get_rb,-1,-1,1,-1,-1);
		//return triple_list_index - 1;
	}
}
void resume_doubleop(AST_NODE *p)
{
	stack_item temp_item;
	symtbl_item *temp_symtbl;
	int a_index;
	
	while(stack_top!=0){
		temp_item = pop();
		if(temp_item.item_type == 0){
			temp_symtbl = symtbl_query(p->symtbl,temp_item.item.var_name, 0);
			if(temp_symtbl->type == CHAR_T&& temp_symtbl->star_num ==0) {
				if(temp_item.flag == 1) add_triple(add_op, (int)temp_item.item.var_name, 1, 0, 0, 2);
				else add_triple(minus_op, (int)temp_item.item.var_name, 1, 0, 0, 2);
				a_index = triple_list_index-1;
				add_triple(assign_op, (int)temp_item.item.var_name, a_index, 0, 0, 1);
			}
			else {
				if(temp_item.flag == 1) add_triple(add_op, (int)temp_item.item.var_name, 1, 1, 0, 2);
				else add_triple(minus_op, (int)temp_item.item.var_name, 1, 1, 0, 2);
				a_index = triple_list_index-1;
				add_triple(assign_op, (int)temp_item.item.var_name, a_index, 1, 0, 1);
			}		
		}
		else{
			add_triple(star_op,temp_item.item.temp_index,-1,triple_list[temp_item.item.temp_index].result_type,1,0);
			a_index = triple_list_index-1;
			add_triple(add_op,a_index,1,triple_list[a_index].result_type,1,2);
			a_index = triple_list_index-1;
			add_triple(star_assign_op, temp_item.item.temp_index, a_index, triple_list[a_index].result_type, 1, 1);	
		}
	}
}

