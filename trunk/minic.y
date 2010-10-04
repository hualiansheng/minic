%{
#define YYSTYPE int
#include <memory.h>
#include <stdio.h>
#include <assert.h>
#include "AST.h"
int yylex (void);
void yyerror (char const*);
AST_NODE* tree_root;
AST_NODE* root;
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
program		:	external_decls {root = AST_new_Node();
					root -> nodeType = PROGRAM;
					root -> nodeLevel = 0;
					AST_addChild(root,EXTERNAL_DECLS,$1);
					}
		;
external_decls	:	declaration external_decls
			{
			root = AST_new_Node();
			root -> nodeType = EXTERNAL_DECLS;
			AST_addChild(root,DECLARATION,$1);
			AST_addChild(root,external_decls,$2);
			$$ = root;			
			} 
		| 	function_list
			{
			root = AST_new_Node();
			root -> nodeType = EXTERNAL_DECLS;
			AST_addChild(root,FUNCTION_LIST,$1);
			$$ = root;			
			}
		;
declaration	:	EXTERN type_name var_list ';'
			{
			root = AST_new_Node();
			root -> nodeType = DECLARATION;
			AST_addChild(root,EXTERN,$1);
			AST_addChild(root,TYPE_NAME,$2);
			AST_addChild(root,VAR_LIST,$3);
			AST_addChild(root,SEMICOLON,$4.ptr);
			$$ = root;
			}
		| 	REGISTER type_name var_list ';'
			{
			root = AST_new_Node();
			root -> nodeType = DECLARATION;
			AST_addChild(root,REGISTER,$1);
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
			AST_addChild(root,VOID,$1.ptr);
			$$ = root;
			} 
		|	INT 
			{
			root = AST_new_Node();
			root -> nodeType = TYPE_NAME;
			AST_addChild(root,INT,$1.ptr);
			$$ = root;
			} 
		|	CHAR
			{
			root = AST_new_Node();
			root -> nodeType = TYPE_NAME;
			AST_addChild(root,CHAR,$1.ptr);
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
			AST_addChild(root,IDENT,$1.ptr);
			AST_addChild(root,LEFT_SQUARE_BRACKET,$2.ptr);
			AST_addChild(root,ICONSTANT,$3.ptr);
			AST_addChild(root,RIGHT_SQUARE_BRACKET,$4.ptr);
			$$ = root;
			}
		;
scalar_var	:	IDENT
			{
			root = AST_new_Node();
			root -> nodeType = SCALAR_VAR;
			AST_addChild(root,IDENT,$1.ptr);
			$$ = root;
			}
			 
		| 	IDENT '(' parm_type_list ')'
			{
			root = AST_new_Node();
			root -> nodeType = SCALAR_VAR;
			AST_addChild(root,IDENT,$1.ptr);
			AST_addChild(root,LEFT_PARENTHESES,$2.ptr);
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
			AST_addChild(root,LEFT_BRACE,$4.ptr);
			$$ = root;
			}			
		;
function_hdr	:	type_name IDENT '(' parm_type_list ')'
			{
			root = AST_new_Node();
			root -> nodeType = FUNCTION_HDR;
			AST_addChild(root,TYPE_NAME,$1);
			AST_addChild(root,IDENT,$2.ptr);
			AST_addChild(root,LEFT_PARENTHESES,$3.ptr);
			AST_addChild(root,PARM_TYPE_LIST,$4);
			AST_addChild(root,RIGHT_PARENTHESES,$5.ptr);
			$$ = root;
			}
		|	type_name '*' IDENT '(' parm_type_list ')'
			{
			root = AST_new_Node();
			root -> nodeType = FUNCTION_HDR;
			AST_addChild(root,TYPE_NAME,$1);
			AST_addChild(root,STAR,$2,ptr);
			AST_addChild(root,IDENT,$3.ptr);
			AST_addChild(root,LEFT_PARENTHESES,$4.ptr);
			AST_addChild(root,PARM_TYPE_LIST,$5);
			AST_addChild(root,RIGHT_PARENTHESES,$6.ptr);
			$$ = root;
			}

		|	IDENT '(' parm_type_list ')'
			{
			root = AST_new_Node();
			root -> nodeType = FUNCTION_HDR;
			AST_addChild(root,IDENT,$1.ptr);
			AST_addChild(root,LEFT_PARENTHESES,$2.ptr);
			AST_addChild(root,PARM_TYPE_LIST,$3);
			AST_addChild(root,RIGHT_PARENTHESES,$4.ptr);
			$$ = root;
			}
		;
parm_type_list	:	VOID
			{
			root = AST_new_Node();
			root -> nodeType = PARM_TYPE_LIST;
			AST_addChild(root,VOID,$1.ptr);
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
			AST_addChild(root,IDENT,$2.ptr);
			$$ = root;
			}
		| 	type_name '*' IDENT
			{
			root = AST_new_Node();
			root -> nodeType = PARM_DECL;
			AST_addChild(root,TYPE_NAME,$1);
			AST_addChild(root,STAR,$2.ptr);
			AST_addChild(root,IDENT,$3.ptr);
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
			AST_addChild(root,EPSILON,$1);
			$$ = root;
			}
		| 	declaration internal_decls
			{
			root = AST_new_Node();
			root -> nodeType = INTERNAL_DECLS;
			AST_addChild(root,DECLARATION,$1);
			AST_addChild(root,INTERAL_DECLS,$2);
			$$ = root;
			}
 
		;
statement_list	:	
			{
			root = AST_new_Node();
			root -> nodeType = STATEMENT_LIST;
			AST_addChild(root,EPSILON,$1);
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
compoundstmt	:	'{' statement_list '}'
			{
			root = AST_new_Node();
			root -> nodeType = COMPOUNDSTMT;
			AST_addChild(root,LEFT_BRACE,$1.ptr);
			AST_addChild(root,STATEMENT_LIST,$2);
			AST_addChild(root,RIGHT_BRACE,$3.ptr);
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
			AST_addChild(root,IF,$1.ptr);
			AST_addChild(root,LEFT_PARENTHESES,$2.ptr);
			AST_addChild(root,EXPRESSION,$3);
			AST_addChild(root,RIGHT_PARENTHESES,$4.ptr);
			AST_addChild(root,STATEMENT,$5);
			$$ = root;
			}
 
		|	IF '(' expression ')' statement ELSE statement
			{
			root = AST_new_Node();
			root -> nodeType = IFSTMT;
			AST_addChild(root,IF,$1.ptr);
			AST_addChild(root,LEFT_PARENTHESES,$2.ptr);
			AST_addChild(root,EXPRESSION,$3);
			AST_addChild(root,RIGHT_PARENTHESES,$4.ptr);
			AST_addChild(root,STATEMENT,$5);
			AST_addChild(root,ELSE,$6.ptr);
			AST_addChild(root,STATEMENT,$7);
			$$ = root;
			}
		; 
for_stmt	:	FOR '(' expression ';' expression ';' expression ')' statement
			{
			root = AST_new_Node();
			root -> nodeType = FOR_STMT;
			AST_addChild(root,FOR,$1.ptr);
			AST_addChild(root,LEFT_PARENTHESES,$2.ptr);
			AST_addChild(root,EXPRESSION,$3);
			AST_addChild(root,SEMICOLON,$4.ptr);
			AST_addChild(root,EXPRESSION,$5);
			AST_addChild(root,SEMICOLON,$6.ptr);
			AST_addChild(root,EXPRESSION,$7);
			AST_addChild(root,RIGHT_PARENTHESES,$8.ptr);			
			AST_addChild(root,STATEMENT,$9.ptr);
			$$ = root;
			}
		;
while_stmt	:	WHILE '(' expression ')' statement
			{
			root = AST_new_Node();
			root -> nodeType = WHILE_STMT;
			AST_addChild(root,WHILE,$1.ptr);
			AST_addChild(root,LEFT_PARENTHESES,$2.ptr);
			AST_addChild(root,EXPRESSION,$3);
			AST_addChild(root,RIGHT_PARENTHESES,$4.ptr);
			AST_addChild(root,STATEMENT,$5);
			$$ = root;
			}
		;
return_stmt	:	RETURN expression ';'
			{
			root = AST_new_Node();
			root -> nodeType = RETURN_STMT;
			AST_addChild(root,RETURN,$1.ptr);
			AST_addChild(root,EXPRESSION,$2);
			AST_addChild(root,SEMICOLON,$3.ptr);
			$$ = root;
			} 
		| 	RETURN ';'
			{
			root = AST_new_Node();
			root -> nodeType = RETURN_STMT;
			AST_addChild(root,RETURN,$1.ptr);
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
			AST_addChild(root,IDENT,$1.ptr);
			$$ = root;
			} 
		|	IDENT '[' expression ']'
			{
			root = AST_new_Node();
			root -> nodeType = LVALUE;
			AST_addChild(root,IDENT,$1.ptr);
			AST_addChild(root,LEFT_SQUARE_BRACKET,$2.ptr);
			AST_addChild(root,EXPRESSION,$3);
			AST_addChild(root,RIGHT_SQUARE_BRACKET,$4.ptr);
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
			AST_addChild(root,LEFT_PARENTHESES,$1.ptr);
			AST_addChild(root,RVALUE,$2);
			AST_addChild(root,RIGHT_PARENTHESES,$3.ptr);
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
			AST_addChild(root,DOUBLE_OP,$1.ptr);
			AST_addChild(root,LVALUE,$2);
			$$ = root;
			} 
		|	lvalue DOUBLE_OP %prec DOUBLE
			{
			root = AST_new_Node();
			root -> nodeType = RVALUE;
			AST_addChild(root,LVALUE,$1);
			AST_addChild(root,DOUBLE_OP,$2.ptr);
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
			AST_addChild(root,IDENT,$1.ptr);
			AST_addChild(root,LEFT_PARENTHESES,$2.ptr);
			AST_AddChild(root,ARGUMENT_LIST,$3);
			AST_addChild(root,RIGHT_PARENTHESES,$4.ptr);
			$$ = root;
			} 
		|	IDENT '(' ')'
			{
			root = AST_new_Node();
			root -> nodeType = RVALUE;
			AST_addChild(root,IDENT,$1.ptr);
			AST_addChild(root,LEFT_PARENTHESES,$2.ptr);
			AST_addChild(root,RIGHT_PARENTHESES,$3.ptr);
			$$ = root;
			}
		;
op		:	BOOLEAN_OP 
			{
			root = AST_new_Node();
			root -> nodeType = OP;
			AST_addChild(root,BOOLEAN_OP,$1.ptr);
			$$ = root;
			}
		| 	REL_OP
			{
			root = AST_new_Node();
			root -> nodeType = OP;
			AST_addChild(root,REL_OP,$1.ptr);
			$$ = root;
			}
		;
constant	:	ICONSTANT
			{
			root = AST_new_Node();
			root -> nodeType = CONSTANT;
			AST_addChild(root,ICONSTANT,$1.ptr);
			$$ = root;
			}
		| 	CHAR_CONSTANT 
			{
			root = AST_new_Node();
			root -> nodeType = CONSTANT;
			AST_addChild(root,CHAR_CONSTANTs,$1.ptr);
			$$ = root;
			}
		| 	STRING_CONSTANT
			{
			root = AST_new_Node();
			root -> nodeType = CONSTANT;
			AST_addChild(root,STRING_CONSTANT,$1.ptr);
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