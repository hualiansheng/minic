%{
#include <memory.h>
int yylex (void);
void yyerror (char const*);
%}
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


%%
program		:	external_decls {printf("ok");};
external_decls	:	declaration external_decls 
	| function_def;
declaration	:	modifier type_name varlist;
modifier	:	
	| 	EXTERN 
	| 	REGISTER;
type_name	:	VOID | INT | CHAR;
var_list	:	varlist ',' var_item 
	| 	var_item;
var_item	:	array_var 
	| 	scalar_var 
	| 	'*' scalar_var;
array_var	:	IDENT '[' ICONSTANT ']';
scalar_var	:	IDENT 
	| 	IDNET '(' parm_type_list ')';
function_def	:	functionhdr '(' function_body ')';
function_hdr	:	type_name IDENT '(' parm_type_list ')'
	|	type_name '*' IDENT '(' parm_type_list ')'
	|	IDNET '(' parm_type_list ')';
parm_type_list	:	VOID 
	| 	parmlist;
parm_list	:	parmlist , parm_decl 
	| 	parm_decl;
parm_decl	:	type_name ident 
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
	| 	forstmt 
	| 	whilestmt 
	| 	return_stmt;
compoundstmt	:	'{' statement_list '}';
nullstmt	:	;
expression_stmt	:	expression ;
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
	|	IDENT '(' argumentlist ')'
	|	IDENT '(' ')'
	|	postfix_expr DOUBLE_OP
	|	postfix_expr DOUBLE_OP
		primary_expr;
primary_expr	:	IDENT | CONSTANT | '(' expression ')';
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
