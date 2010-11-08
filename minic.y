%{
#include <memory.h>
#include <stdio.h>
#include <assert.h>
#include "attr.h"
#include "AST.h"
#define BOOL_AND 1
#define BOOL_OR 2
#define REL_EQ 3
#define REL_NEQ 4
#define REL_GRT 5
#define REL_SML 6
#define REL_GRE 7
#define REL_SME 8
#define REL_DAD 9
#define REL_DSU 0
//symtable support: scope number
int scope_number;
void make_error(AST_NODE* root,char* err_msg, int first_line, int first_column, int last_line, int last_column);
int yylex (void);
void yyerror (char const*);
AST_NODE* tree_root;
AST_NODE* root;
extern FILE* yyin;
//error number, decleared in main.c; handled by yyerror();
extern int error_number;
%}

%union {
	iattr int_attr;
	cattr char_attr;
	sattr string_attr;
	AST_NODE* ptr;
}

%token <string_attr> EXTERN
%token <string_attr> REGISTER
%token <string_attr> VOID
%token <string_attr> INT
%token <string_attr> CHAR
%token <string_attr> IDENT
%token <string_attr> IF
%token <string_attr> ELSE
%token <string_attr> FOR
%token <string_attr> WHILE
%token <string_attr> RETURN
%token <int_attr> BOOLEAN_OP
%token <int_attr> REL_OP
%token <int_attr> DOUBLE_OP
%token <int_attr> ICONSTANT
%token <char_attr> CHAR_CONSTANT
%token <string_attr> STRING_CONSTANT
%token <char_attr> '{' '}' '(' ')' '[' ']' ';' ',' '.' '+' '-' '*' '!' '&' '='

%type <ptr> program
%type <ptr> external_decls
%type <ptr> declaration
%type <ptr> function_list
%type <ptr> type_name
%type <ptr> var_list
%type <ptr> var_item
%type <ptr> array_var
%type <ptr> scalar_var
%type <ptr> function_def
%type <ptr> function_hdr
%type <ptr> parm_type_list
%type <ptr> parm_list
%type <ptr> parm_decl
%type <ptr> function_body
%type <ptr> internal_decls
%type <ptr> statement_list
%type <ptr> statement
%type <ptr> compoundstmt
%type <ptr> nullstmt
%type <ptr> expression_stmt
%type <ptr> ifstmt
%type <ptr> for_stmt
%type <ptr> while_stmt
%type <ptr> return_stmt
%type <ptr> expression
%type <ptr> assignment_expression
%type <ptr> lvalue
%type <ptr> rvalue
%type <ptr> op
%type <ptr> constant
%type <ptr> argument_list


%right '='
%left '+' '-'
%left '*'

%left BOOLEAN_OP REL_OP
%right DOUBLE
%right DOUBLE_OP
%right NOT
%right ADDRESS
%right UMINUS UPLUS
%right UASTAR

%nonassoc UIF
%nonassoc ELSE
%%
program		:	external_decls {root = AST_new_Node();
					root -> nodeType = PROGRAM;
					root -> nodeLevel = 0;
					AST_addChild(root,EXTERNAL_DECLS,$1);
					tree_root = root;
					}

		;
external_decls	:	declaration external_decls
			{
			root = AST_new_Node();
			root -> nodeType = EXTERNAL_DECLS;
			AST_addChild(root,DECLARATION,$1);
			AST_addChild(root,EXTERNAL_DECLS,$2);
			$$ = root;			
			} 
		| 	function_list
			{
			root = AST_new_Node();
			root -> nodeType = EXTERNAL_DECLS;
			AST_addChild(root,FUNCTION_LIST,$1);
			$$ = root;			
			}
		|	error external_decls
			{
				root = AST_new_Node();
				root -> nodeType = RVALUE;
				make_error(root,"bad declaration",@1.first_line,@1.first_column,@1.last_line,@1.last_column);
				AST_addChild(root,EXTERNAL_DECLS,$2);
				$$ = root;
			}
		;
