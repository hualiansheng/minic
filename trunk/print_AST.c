#include "AST.h"
#include <stdio.h>
char name[][30] = {"PROGRAM","EXTERNAL_DECLS","DECLARATION","FUNCTION_LIST","TYPE_NAME","VAR_LIST","VAR_ITEM","ARRAY_VAR","SCALAR_VAR","FUNCTION_DEF","FUNCTION_HDR","PARM_TYPE_LIST","PARM_LIST","PARM_DECL","FUNCTION_BODY","INTERNAL_DECLS","STATEMENT_LIST","STATEMENT","COMPOUNDSTMT","NULLSTMT","EXPRESSION_STMT","IFSTMT","FOR_STMT","WHILE_STMT","RETURN_STMT","EXPRESSION","ASSIGNMENT_EXPRESSION","LVALUE","RVALUE","OP","CONSTANT","ARGUMENT_LIST","EXTERN_T","REGISTER_T","VOID_T","INT_T","CHAR_T","IF_T","ELSE_T","FOR_T","WHILE_T","RETURN_T","BOOLEAN_OP_T","REL_OP_T","DOUBLE_OP_T","ICONSTANT_T","CHAR_CONSTANT_T","STRING_CONSTANT_T","SEMICOLON","COMMA","STAR","LEFT_SQUARE_BRACKET","RIGHT_SQUARE_BRACKET","LEFT_PARENTHESE","RIGHT_PARENTHESE","LEFT_BRACE","RIGHT_BRACE","EQUALITY_SIGN","MINUS_SIGN","PLUS_SIGN","MULTIPLY_SIGN","POSITIVE_SIGN","NEGATIVE_SIGN","NOT_SIGN","ADDRESS_SIGN","IDENT_T","EPSILON"};
// use this function to print the AST tree (with terminal/non-terminal names)
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


