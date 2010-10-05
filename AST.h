#ifndef _AST_H_
#define _AST_H_
//Node macro
#define PROGRAM 1001
#define EXTERNAL_DECLS 1002
#define DECLARATION 1003
#define FUNCTION_LIST 1004
#define TYPE_NAME 1005
#define VAR_LIST 1006
#define VAR_ITEM 1007
#define ARRAY_VAR 1008
#define SCALAR_VAR 1009
#define FUNCTION_DEF 1010
#define FUNCTION_HDR 1011
#define PARM_TYPE_LIST 1012
#define PARM_LIST 1013
#define PARM_DECL 1014
#define FUNCTION_BODY 1015
#define INTERNAL_DECLS 1016
#define STATEMENT_LIST 1017
#define STATEMENT 1018
#define COMPOUNDSTMT 1019
#define NULLSTMT 1020
#define EXPRESSION_STMT 1021
#define IFSTMT 1022
#define FOR_STMT 1023
#define WHILE_STMT 1024
#define RETURN_STMT 1025
#define EXPRESSION 1026
#define ASSIGNMENT_EXPRESSION 1027
#define LVALUE 1028
#define RVALUE 1029
#define OP 1030
#define CONSTANT 1031
#define ARGUMENT_LIST 1032
#define EXTERN_T 1033
#define REGISTER_T 1034
#define VOID_T 1035
#define INT_T 1036
#define CHAR_T 1037
#define IF_T 1038
#define ELSE_T 1039
#define FOR_T 1040
#define WHILE_T 1041
#define RETURN_T 1042
#define BOOLEAN_OP_T 1043
#define REL_OP_T 1044
#define DOUBLE_OP_T 1045
#define ICONSTANT_T 1046
#define CHAR_CONSTANT_T 1047
#define STRING_CONSTANT_T 1048
#define SEMICOLON 1049
#define COMMA 1050
#define STAR 1051
#define LEFT_SQUARE_BRACKET 1052
#define RIGHT_SQUARE_BRACKET 1053
#define LEFT_PARENTHESE 1054
#define RIGHT_PARENTHESE 1055
#define LEFT_BRACE 1056
#define RIGHT_BRACE 1057
#define EQUALITY_SIGN 1058
#define MINUS_SIGN 1059
#define PLUS_SIGN 1060
#define MULTIPLY_SIGN 1061
#define POSITIVE_SIGN 1062
#define NEGATIVE_SIGN 1063
#define NOT_SIGN 1064
#define ADDRESS_SIGN 1065
#define IDENT_T 1066
#define EPSILON 1067
union node_content
{
	char c_content;
	char* s_content;
	int i_content;
};
typedef struct AST_NODE{
  	int nodeType;//The type of node, defined above
  	int nodeLevel;//Level of the node
	union node_content content;
  	struct AST_NODE * father;//Father node
  	struct AST_NODE * leftChild;//Left child node
  	struct AST_NODE * rightSibling;//Right sibling node
} AST_NODE;

AST_NODE* AST_new_Node();
void AST_addChild(AST_NODE* root, int child_nodeType, AST_NODE* child);
#endif
