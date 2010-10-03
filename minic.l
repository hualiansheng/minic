%{
#include <stdio.h>
#include "minic.tab.h"
%}

id			[_a-zA-Z][_a-zA-Z0-9]*
special			[{}\(\)\[\];,\.+\-\*\!\&]
iconstant		[0-9]+
char_constant		\'.\'
string_constant		\".*\"

%%

extern			return EXTERN;
register		return EXTERN;
void			return VOID;
int			return INT;
char			return CHAR;
if 			return IF;
else			return ELSE;
for 			return FOR;
while 			return WHILE;
return 			return RETURN;
{id}			return IDENT;
{special}		return yytext[0];
"&&"			return BOOLEAN_OP;
"||"			return BOOLEAN_OP;
"=="			return REL_OP;
"!="			return REL_OP;
">"			return REL_OP;
"<"			return REL_OP;
">="			return REL_OP;
"<="			return REL_OP;
"++"			return DOUBLE_OP;
"--"			return DOUBLE_OP;
{iconstant}		return ICONSTANT;
{char_constant}		return CHAR_CONSTANT;
{string_constant}	return STRING_CONSTANT;
\n			{}
.			{}
		
%%
int yywrap()
{
	return 1;
}
void main()
{
	yylex();
}
