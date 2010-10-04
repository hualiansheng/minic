%{
#define YYSTYPE int
#include <memory.h>
#include <stdio.h>
#include <assert.h>
int yylex (void);
void yyerror (char const*);
extern FILE* yyin;
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
%token ICONSTANT
%token FCONSTANT
%token CHAR_CONSTANT
%token STRING_CONSTANT



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
program		:	external_decls {fprintf( stderr, "ok");}
		;
external_decls	:	declaration external_decls 
		| 	function_list
		;
declaration	:	EXTERN type_name var_list ';'
		| 	REGISTER type_name var_list ';'
		| 	type_name var_list ';'
		;
function_list	:	function_list function_def 
		|	function_def
		;
//modifier	:	
//	| 	EXTERN 
//	| 	REGISTER;
type_name	:	VOID 
		|	INT 
		|	CHAR;
var_list	:	var_list ',' var_item 
		|	var_item
		;
var_item	:	array_var 
		| 	scalar_var 
		| 	'*' scalar_var
		;
array_var	:	IDENT '[' ICONSTANT ']'
		|	IDENT '[' ICONSTANT error {yyclearin;
		fprintf(stderr, "line!!!: %d\n",@4.first_column);}
		;
scalar_var	:	IDENT 
		| 	IDENT '(' parm_type_list ')'
		;
function_def	:	function_hdr '{' function_body '}'
		;
function_hdr	:	type_name IDENT '(' parm_type_list ')'
		|	type_name '*' IDENT '(' parm_type_list ')'
		|	IDENT '(' parm_type_list ')'
		;
parm_type_list	:	VOID 
		| 	parm_list
		;
parm_list	:	parm_list ',' parm_decl 
		| 	parm_decl
		;
parm_decl	:	type_name IDENT
		| 	type_name '*' IDENT
		;
function_body	:	internal_decls statement_list
		;
internal_decls	:	
		| 	declaration internal_decls 
		;
statement_list	:	
		| 	statement statement_list 
		;
statement	:	compoundstmt 
		| 	nullstmt 
		| 	expression_stmt 
		| 	ifstmt
		| 	for_stmt 
		| 	while_stmt 
		| 	return_stmt
		;
compoundstmt	:	'{' statement_list '}'
		;
nullstmt	:	';' 
		;
expression_stmt	:	expression ';' 
		;
ifstmt		:	IF '(' expression ')' statement %prec UIF
		|	IF '(' expression ')' statement ELSE statement
		; 
for_stmt	:	FOR '(' expression ';' expression ';' expression ')' statement
		;
while_stmt	:	WHILE '(' expression ')' statement
		;
return_stmt	:	RETURN expression ';' 
		| 	RETURN ';'
		;
expression	:	rvalue 
		|	assignment_expression
		;
assignment_expression
		:	lvalue '=' assignment_expression
		|	lvalue '=' rvalue
		;
	
lvalue		:	'*' rvalue 
		|	IDENT
		|	IDENT '[' expression ']'
		;
rvalue		:	lvalue %prec '-'
		|	rvalue '+' rvalue
		|	rvalue '-' rvalue
		|	rvalue '*' rvalue
		|	rvalue op  rvalue %prec BOOLEAN_OP
		|	'(' rvalue ')'
		|	'+' rvalue %prec UPLUS
		|	'-' rvalue %prec UMINUS
		|	'!' rvalue %prec NOT
		|	'&' lvalue %prec ADDRESS
		|	DOUBLE_OP lvalue %prec DOUBLE
		|	lvalue DOUBLE_OP %prec DOUBLE
		|	constant
		|	IDENT '(' argument_list ')'
		|	IDENT '(' ')'
		;
op		:	BOOLEAN_OP 
		| 	REL_OP
		;
constant	:	ICONSTANT 
		| 	FCONSTANT
		| 	CHAR_CONSTANT 
		| 	STRING_CONSTANT
		;
argument_list	:	argument_list ',' expression 
		| 	expression
;

%%
int main(int argc, char** argv)
{
	yydebug=1;
	FILE* source_file;
	argc--,argv++;
	if(argc)
		source_file = fopen (argv[0], "r" );
	assert( source_file != NULL );
	fprintf(stderr,"sdf\n");
	yyin = source_file;
	yyparse();
	fclose(source_file);
}
void yyerror(char const * s)
{
	fprintf(stderr, "%s\n",s);
}
