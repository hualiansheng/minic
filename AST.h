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
#define ARRAY_VAR 1010
#define SCALAR_VAR 1011
#define FUNCTION_DEF 1012
#define FUNCTION_HDR 1013
#define PARM_TYPE_LIST 1014
#define PARM_LIST 1015
#define PARM_DECL 1016
#define FUNCTION_BODY 1017
#define INTERNAL_DECLS 1018
#define STATEMENT_LIST 1019
#define STATEMENT 1020
#define COMPOUNDSTMT 1021
#define NULLSTMT 1022
#define EXPRESSION_STMT 1023
#define IFSTMT 1024
#define FOR_STMT 1025
#define WHILE_STMT 1026
#define RETURN_STMT 1027
#define EXPRESSION 1028
#define ASSIGNMENT_EXPRESSION 1029
#define LVALUE 1030
#define RVALUE 1031
#define OP 1032
#define CONSTANT 1033
#define ARGUMENT_LIST 1034
#define EXTERN_T 1035
#define REGISTER_T 1036
#define VOID_T 1037
#define INT_T 1038
#define CHAR_T 1039
#define IF_T 1041
#define ELSE_T 1042
#define FOR_T 1043
#define WHILE_T 1044
#define RETURN_T 1045
#define BOOLEAN_OP_T 1046
#define REL_OP_T 1047
#define DOUBLE_OP_T 1048
#define ICONSTANT_T 1049
#define CHAR_CONSTANT_T 1051
#define STRING_CONSTANT_T 1052
#define SEMICOLON 1053
#define COMMA 1054
#define STAR 1055
#define LEFT_SQUARE_BRACKET 1056
#define RIGHT_SQUARE_BRACKET 1057
#define LEFT_PARENTHESE 1058
#define RIGHT_PARENTHESE 1059
#define LEFT_BRACE 1060
#define RIGHT_BRACE 1061
#define EQUALITY_SIGN 1062
#define MINUS_SIGN 1063
#define PLUS_SIGN 1064
#define MULTIPLY_SIGN 1065
#define POSITIVE_SIGN 1066
#define NEGATIVE_SIGN 1067
#define NOT_SIGN 1068
#define ADDRESS_SIGN 1069
#define IDENT_T 1071
#define EPSILON 1074
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
void AST_addLeftChild(AST_NODE* root, int child_nodeType, AST_NODE* child);
#endif
