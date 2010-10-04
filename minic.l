%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minic.tab.h"
#include "attr.h"
%}

id			[_a-zA-Z][_a-zA-Z0-9]*
special			[{}\(\)\[\];,\.+\-\*\!\&\=]
iconstant		[0-9]+
char_constant		\'(.|\\n)\'
string_constant		\"(.|\\n)*\"

%%

extern			return EXTERN;
register		return REGISTER;
void			return VOID;
int			return INT;
char			return CHAR;
if 			return IF;
else			return ELSE;
for 			return FOR;
while 			return WHILE;
return 			return RETURN;
{id}			{
				yylval.string_attr.sval = (char*)malloc((yyleng+1)*sizeof(char));
				strncpy(yylval.string_attr.sval, yytext, yyleng);
				yylval.string_attr.sval[yyleng] = '\0';
				return IDENT;
			}
{special}		return yytext[0];
"&&"			yylval.int_attr.ival = 1; return BOOLEAN_OP;
"||"			yylval.int_attr.ival = 2; return BOOLEAN_OP;
"=="			yylval.int_attr.ival = 3; return REL_OP;
"!="			yylval.int_attr.ival = 4; return REL_OP;
">"			yylval.int_attr.ival = 5; return REL_OP;
"<"			yylval.int_attr.ival = 6; return REL_OP;
">="			yylval.int_attr.ival = 7; return REL_OP;
"<="			yylval.int_attr.ival = 8; return REL_OP;
"++"			yylval.int_attr.ival = 9; return DOUBLE_OP;
"--"			yylval.int_attr.ival = 0; return DOUBLE_OP;
{iconstant}		{
				yylval.int_attr.ival = atoi(yytext);
				return ICONSTANT;
			}
{char_constant}		yylval.char_attr.cval = yytext[1]; return CHAR_CONSTANT;
{string_constant}	{
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
