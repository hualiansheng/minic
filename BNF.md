# MiniC BNF 范式 #
约定：
  1. 终结符采用大写，终结符采用小写
  1. 终结符的定义和`AST.h`中对应

```
program		->	external_decls 
external_decls	->	declaration external_decls | function_list
declaration	->	EXTERN type_name var_list ; | REGISTER type_name var_list ; | type_name var_list ;
function_list	->	function_list function_def | function_def
type_name	->	VOID | INT | CHAR
var_list	        ->	var_list , var_item | var_item
var_item	->	array_var | scalar_var | * scalar_var
array_var	->	IDENT [ ICONSTANT ]
scalar_var	->	IDENT | IDENT ( parm_type_list )
function_def	->	function_hdr { function_body }
function_hdr	->	type_name IDENT ( parm_type_list ) | type_name * IDENT ( parm_type_list ) | IDENT ( parm_type_list )
parm_type_list	->	EPSILON | VOID | parm_list
parm_list	->	parm_list , parm_decl | parm_decl
parm_decl	->	type_name IDENT | type_name * IDENT
function_body	->	internal_decls statement_list
internal_decls	->	EPSILON | declaration internal_decls
statement_list	->	EPSILON | statement statement_list 
statement	->	compoundstmt | nullstmt | expression_stmt | ifstmt | for_stmt | while_stmt | return_stmt
compoundstmt	->	{ internal_decls statement_list }
nullstmt	->	;
expression_stmt	->	expression ;
ifstmt		->	IF ( expression ) statement | IF ( expression ) statement ELSE statement
for_stmt	->	FOR ( expression ; expression ; expression ) statement
while_stmt	->	WHILE ( expression ) statement
return_stmt	->	RETURN expression ; | RETURN ;
expression	->	rvalue | assignment_expression
assignment_expression	
		->	lvalue = assignment_expression | lvalue = rvalue
lvalue		->	* rvalue | IDENT | IDENT [ expression ]
rvalue		->	lvalue | rvalue + rvalue | rvalue - rvalue | rvalue * rvalue | rvalue op rvalue | ( rvalue ) | + rvalue | - rvalue| ! rvalue 
			| & lvalue | DOUBLE_OP lvalue |	lvalue DOUBLE_OP | constant | IDENT ( argument_list ) |	IDENT ( )
op		->	BOOLEAN_OP | REL_OP
constant	->	ICONSTANT | CHAR_CONSTANT | STRING_CONSTANT
argument_list	->	argument_list , expression | expression
```

文法说明：
  1. 表达式文法是二义性文法，然而bison支持指定特定规则的规约/移入优先级，因此能够正确识别表达式
```
      ...
      //设定优先级和结合律
      %right '='
      %left '+' '-'
      %left '*'
      ...
```
  1. `if-else if-else` 结构是二义性文法，然而bison支持特定规则的规约移入优先级，因此能够正确处理这一结构，minic认为else总跟最近的if匹配。
```
     ...
     %nonassoc UIF
     %nonassoc ELSE
     ...
     ifstmt		:	IF '(' expression ')' statement %prec UIF
     ...
```

参见：[bison手册：Operator\_Precedence](http://www.gnu.org/software/bison/manual/bison.html#Precedence)