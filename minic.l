%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "attr.h"
#include "minic.tab.h"
extern YYSTYPE yylval;
%}

id			[_a-zA-Z][_a-zA-Z0-9]*
special			[{}\(\)\[\];,\.+\-\*\!\&\=]
iconstant		[0-9]+
char_constant		\'(.|\\n)\'
string_constant		\"(.|\\n)*\"

%%

extern			yylval.string_attr.ptr = AST_new_Node(); return EXTERN;
register		yylval.string_attr.ptr = AST_new_Node(); return REGISTER;
void			yylval.string_attr.ptr = AST_new_Node(); return VOID;
int			yylval.string_attr.ptr = AST_new_Node(); return INT;
char			yylval.string_attr.ptr = AST_new_Node(); return CHAR;
if 			yylval.string_attr.ptr = AST_new_Node(); return IF;
else			yylval.string_attr.ptr = AST_new_Node(); return ELSE;
for 			yylval.string_attr.ptr = AST_new_Node(); return FOR;
while 			yylval.string_attr.ptr = AST_new_Node(); return WHILE;
return 			yylval.string_attr.ptr = AST_new_Node(); return RETURN;
{id}			{
				yylval.string_attr.ptr = AST_new_Node();
				yylval.string_attr.sval = (char*)malloc((yyleng+1)*sizeof(char));
				strncpy(yylval.string_attr.sval, yytext, yyleng);
				yylval.string_attr.sval[yyleng] = '\0';
				return IDENT;
			}
{special}		yylval.char_attr.ptr = AST_new_Node(); return yytext[0];
"&&"			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = 1; return BOOLEAN_OP;
"||"			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = 2; return BOOLEAN_OP;
"=="			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = 3; return REL_OP;
"!="			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = 4; return REL_OP;
">"			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = 5; return REL_OP;
"<"			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = 6; return REL_OP;
">="			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = 7; return REL_OP;
"<="			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = 8; return REL_OP;
"++"			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = 9; return DOUBLE_OP;
"--"			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = 0; return DOUBLE_OP;
{iconstant}		{
				yylval.int_attr.ptr = AST_new_Node(); 
				yylval.int_attr.ival = atoi(yytext);
				return ICONSTANT;
			}
{char_constant}		yylval.char_attr.ptr = AST_new_Node(); yylval.char_attr.cval = yytext[1]; return CHAR_CONSTANT;
{string_constant}	{
				yylval.string_attr.ptr = AST_new_Node(); 
				yylval.string_attr.sval = (char*)malloc((yyleng-1)*sizeof(char));
				strncpy(yylval.string_attr.sval, yytext+1, yyleng-1);
				yylval.string_attr.sval[yyleng-2] = '\0';
				return STRING_CONSTANT;
			}
\n			{}
.			{}
		
%%
int yywrap()
{
	return 1;
}
