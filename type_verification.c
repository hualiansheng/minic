#include "type_verification.h"
#include <stdio.h>
#include "validation_utils.h"
extern char name[][30];
data_type (*check_type[67])(AST_NODE*);	//function pointer
void check_initial()
{
	int i;
	for(i = 0 ; i < 67 ;i++){
		check_type[i] = check_type_SIGN;
	}
	check_type[EXPRESSION-FUNC_OFFSET] = check_type_exp;
	check_type[ASSIGNMENT_EXPRESSION-FUNC_OFFSET] = check_type_assignment;
	check_type[LVALUE-FUNC_OFFSET] = check_type_lvalue;
	check_type[RVALUE-FUNC_OFFSET] = check_type_rvalue;
	check_type[OP-FUNC_OFFSET] = check_type_op;
	check_type[CONSTANT-FUNC_OFFSET] = check_type_constant;
	check_type[IDENT_T-FUNC_OFFSET] = check_type_ID;
}
void dfs_type_verification(AST_NODE* root)
{
	AST_NODE* p = root;
//	int type = root -> nodeType;
//	fprintf(stdout,"scanning %d%s...\n",type,name[p->nodeType-1001]);
//	fflush(stdout);
	check_initial();
	if(p->nodeType == EXPRESSION){
		check_type[p->nodeType-FUNC_OFFSET](p);
		return;
	}
	if(p->leftChild == NULL) return;
	for(p = p->leftChild ; p != NULL ; p = p->rightSibling)
		dfs_type_verification(p);
}
data_type check_type_exp(AST_NODE* exp_node)
{
	AST_NODE* p = exp_node;
	int i = 0 ;
	data_type child_type[1];
	for( p = p->leftChild; p!=NULL; p=p->rightSibling)
	{
		child_type[i++] = check_type[p->nodeType-FUNC_OFFSET](p);
	}
	if(child_type[0].type == -1) return check_wrong();
	return child_type[0];
}
data_type check_type_assignment(AST_NODE* assignment_node)
{
	AST_NODE* p = assignment_node;
	int i, j;
	data_type child_type[3];
	i = 0;
	for( p = p->leftChild; p!=NULL; p=p->rightSibling)
	{
		child_type[i++] = check_type[p->nodeType-FUNC_OFFSET](p);
	}
	for(j = 0 ;j < i ; j ++)
	{
		if(child_type[j].type == -1 )return check_wrong();
	}
	/*if(check_double(child_type[1].type,child_type[0],child_type[2]) == 0){
		return check_wrong();
	}
	else return child_type[0];*/
	return check_double(child_type[1].type,child_type[0],child_type[2]);
}
data_type check_type_lvalue(AST_NODE* lvalue)
{
	AST_NODE* p = lvalue;
	int i, j;
	data_type child_type[4];
	i = 0;
	for( p = p->leftChild; p!=NULL; p=p->rightSibling)
	{
		child_type[i++] = check_type[p->nodeType-FUNC_OFFSET](p);
	}
	for(j = 0 ; j < i ; j++)
	{
		if(child_type[j].type == -1) return check_wrong();
	}
	if( i == 1){
		symtbl_item *temp_item = symtbl_query(lvalue->leftChild->symtbl, (lvalue->leftChild->content).s_content, 0);
		if(temp_item->type == FUNCTION_DEF){
			printf("%s is not a variable\n",(lvalue->leftChild->content).s_content);
			return check_wrong();		
		}
		return child_type[0];
	}
	else if(i == 2){
		if(child_type[1].star_num <= 0) return check_wrong();
		child_type[1].star_num -= 1;
		return child_type[1];
	}
	else if(i == 4){
		//printf("%d %d %d ", child_type[0].star_num, child_type[2].type, child_type[2].star_num);
		if(child_type[0].star_num == 1 /*&& child_type[0].size >= 0*/ && (child_type[2].type == INT_T || child_type[2].type == CHAR_T) 
			&& child_type[2].star_num == 0){
			child_type[0].star_num -= 1;
			child_type[0].size = -1;
			return child_type[0];
		}
		else{
			printf("array error!\n");
			return check_wrong();
		}
	}
	return check_wrong();
}
data_type check_type_rvalue(AST_NODE* rvalue)
{
	AST_NODE* p = rvalue;
	int i, j, k;
	data_type child_type[4];
	data_type para_type[20];
	symtbl_hdr* h;
	i = 0;
	for( p = p->leftChild; p!=NULL; p=p->rightSibling)
	{
		child_type[i++] = check_type[p->nodeType-FUNC_OFFSET](p);
	}
	for(j = 0 ; j < i ; j++)
	{
		if(child_type[j].type == -1) return check_wrong();
	}
	if(i == 1) return child_type[0];
	else if(i == 2){
		if(child_type[1].type == DOUBLE_OP_T){
			return check_single(child_type[1].type, child_type[0]);
		}
		else{
			return check_single(child_type[0].type, child_type[1]);
		}
	}
	else if(i == 3){
		if(child_type[0].type == LEFT_PARENTHESE) return child_type[1];
		else if(child_type[1].type == LEFT_PARENTHESE){
			h = func_query(tree_root->symtbl, (rvalue->leftChild->content).s_content);
			if(h == NULL){
				printf("error!!!!!!!\n");
				return check_wrong();			
			}
			if(h->para_num != 0){
				printf("para error!\n");
				return check_wrong();		
			}
			else return child_type[0];
		}
		else return check_double(child_type[1].type, child_type[0], child_type[2]);			
	}
	else if(i == 4){
		k = 0;
		h = func_query(tree_root->symtbl, (rvalue->leftChild->content).s_content);
		child_type[0].type = h->ret_type;
		child_type[0].star_num = h->ret_star;
		child_type[0].size = -1;
		p = rvalue -> leftChild -> rightSibling -> rightSibling;
		while(1){
			p = p->leftChild;
			if(p->nodeType == EXPRESSION){
				para_type[k++] = check_type[p->nodeType-FUNC_OFFSET](p);
				break;
			}
			else{
				para_type[k++] = check_type[p->rightSibling->rightSibling->nodeType-FUNC_OFFSET](p->rightSibling->rightSibling);
			}		
		}
		if(k != h -> para_num){
			printf("para error\n");
			return check_wrong();		
		}
		else{
			for(j = 0 ; j < k ; j ++){
				//printf("%d %d %d %d", para_type[j].star_num,para_type[j].type,h->item[j].star_num,h->item[j].type);
				if(para_type[j].star_num != h->item[j].star_num || para_type[j].type != h->item[j].type)
					printf("param warning\n");
			}		
		}
		return child_type[0];
	}
	else return check_wrong();
}