declaration	:	EXTERN type_name var_list ';'
			{
			root = AST_new_Node();
			root -> nodeType = DECLARATION;
			AST_addChild(root,EXTERN_T,$1.ptr);
			AST_addChild(root,TYPE_NAME,$2);
			AST_addChild(root,VAR_LIST,$3);
			AST_addChild(root,SEMICOLON,$4.ptr);
			$$ = root;
			}
		| 	REGISTER type_name var_list ';'
			{
			root = AST_new_Node();
			root -> nodeType = DECLARATION;
			AST_addChild(root,REGISTER_T,$1.ptr);
			AST_addChild(root,TYPE_NAME,$2);
			AST_addChild(root,VAR_LIST,$3);
			AST_addChild(root,SEMICOLON,$4.ptr);
			$$ = root;
			}
		| 	type_name var_list ';'
			{
			root = AST_new_Node();
			root -> nodeType = DECLARATION;
			AST_addChild(root,TYPE_NAME,$1);
			AST_addChild(root,VAR_LIST,$2);
			AST_addChild(root,SEMICOLON,$3.ptr);
			$$ = root;
			}		
		;
function_list	:	function_list function_def
			{
			root = AST_new_Node();
			root -> nodeType = FUNCTION_LIST;
			AST_addChild(root,FUNCTION_LIST,$1);
			AST_addChild(root,FUNCTION_DEF,$2);
			$$ = root;
			}

		|	function_def
			{
			root = AST_new_Node();
			root -> nodeType = FUNCTION_LIST;
			AST_addChild(root,FUNCTION_DEF,$1);
			$$ = root;
			}
		;
type_name	:	VOID
			{
			root = AST_new_Node();
			root -> nodeType = TYPE_NAME;
			AST_addChild(root,VOID_T,$1.ptr);
			$$ = root;
			} 
		|	INT 
			{
			root = AST_new_Node();
			root -> nodeType = TYPE_NAME;
			AST_addChild(root,INT_T,$1.ptr);
			$$ = root;
			} 
		|	CHAR
			{
			root = AST_new_Node();
			root -> nodeType = TYPE_NAME;
			AST_addChild(root,CHAR_T,$1.ptr);
			$$ = root;
			} 
		;
var_list	:	var_list ',' var_item 
			{
			root = AST_new_Node();
			root -> nodeType = VAR_LIST;
			AST_addChild(root,VAR_LIST,$1);
			AST_addChild(root,COMMA,$2.ptr);
			AST_addChild(root,VAR_ITEM,$3);
			$$ = root;
			} 
		|	var_item
			{
			root = AST_new_Node();
			root -> nodeType = VAR_LIST;
			AST_addChild(root,VAR_ITEM,$1);
			$$ = root;
			} 
		;
var_item	:	array_var
			{
			root = AST_new_Node();
			root -> nodeType = VAR_ITEM;
			AST_addChild(root,ARRAY_VAR,$1);
			$$ = root;
			}  
		| 	scalar_var
			{
			root = AST_new_Node();
			root -> nodeType = VAR_ITEM;
			AST_addChild(root,SCALAR_VAR,$1);
			$$ = root;
			}
		| 	'*' scalar_var
			{
			root = AST_new_Node();
			root -> nodeType = VAR_ITEM;
			AST_addChild(root,STAR,$1.ptr);
			AST_addChild(root,SCALAR_VAR,$2);
			$$ = root;
			}
		;
array_var	:	IDENT '[' ICONSTANT ']'
			{
			root = AST_new_Node();
			root -> nodeType = ARRAY_VAR;
			AST_addChild(root,IDENT_T,$1.ptr);
			AST_addChild(root,LEFT_SQUARE_BRACKET,$2.ptr);
			AST_addChild(root,ICONSTANT_T,$3.ptr);
			AST_addChild(root,RIGHT_SQUARE_BRACKET,$4.ptr);
			$$ = root;
			}
		;
