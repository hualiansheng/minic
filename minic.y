%{
#include <memory.h>
int yylex (void);
void yyerror (char const*);

%}

%glr-parser
%token EXTERN
%token REGISTER
%token VOID
%token INT
%token CHAR
%token IDENT
%token IF
%token ELSE
%token FOR
%token WHILE
%token RETURN
%token BOOLEAN_OP
%token REL_OP
%token DOUBLE_OP
%token ICONSTANT
%token FCONSTANT
%token CHAR_CONSTANT
%token STRING_CONSTANT



%right '='
%left '+' '-'
%left '*'

%left UMINUS UPLUS
%left UASTAR
%%
program		:	external_decls {printf("ok");}
		 ;
external_decls	:	declaration external_decls 
	| function_list
	;
declaration	:	EXTERN type_name var_list ';'
			|	REGISTER type_name var_list ';'
			|   type_name var_list ';'
			;
function_list	:	function_list function_def 
			  | function_def
			  ;
//modifier	:	
//	| 	EXTERN 
//	| 	REGISTER;
type_name	:	VOID | INT | CHAR;
var_list	:	var_list ',' var_item 
	| 	var_item;
var_item	:	array_var 
	| 	scalar_var 
	| 	'*' scalar_var;
array_var	:	IDENT '[' ICONSTANT ']';
scalar_var	:	IDENT 
	| 	IDENT '(' parm_type_list ')';
function_def	:	function_hdr '{' function_body '}';
function_hdr	:	type_name IDENT '(' parm_type_list ')'
	|	type_name '*' IDENT '(' parm_type_list ')'
	|	IDENT '(' parm_type_list ')';
parm_type_list	:	VOID 
	| 	parm_list;
parm_list	:	parm_list  parm_decl 
	| 	parm_decl;
parm_decl	:	type_name IDENT
	| 	type_name '*' IDENT;
function_body	:	internal_decls statement_list;
internal_decls	:	
	| 	declaration internal_decls ;
statement_list	:	
	| 	statement statement_list ;
statement	:	compoundstmt 
	| 	nullstmt 
	| 	expression_stmt 
	| 	ifstmt
	| 	for_stmt 
	| 	while_stmt 
	| 	return_stmt;
compoundstmt	:	'{' statement_list '}';
nullstmt	:	';' ;
expression_stmt	:	expression ';' ;
ifstmt	:	IF '(' expression ')' statement
	|	IF '(' expression ')' statement ELSE statement;
for_stmt	:	FOR '(' expression ';' expression ';' expression ')' statement;
while_stmt	:	WHILE '(' expression ')' statement;
return_stmt	:	RETURN expression ';' 
	| 	RETURN ';';
expression	:	assignment_expr 
	| 	binary_expr;
assignment_expr	:	lvalue '=' expression;
lvalue		:	'*' expression 
	| 	IDENT 
	| 	IDENT '[' expression ']';
binary_expr	:	binary_expr binary_op unary_expr 
	|	unary_expr;
binary_op	:	BOOLEAN_OP 
	| 	REL_OP
	| 	'+' 
	| 	'-' 
	| 	'*';
unary_expr	:	'!' unary_expr 
	| 	'+' unary_expr 
	| 	'-' unary_expr 
	| 	'&' unary_expr
	| 	'*' unary_expr 
	| 	DOUBLE_OP unary_expr
	| 	postfix_expr
	;
postfix_expr	:	IDENT '[' expression ']'
	|	IDENT '(' argument_list ')'
	|	IDENT '(' ')'
	|	postfix_expr DOUBLE_OP
	|	postfix_expr DOUBLE_OP
		primary_expr;
primary_expr	:	IDENT | constant | '(' expression ')';
constant	:	ICONSTANT 
	| 	FCONSTANT
	| 	CHAR_CONSTANT 
	| 	STRING_CONSTANT;
argument_list	:	argument_list ',' expression 
	| 	expression;

%%
int main()
{
	return yyparse();
}
#include<stdio.h>
void yyerror(char const * s)
{
	fprintf(stderr, "%s\n");
}
