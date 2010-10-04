#ifndef _AST_H_
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
#define ASSIGNMENT_EXPR 1029
#define LVALUE 1030
#define RVALUE 1031
#define OP 1032
#define CONSTANT 1033
#define ARGUMENT_LIST 1034
#define EXTERN 1035
#define REGISTER 1036
#define VOID 1037
#define INT 1038
#define CHAR 1039
#define IDENT 1040
#define IF 1041
#define ELSE 1042
#define FOR 1043
#define WHILE 1044
#define RETURN 1045
#define BOOLEAN_OP 1046
#define REL_OP 1047
#define DOUBLE_OP 1048
#define ICONSTANT 1049
#define FCONSTANT 1050
#define CHAR_CONSTANT 1051
#define STRING_CONSTANT 1052
#define SEMICOLON 1053
#define COMMA 1054
#define STAR 1055
#define LEFT_SQUARE_BRACKET 1056
#define RIGHT_SQUARE_BRACKET 1057
#define LEFT_PARENTHESES 1058
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
#define DOUBLE_OP 1070
#define IDENT 1071
#define BOOLEAN_OP 1072
#define REL_OP 1073
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