scalar_var	:	IDENT
			{
			root = AST_new_Node();
			root -> nodeType = SCALAR_VAR;
			AST_addChild(root,IDENT_T,$1.ptr);
			$$ = root;
			}
			 
		| 	IDENT '(' parm_type_list ')'
			{
			root = AST_new_Node();
			root -> nodeType = SCALAR_VAR;
			AST_addChild(root,IDENT_T,$1.ptr);
			AST_addChild(root,LEFT_PARENTHESE,$2.ptr);
			AST_addChild(root,PARM_TYPE_LIST,$3);
			AST_addChild(root,RIGHT_PARENTHESE,$4.ptr);
			$$ = root;
			}
		;
function_def	:	function_hdr '{' function_body '}'
			{
			root = AST_new_Node();
			root -> nodeType = FUNCTION_DEF;
			AST_addChild(root,FUNCTION_HDR,$1);
			AST_addChild(root,LEFT_BRACE,$2.ptr);
			AST_addChild(root,FUNCTION_BODY,$3);
			AST_addChild(root,RIGHT_BRACE,$4.ptr);
			$$ = root;
			}			
		;
function_hdr	:	type_name IDENT '(' parm_type_list ')'
			{
			root = AST_new_Node();
			root -> nodeType = FUNCTION_HDR;
			AST_addChild(root,TYPE_NAME,$1);
			AST_addChild(root,IDENT_T,$2.ptr);
			AST_addChild(root,LEFT_PARENTHESE,$3.ptr);
			AST_addChild(root,PARM_TYPE_LIST,$4);
			AST_addChild(root,RIGHT_PARENTHESE,$5.ptr);
			$$ = root;
			}
		|	type_name '*' IDENT '(' parm_type_list ')'
			{
			root = AST_new_Node();
			root -> nodeType = FUNCTION_HDR;
			AST_addChild(root,TYPE_NAME,$1);
			AST_addChild(root,STAR,$2.ptr);
			AST_addChild(root,IDENT_T,$3.ptr);
			AST_addChild(root,LEFT_PARENTHESE,$4.ptr);
			AST_addChild(root,PARM_TYPE_LIST,$5);
			AST_addChild(root,RIGHT_PARENTHESE,$6.ptr);
			$$ = root;
			}

		|	IDENT '(' parm_type_list ')'
			{
			root = AST_new_Node();
			root -> nodeType = FUNCTION_HDR;
			AST_addChild(root,IDENT_T,$1.ptr);
			AST_addChild(root,LEFT_PARENTHESE,$2.ptr);
			AST_addChild(root,PARM_TYPE_LIST,$3);
			AST_addChild(root,RIGHT_PARENTHESE,$4.ptr);
			$$ = root;
			}
		;
parm_type_list	:	
			{
			root = AST_new_Node();
			root -> nodeType = PARM_TYPE_LIST;
			AST_addChild(root,EPSILON,AST_new_Node());
			$$ = root;
			}	
		|	VOID
			{
			root = AST_new_Node();
			root -> nodeType = PARM_TYPE_LIST;
			AST_addChild(root,VOID_T,$1.ptr);
			$$ = root;
			}
			
		| 	parm_list
			{
			root = AST_new_Node();
			root -> nodeType = PARM_TYPE_LIST;
			AST_addChild(root,PARM_LIST,$1);
			$$ = root;
			}
		;
parm_list	:	parm_list ',' parm_decl
			{
			root = AST_new_Node();
			root -> nodeType = PARM_LIST;
			AST_addChild(root,PARM_LIST,$1);
			AST_addChild(root,COMMA,$2.ptr);
			AST_addChild(root,PARM_DECL,$3);
			$$ = root;
			} 
		| 	parm_decl
			{
			root = AST_new_Node();
			root -> nodeType = PARM_LIST;
			AST_addChild(root,PARM_DECL,$1);
			$$ = root;
			}
		;