data_type check_type_SIGN(AST_NODE *p)
{
	data_type temp;
	temp.type = p->nodeType;
	return temp;
}

data_type check_type_ID(AST_NODE *p)
{
	data_type temp;
	symtbl_item *id_item = symtbl_query(p->symtbl, (p->content).s_content, 0);
	if(id_item == NULL){
		printf("%s ",(p->content).s_content);
		printf("Not Found!\n");
		return check_wrong();	
	}
	else{
		temp.type = id_item->type;
		temp.star_num = id_item->star_num;
		temp.size = id_item->size;
		return temp;
	}
}

data_type check_type_op(AST_NODE *p)
{
	p = p -> leftChild;
	return check_type[p->nodeType-FUNC_OFFSET](p);	
}
data_type check_type_constant(AST_NODE *p)
{
	data_type temp;
	p = p ->leftChild;
	switch(p->nodeType){
	case ICONSTANT_T:{
		temp.type = INT_T;
		temp.star_num = 0;
		temp.size = -1;
		break;
	}
	case CHAR_CONSTANT_T:{
		temp.type = CHAR_T;
		temp.star_num = 0;
		temp.size = -1;
		break;
	}	
	case STRING_CONSTANT_T:{
		temp.type = CHAR_T;
		temp.star_num = 1;
		temp.size = 1;	
		break;
	}
	default: return check_wrong();
	}
	return temp;	
}




