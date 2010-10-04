#ifndef _AST_H_
//Node macro
#define PROGRAM 1001
#define EXTERNAL_DECLS 1002
#define DECLARATION 1003
#define FUNCTION_LIST 1004
#define MODIFIER 1005
#define TYPE_NAME 1006
#define VAR_LIST 1007
#define VAR_ITEM 1008
#define ARRAY_VAR 1009
#define SCALAR_VAR 1010
#define FUNCTION_DEF 1011
#define FUNCTION_HDR 1012
#define PARM_TYPE_LIST 1013
#define PARM_LIST 1014
#define PARM_DECL 1015
#define FUNCTION_BODY 1016
#define INTERNAL_DECLS 1017
#define STATEMENT_LIST 1018
#define STATEMENT 1019
#define COMPOUNDSTMT 1020
#define NULLSTMT 1021
#define EXPRESSION_STMT 1022
#define IFSTMT 1023
#define FOR_STMT 1024
#define WHILE_STMT 1025
#define RETURN_STMT 1026
#define EXPRESSION 1027
#define ASSIGNMENT_EXPR 1028
#define BINARY_EXPR 1029
#define BINARY_OP 1030
#define BOOLEAN_OP 1031
#define REL_OP 1032
#define ARITH_OP 1033
#define UNARY_EXPR 1034
#define UNARY_OP 1035
#define POSTFIX_EXPR 1036
#define PRIMARY_EXPR 1037
#define CONSTANT 1038
#define ARGUMENT_LIST 1039
#define ICONSTANT 1040
#define CHAR_CONSTANT 1041
#define STRING_CONSTANT 1042
#define epsilon 1043
#endif _AST_H_

struct AST_NODE{
  int nodeType;//The type of node, defined above
  char* nodeContent;//The content of the node, int char or string
  int scopeIndex;//The scope of an identifier belongs, if the node represents an identifier
  int nodeLevel;//Level of the node
  struct AST_NODE * father;//Father node
  struct AST_NODE * leftChild;//Left child node
  struct AST_NODE * rightSibling;//Right sibling node
};