parm_decl	:	type_name IDENT
			{
			root = AST_new_Node();
			root -> nodeType = PARM_DECL;
			AST_addChild(root,TYPE_NAME,$1);
			AST_addChild(root,IDENT_T,$2.ptr);
			$$ = root;
			}
		| 	type_name '*' IDENT
			{
			root = AST_new_Node();
			root -> nodeType = PARM_DECL;
			AST_addChild(root,TYPE_NAME,$1);
			AST_addChild(root,STAR,$2.ptr);
			AST_addChild(root,IDENT_T,$3.ptr);
			$$ = root;
			}
		;
function_body	:	internal_decls statement_list
			{
			root = AST_new_Node();
			root -> nodeType = FUNCTION_BODY;
			AST_addChild(root,INTERNAL_DECLS,$1);
			AST_addChild(root,STATEMENT_LIST,$2);
			$$ = root;
			}
		;
internal_decls	:	
			{
			root = AST_new_Node();
			root -> nodeType = INTERNAL_DECLS;
			AST_addChild(root,EPSILON,AST_new_Node());
			$$ = root;
			}
		| 	declaration internal_decls
			{
			root = AST_new_Node();
			root -> nodeType = INTERNAL_DECLS;
			AST_addChild(root,DECLARATION,$1);
			AST_addChild(root,INTERNAL_DECLS,$2);
			$$ = root;
			}
 
		;
statement_list	:	
			{
			root = AST_new_Node();
			root -> nodeType = STATEMENT_LIST;
			AST_addChild(root,EPSILON,AST_new_Node());
			$$ = root;
			}
		| 	statement statement_list 
			{
			root = AST_new_Node();
			root -> nodeType = STATEMENT_LIST;
			AST_addChild(root,STATEMENT,$1);
			AST_addChild(root,STATEMENT_LIST,$2);
			$$ = root;
			}
		;
statement	:	compoundstmt 
			{
			root = AST_new_Node();
			root -> nodeType = STATEMENT;
			AST_addChild(root,COMPOUNDSTMT,$1);
			//symtable support: scope number
			$1->content.i_content = scope_number ++;
			$$ = root;
			}
		| 	nullstmt 
			{
			root = AST_new_Node();
			root -> nodeType = STATEMENT;
			AST_addChild(root,NULLSTMT,$1);
			$$ = root;
			}
		| 	expression_stmt 
			{
			root = AST_new_Node();
			root -> nodeType = STATEMENT;
			AST_addChild(root,EXPRESSION_STMT,$1);
			$$ = root;
			}
		| 	ifstmt
			{
			root = AST_new_Node();
			root -> nodeType = STATEMENT;
			AST_addChild(root,IFSTMT,$1);
			$$ = root;
			}
		| 	for_stmt 
			{
			root = AST_new_Node();
			root -> nodeType = STATEMENT;
			AST_addChild(root,FOR_STMT,$1);
			$$ = root;
			}
		| 	while_stmt
			{
			root = AST_new_Node();
			root -> nodeType = STATEMENT;
			AST_addChild(root,WHILE_STMT,$1);
			$$ = root;
			}
		| 	return_stmt
			{
			root = AST_new_Node();
			root -> nodeType = STATEMENT;
			AST_addChild(root,RETURN_STMT,$1);
			$$ = root;
			}
		;
compoundstmt	:	'{' internal_decls statement_list '}'
			{
			root = AST_new_Node();
			root -> nodeType = COMPOUNDSTMT;
			AST_addChild(root,LEFT_BRACE,$1.ptr);
			AST_addChild(root,INTERNAL_DECLS,$2);
			AST_addChild(root,STATEMENT_LIST,$3);
			AST_addChild(root,RIGHT_BRACE,$4.ptr);
			$$ = root;
			}
		;
nullstmt	:	';'
			{
			root = AST_new_Node();
			root -> nodeType = NULLSTMT;
			AST_addChild(root,SEMICOLON,$1.ptr);
			$$ = root;
			}
 
		;
expression_stmt	:	expression ';'
			{
			root = AST_new_Node();
			root -> nodeType = EXPRESSION_STMT;
			AST_addChild(root,EXPRESSION,$1);
			AST_addChild(root,SEMICOLON,$2.ptr);
			$$ = root;
			}
 
		;
