void dfs_type_verification(AST_NODE* root)
{
	AST_NODE* p = root;
	if(p->nodeType == EXPRESSION){
		check_type[p->nodeType](p);
		return;
	}
	for(p = p->leftChild ; p != NULL ; p = p->rightSibling)
		dfs_type_verification(p);
}
data_type check_type_exp(AST_NODE* exp_node)
{
	AST_NODE* p = exp_node;
	int i = 0 ;
	int j;
	data_type child_type[1];
	for( p = p->leftChild; p!=NULL; p=p->rightSibling)
	{
		child_type[i++] = check_type[p->nodeType](p);
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
		child_type[i++] = check_type[p->nodeType](p);
	}
	for(j = 0 ;j < i ; j ++)
	{
		if(child_type[j].type == -1 )return check_wrong();
	}
	if(check_double(child_type[1].type,child_type[0],child_type[2]) == 0){
		return check_wrong();
	}
	else return child_type[0];
}
data_type check_type_lvalue(AST_NODE* lvalue)
{
	AST_NODE* p = lvalue;
	int i, j;
	data_type child_type[4];
	i = 0;
	for( p = p->leftChild; p!=NULL; p=p->rightSibling)
	{
		child_type[i++] = check_type[p->nodeType](p);
	}
	for(j = 0 ; j < i ; j++)
	{
		if(child_type[j].type == -1) return check_wrong();
	}
	if( i == 1) return child_type[0];
	else if(i == 2){
		if(child_type[1].star_num <= 0) return check_wrong();
		child_type[1].star_num -= 1;
		return child_type[1];
	}
	else if(i == 4){
		if(child_type[0].star_num == 1 /*&& child_type[0].size >= 0*/ && (child_type[2].type == INT_T || child_type[2].type == CHAR_T) 
			&& child_type[2].star_num == 0){
			child_type[0].star_num -= 1;
			child_type[0].size = -1;
			return child_type[0];
		}
		else return check_wrong();
	}
}
data_type check_type_rvalue(AST_NODE* rvalue)
{
	AST_NODE* p = rvalue;
	int i, j;
	data_type child_type[4];
	i = 0;
	for( p = p->leftChild; p!=NULL; p=p->rightSibling)
	{
		child_type[i++] = check_type[p->nodeType](p);
	}
	for(j = 0 ; j < i ; j++)
	{
		if(child_type[j].type == -1) return check_wrong();
	}
	if(i == 1) return child_type[0];
	else if(i == 2){
		if(child_type[1].type == DOUBLE_OP_T) return check_single(child_type[1].type, child_type[0]);
		else return check_single(child_type[0].type, child_type[1]);
	}
	else if(i == 3){
		if(child_type[0].type == LEFT_PARENTHESE) return child_type[1];
		else if(child_type[1].type == LEFT_PARENTHESE) return child_type[0];
		else return check_double(child_type[1].type, child_type[0], child_type[2]);			
	}
	else if(i == 4){
	}


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
	symtbl_item *id_item = symtbl_query(p->symtbl, (p->content).s_content, 1);
	if(id_item == NULL){
		temp.type = -1;
		return temp;	
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
	return check_type[p->nodeType](p);	
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
	}
	return temp;	
}




/**
	检查单目运算符
*/
data_type check_single(int op_type,data_type op_num)
{
	switch(child_type[0].type){
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
		if(op_num.type != VOID_T || op_num.star_num != 0){
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
			op_num.star_num += 1;
			op_num.size = -1;
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
int check_double(int op_type,data_type op1, data_type op2)
{
	switch(op_type){
	case EQUALITY_SIGN:{
		if(op1.star_num != 0 && op1.size != -1){
			return check_wrong();		
		}
		if(op1.star_num != op2.star_num){
			printf("warning!");
			return op1;	
		}
		else if(op1.star_num == op2.star_num){
			if(op1.type == CHAR_T && op2.TYPE == INT_T){
				printf("warning!");
				return op1;			
			}
			else return op1;
		}
		break;
	}
	case OP:{
		if(op1.star_num != op2.star_num){
			printf("warning!");
			op1.type = INT_T;
			op1.star_num = 0;
			op1.size = -1;
			return op1;
		}
		else if(op1.type != op2.type){
			printf("warning!");
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
				printf("error!");
				return check_wrong();
			}
			else{
				op1.size = -1;
				return op1;			
			}
		}
		else if(op1.type != op2.type && op1.star_num > 0){
			printf("error!");
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
				printf("error!");
				return check_wrong();			
			}
		}
		else if(op1.star_num > 0){
			printf("error!");
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
			printf("error!");
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
