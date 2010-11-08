%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "attr.h"
#include "minic.tab.h"
extern YYSTYPE yylval;
extern YYLTYPE yylloc;
%}

id			[_a-zA-Z][_a-zA-Z0-9]*
special			[{}\(\)\[\];,\.+\-\*\!\&\=]
iconstant		[0-9]+
char_constant		\'(.|\\n)\'
string_constant		\"(.|\\n)*\"

%%
.				yylloc.first_column=yylloc.last_column; yylloc.last_column++; REJECT;
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
				yylval.string_attr.ptr -> content.s_content = (char*)malloc((yyleng+1)*sizeof(char));
				strncpy(yylval.string_attr.sval, yytext, yyleng);
				strncpy(yylval.string_attr.ptr -> content.s_content, yytext, yyleng);
				yylval.string_attr.sval[yyleng] = '\0';
				yylval.string_attr.ptr -> content.s_content[yyleng] = '\0';
				return IDENT;
			}
{special}		yylval.char_attr.ptr = AST_new_Node(); return yytext[0];
"&&"			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = ANDAND; return BOOLEAN_OP;
"||"			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = OROR; return BOOLEAN_OP;
"=="			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = EQEQ; return REL_OP;
"!="			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = NOTEQ; return REL_OP;
">"			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = LARGERTHAN; return REL_OP;
"<"			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = SMALLERTHAN; return REL_OP;
">="			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = LTEQ; return REL_OP;
"<="			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = STEQ; return REL_OP;
"++"			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = PLUSPLUS; return DOUBLE_OP;
"--"			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = MINUSMINUS; return DOUBLE_OP;
{iconstant}		{
				yylval.int_attr.ptr = AST_new_Node(); 
				yylval.int_attr.ival = atoi(yytext);
				yylval.int_attr.ptr->content.i_content = yylval.int_attr.ival;
				return ICONSTANT;
			}
{char_constant}		{
				yylval.char_attr.ptr = AST_new_Node();
				yylval.char_attr.cval = yytext[1];
				yylval.char_attr.ptr->content.c_content = yylval.char_attr.cval;
				return CHAR_CONSTANT;
			}
{string_constant}	{
				yylval.string_attr.ptr = AST_new_Node(); 
				yylval.string_attr.sval = (char*)malloc((yyleng-1)*sizeof(char));
				yylval.string_attr.ptr -> content.s_content = (char*)malloc((yyleng+1)*sizeof(char));
				strncpy(yylval.string_attr.sval, yytext+1, yyleng-1);
				strncpy(yylval.string_attr.ptr -> content.s_content, yytext+1, yyleng-1);
				yylval.string_attr.sval[yyleng-2] = '\0';
				yylval.string_attr.ptr -> content.s_content[yyleng-2] = '\0';
				return STRING_CONSTANT;
			}
\n			{
				++yylloc.last_line;
				yylloc.last_column = 0;
			}
.			{
			}
		
%%
int yywrap()
{
	return 1;
}