ifstmt		:	IF '(' expression ')' statement %prec UIF
			{
			root = AST_new_Node();
			root -> nodeType = IFSTMT;
			AST_addChild(root,IF_T,$1.ptr);
			AST_addChild(root,LEFT_PARENTHESE,$2.ptr);
			AST_addChild(root,EXPRESSION,$3);
			AST_addChild(root,RIGHT_PARENTHESE,$4.ptr);
			AST_addChild(root,STATEMENT,$5);
			$$ = root;
			}
 
		|	IF '(' expression ')' statement ELSE statement
			{
			root = AST_new_Node();
			root -> nodeType = IFSTMT;
			AST_addChild(root,IF_T,$1.ptr);
			AST_addChild(root,LEFT_PARENTHESE,$2.ptr);
			AST_addChild(root,EXPRESSION,$3);
			AST_addChild(root,RIGHT_PARENTHESE,$4.ptr);
			AST_addChild(root,STATEMENT,$5);
			AST_addChild(root,ELSE_T,$6.ptr);
			AST_addChild(root,STATEMENT,$7);
			$$ = root;
			}
		; 
for_stmt	:	FOR '(' expression ';' expression ';' expression ')' statement
			{
			root = AST_new_Node();
			root -> nodeType = FOR_STMT;
			AST_addChild(root,FOR_T,$1.ptr);
			AST_addChild(root,LEFT_PARENTHESE,$2.ptr);
			AST_addChild(root,EXPRESSION,$3);
			AST_addChild(root,SEMICOLON,$4.ptr);
			AST_addChild(root,EXPRESSION,$5);
			AST_addChild(root,SEMICOLON,$6.ptr);
			AST_addChild(root,EXPRESSION,$7);
			AST_addChild(root,RIGHT_PARENTHESE,$8.ptr);			
			AST_addChild(root,STATEMENT,$9);
			$$ = root;
			}
		;
while_stmt	:	WHILE '(' expression ')' statement
			{
			root = AST_new_Node();
			root -> nodeType = WHILE_STMT;
			AST_addChild(root,WHILE_T,$1.ptr);
			AST_addChild(root,LEFT_PARENTHESE,$2.ptr);
			AST_addChild(root,EXPRESSION,$3);
			AST_addChild(root,RIGHT_PARENTHESE,$4.ptr);
			AST_addChild(root,STATEMENT,$5);
			$$ = root;
			}
		;
return_stmt	:	RETURN expression ';'
			{
			root = AST_new_Node();
			root -> nodeType = RETURN_STMT;
			AST_addChild(root,RETURN_T,$1.ptr);
			AST_addChild(root,EXPRESSION,$2);
			AST_addChild(root,SEMICOLON,$3.ptr);
			$$ = root;
			} 
		| 	RETURN ';'
			{
			root = AST_new_Node();
			root -> nodeType = RETURN_STMT;
			AST_addChild(root,RETURN_T,$1.ptr);
			AST_addChild(root,SEMICOLON,$2.ptr);
			$$ = root;
			} 
		;
expression	:	rvalue 
			{
			root = AST_new_Node();
			root -> nodeType = EXPRESSION;
			AST_addChild(root,RVALUE,$1);
			$$ = root;
			} 
		|	assignment_expression
			{
			root = AST_new_Node();
			root -> nodeType = EXPRESSION;
			AST_addChild(root,ASSIGNMENT_EXPRESSION,$1);
			$$ = root;
			} 
		;
assignment_expression
		:	lvalue '=' assignment_expression
			{
			root = AST_new_Node();
			root -> nodeType = ASSIGNMENT_EXPRESSION;
			AST_addChild(root,LVALUE,$1);
			AST_addChild(root,EQUALITY_SIGN,$2.ptr);
			AST_addChild(root,ASSIGNMENT_EXPRESSION,$3);
			$$ = root;
			} 

		|	lvalue '=' rvalue
			{
			root = AST_new_Node();
			root -> nodeType = ASSIGNMENT_EXPRESSION;
			AST_addChild(root,LVALUE,$1);
			AST_addChild(root,EQUALITY_SIGN,$2.ptr);
			AST_addChild(root,RVALUE,$3);
			$$ = root;
			} 
		;
	