/**
	检查单目运算符
*/
data_type check_single(int op_type,data_type op_num)
{
	switch(op_type){
	case POSITIVE_SIGN:
	{
		if((op_num.type == INT_T || op_num.type == CHAR_T) && op_num.star_num == 0) return op_num;
		else{
			return check_wrong();
		}		
		break;
	}
	case NEGATIVE_SIGN:
	{
		if((op_num.type == INT_T || op_num.type == CHAR_T) && op_num.star_num == 0) return op_num;
		else{
			return check_wrong();
		}		
		break;
	}
	case NOT_SIGN:{
		if(op_num.type != VOID_T || op_num.star_num != 0){
			op_num.type = INT_T;
			op_num.star_num = 0;
			op_num.size = -1;
			return op_num;		
		}
		else{
			return check_wrong();
		}
		break;
	}
	case ADDRESS_SIGN:{
		if(op_num.type != VOID_T){
			op_num.star_num += 1;
			op_num.size = -1;
			return op_num;		
		}
		else{
			return check_wrong();
		}
		break;
	}
	case DOUBLE_OP_T:{
		if(!(op_num.star_num >= 1 && op_num.size != -1)){
			return op_num;		
		}
		else{
			return check_wrong();
		}
		break;
	}
	default:{
		return check_wrong();
		break;
	}
	}
}


/*
	检查双目运算符
*/
data_type check_double(int op_type,data_type op1, data_type op2)
{
	switch(op_type){
	case EQUALITY_SIGN:{
		if(op1.star_num != 0 && op1.size != -1){
			return check_wrong();		
		}
		if(op1.star_num != op2.star_num){
			printf("warning!\n");
			return op1;	
		}
		else if(op1.star_num == op2.star_num){
			if(op1.type == CHAR_T && op2.type == INT_T){
				printf("warning!\n");
				return op1;			
			}
			else return op1;
		}
		break;
	}
	case OP:{
		if(op1.star_num != op2.star_num){
			printf("warning!\n");
			op1.type = INT_T;
			op1.star_num = 0;
			op1.size = -1;
			return op1;
		}
		else if(op1.type != op2.type){
			printf("warning!\n");
			op1.type = INT_T;
			op1.star_num = 0;
			op1.size = -1;
			return op1;
		}
		else{
			op1.type = INT_T;
			op1.star_num = 0;
			op1.size = -1;
			return op1;
		}
		break;
	}
	case MINUS_SIGN:{
		if(op1.star_num != op2.star_num){
			if(op2.star_num > 0){
				printf("error!\n");
				return check_wrong();
			}
			else{
				op1.size = -1;
				return op1;			
			}
		}
		else if(op1.type != op2.type && op1.star_num > 0){
			printf("error!\n");
			return check_wrong();			
		}
		else {
			op1.type = INT_T;
			op1.star_num = 0;
			op1.size = -1;
			return op1;	
		}
		break;
	}
	case PLUS_SIGN:{
		if(op1.star_num != op2.star_num){
			if(op2.star_num == 0){
				op1.size = -1;
				return op1;	
			}
			else if(op1.star_num ==0){
				op2.size = -1;
				return op2;			
			}
			else{
				printf("error!\n");
				return check_wrong();			
			}
		}
		else if(op1.star_num > 0){
			printf("error!\n");
			return check_wrong();			
		}
		else {
			op1.type = INT_T;
			op1.star_num = 0;
			op1.size = -1;
			return op1;	
		}
		break;
	}
	case MULTIPLY_SIGN:{
		if(op1.star_num >0 ||  op2.star_num > 0){
			printf("error!\n");
			return check_wrong();			
		}
		else {
			op1.type = INT_T;
			op1.star_num = 0;
			op1.size = -1;
			return op1;	
		}
		break;
	}
	}
	return check_wrong();

}
/*
	返回一个表示错误的类型
*/
data_type check_wrong()
{
	data_type temp;
	temp.type = -1;
	temp.star_num = -1;
	temp.size = -1;
	return temp;
}
