%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "attr.h"
#include "minic.tab.h"
extern YYSTYPE yylval;
extern YYLTYPE yylloc;
void change_yylloc();
%}

id			[_a-zA-Z][_a-zA-Z0-9]*
special			[{}\(\)\[\];,\.+\-\*\!\&\=]
iconstant		[0-9]+
char_constant		\'(.|\\n)\'
string_constant		\"(.|\\n)*\"

%%

[:space:]+		yylloc.last_column++; REJECT;
extern			yylval.string_attr.ptr = AST_new_Node(); change_yylloc(); return EXTERN;
register		yylval.string_attr.ptr = AST_new_Node(); change_yylloc(); return REGISTER;
void			yylval.string_attr.ptr = AST_new_Node(); change_yylloc(); return VOID;
int			yylval.string_attr.ptr = AST_new_Node(); change_yylloc(); return INT;
char			yylval.string_attr.ptr = AST_new_Node(); change_yylloc(); return CHAR;
if 			yylval.string_attr.ptr = AST_new_Node(); change_yylloc(); return IF;
else			yylval.string_attr.ptr = AST_new_Node(); change_yylloc(); return ELSE;
for 			yylval.string_attr.ptr = AST_new_Node(); change_yylloc(); return FOR;
while 			yylval.string_attr.ptr = AST_new_Node(); change_yylloc(); return WHILE;
return 			yylval.string_attr.ptr = AST_new_Node(); change_yylloc(); return RETURN;
{id}			{
				yylval.string_attr.ptr = AST_new_Node();
				yylval.string_attr.sval = (char*)malloc((yyleng+1)*sizeof(char));
				yylval.string_attr.ptr -> content.s_content = (char*)malloc((yyleng+1)*sizeof(char));
				strncpy(yylval.string_attr.sval, yytext, yyleng);
				strncpy(yylval.string_attr.ptr -> content.s_content, yytext, yyleng);
				yylval.string_attr.sval[yyleng] = '\0';
				yylval.string_attr.ptr -> content.s_content[yyleng] = '\0';
				//fprintf(stderr,"IDENT:%s\n",yytext);
				change_yylloc();
				return IDENT;
			}
{special}		yylval.char_attr.ptr = AST_new_Node(); change_yylloc(); return yytext[0];
"&&"			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = 1; change_yylloc(); return BOOLEAN_OP;
"||"			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = 2; change_yylloc(); return BOOLEAN_OP;
"=="			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = 3; change_yylloc(); return REL_OP;
"!="			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = 4; change_yylloc(); return REL_OP;
">"			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = 5; change_yylloc(); return REL_OP;
"<"			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = 6; change_yylloc(); return REL_OP;
">="			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = 7; change_yylloc(); return REL_OP;
"<="			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = 8; change_yylloc(); return REL_OP;
"++"			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = 9; change_yylloc(); return DOUBLE_OP;
"--"			yylval.int_attr.ptr = AST_new_Node(); yylval.int_attr.ival = 0; change_yylloc(); return DOUBLE_OP;
{iconstant}		{
				yylval.int_attr.ptr = AST_new_Node(); 
				yylval.int_attr.ival = atoi(yytext);
				yylval.int_attr.ptr->content.i_content = yylval.int_attr.ival;
				change_yylloc();
				return ICONSTANT;
			}
{char_constant}		{
				yylval.char_attr.ptr = AST_new_Node();
				yylval.char_attr.cval = yytext[1];
				yylval.char_attr.ptr->content.c_content = yylval.char_attr.cval;
				change_yylloc();
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
				change_yylloc();
				return STRING_CONSTANT;
			}
\n			{
				++yylloc.last_line;
				yylloc.last_column = 0;
				yylloc.first_column = 0;
			}
.			{
			}
		
%%
int yywrap()
{
	return 1;
}
void change_yylloc()
{
	yylloc.first_line = yylloc.last_line;
	yylloc.first_column = yylloc.last_column;
	yylloc.last_column = yylloc.first_column + yyleng;
}