lvalue		:	'*' rvalue 
			{
			root = AST_new_Node();
			root -> nodeType = LVALUE;
			AST_addChild(root,STAR,$1.ptr);
			AST_addChild(root,RVALUE,$2);
			$$ = root;
			} 
		|	IDENT
			{
			root = AST_new_Node();
			root -> nodeType = LVALUE;
			AST_addChild(root,IDENT_T,$1.ptr);
			$$ = root;
			} 
		|	IDENT '[' expression ']'
			{
			root = AST_new_Node();
			root -> nodeType = LVALUE;
			AST_addChild(root,IDENT_T,$1.ptr);
			AST_addChild(root,LEFT_SQUARE_BRACKET,$2.ptr);
			AST_addChild(root,EXPRESSION,$3);
			AST_addChild(root,RIGHT_SQUARE_BRACKET,$4.ptr);
			$$ = root;
			} 
		|	error
			{
				root = AST_new_Node();
				root -> nodeType = RVALUE;
				make_error(root,"bad lvalue",@1.first_line,@1.first_column,@1.last_line,@1.last_column);
				$$ = root;
			}
		;
rvalue		:	lvalue %prec '-'
			{
			root = AST_new_Node();
			root -> nodeType = RVALUE;
			AST_addChild(root,LVALUE,$1);
			$$ = root;
			} 
		|	rvalue '+' rvalue
			{
			root = AST_new_Node();
			root -> nodeType = RVALUE;
			AST_addChild(root,RVALUE,$1);
			AST_addChild(root,PLUS_SIGN,$2.ptr);
			AST_addChild(root,RVALUE,$3);
			$$ = root;
			}
		|	rvalue '-' rvalue
			{
			root = AST_new_Node();
			root -> nodeType = RVALUE;
			AST_addChild(root,RVALUE,$1);
			AST_addChild(root,MINUS_SIGN,$2.ptr);
			AST_addChild(root,RVALUE,$3);
			$$ = root;
			} 
		|	rvalue '*' rvalue
			{
			root = AST_new_Node();
			root -> nodeType = RVALUE;
			AST_addChild(root,RVALUE,$1);
			AST_addChild(root,MULTIPLY_SIGN,$2.ptr);
			AST_addChild(root,RVALUE,$3);
			$$ = root;
			} 
		|	rvalue op  rvalue %prec BOOLEAN_OP
			{
			root = AST_new_Node();
			root -> nodeType = RVALUE;
			AST_addChild(root,RVALUE,$1);
			AST_addChild(root,OP,$2);
			AST_addChild(root,RVALUE,$3);
			$$ = root;
			} 
		|	'(' rvalue ')'
			{
			root = AST_new_Node();
			root -> nodeType = RVALUE;
			AST_addChild(root,LEFT_PARENTHESE,$1.ptr);
			AST_addChild(root,RVALUE,$2);
			AST_addChild(root,RIGHT_PARENTHESE,$3.ptr);
			$$ = root;
			} 
		|	'+' rvalue %prec UPLUS
			{
			root = AST_new_Node();
			root -> nodeType = RVALUE;
			AST_addChild(root,POSITIVE_SIGN,$1.ptr);
			AST_addChild(root,RVALUE,$2);
			$$ = root;
			} 
		|	'-' rvalue %prec UMINUS
			{
			root = AST_new_Node();
			root -> nodeType = RVALUE;
			AST_addChild(root,NEGATIVE_SIGN,$1.ptr);
			AST_addChild(root,RVALUE,$2);
			$$ = root;
			} 
		|	'!' rvalue %prec NOT
			{
			root = AST_new_Node();
			root -> nodeType = RVALUE;
			AST_addChild(root,NOT_SIGN,$1.ptr);
			AST_addChild(root,RVALUE,$2);
			$$ = root;
			} 
		|	'&' lvalue %prec ADDRESS
			{
			root = AST_new_Node();
			root -> nodeType = RVALUE;
			AST_addChild(root,ADDRESS_SIGN,$1.ptr);
			AST_addChild(root,RVALUE,$2);
			$$ = root;
			} 
		|	DOUBLE_OP lvalue %prec DOUBLE
			{
			root = AST_new_Node();
			root -> nodeType = RVALUE;
			AST_addChild(root,DOUBLE_OP_T,$1.ptr);
			AST_addChild(root,LVALUE,$2);
			$$ = root;
			} 
		|	lvalue DOUBLE_OP %prec DOUBLE
			{
			root = AST_new_Node();
			root -> nodeType = RVALUE;
			AST_addChild(root,LVALUE,$1);
			AST_addChild(root,DOUBLE_OP_T,$2.ptr);
			$$ = root;
			} 
		|	constant
			{
			root = AST_new_Node();
			root -> nodeType = RVALUE;
			AST_addChild(root,CONSTANT,$1);
			$$ = root;
			} 

		|	IDENT '(' argument_list ')'
			{
			root = AST_new_Node();
			root -> nodeType = RVALUE;
			AST_addChild(root,IDENT_T,$1.ptr);
			AST_addChild(root,LEFT_PARENTHESE,$2.ptr);
			AST_addChild(root,ARGUMENT_LIST,$3);
			AST_addChild(root,RIGHT_PARENTHESE,$4.ptr);
			$$ = root;
			} 
		|	IDENT '(' ')'
			{
			root = AST_new_Node();
			root -> nodeType = RVALUE;
			AST_addChild(root,IDENT_T,$1.ptr);
			AST_addChild(root,LEFT_PARENTHESE,$2.ptr);
			AST_addChild(root,RIGHT_PARENTHESE,$3.ptr);
			$$ = root;
			}
		;
op		:	BOOLEAN_OP 
			{
			root = AST_new_Node();
			root -> nodeType = OP;
			AST_addChild(root,BOOLEAN_OP_T,$1.ptr);
			$$ = root;
			}
		| 	REL_OP
			{
			root = AST_new_Node();
			root -> nodeType = OP;
			AST_addChild(root,REL_OP_T,$1.ptr);
			$$ = root;
			}
		;
constant	:	ICONSTANT
			{
			root = AST_new_Node();
			root -> nodeType = CONSTANT;
			AST_addChild(root,ICONSTANT_T,$1.ptr);
			$$ = root;
			}
		| 	CHAR_CONSTANT 
			{
			root = AST_new_Node();
			root -> nodeType = CONSTANT;
			AST_addChild(root,CHAR_CONSTANT_T,$1.ptr);
			$$ = root;
			}
		| 	STRING_CONSTANT
			{
			root = AST_new_Node();
			root -> nodeType = CONSTANT;
			AST_addChild(root,STRING_CONSTANT_T,$1.ptr);
			$$ = root;
			}
		;
argument_list	:	argument_list ',' expression 
			{
			root = AST_new_Node();
			root -> nodeType = ARGUMENT_LIST;
			AST_addChild(root,ARGUMENT_LIST,$1);
			AST_addChild(root,COMMA,$2.ptr);
			AST_addChild(root,EXPRESSION,$3);
			$$ = root;
			}
		| 	expression
			{
			root = AST_new_Node();
			root -> nodeType = ARGUMENT_LIST;
			AST_addChild(root,EXPRESSION,$1);
			$$ = root;
			}
		;

%%
void yyerror(char const * s)
{
	fprintf(stderr, "%s\n",s);
	return;
}

void make_error(AST_NODE* root,char* err_msg, int first_line, int first_column, int last_line, int last_column)
{
	AST_addChild(root,ERROR,AST_new_Node());
	fprintf(stderr, "Parsing error: line %d.%d-%d.%d: %s\n ",first_line,first_column,last_line,last_column,err_msg);
	error_number++;
}
