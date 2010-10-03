%{
#include "symrec.h"
#include"calc.tab.h"
#include<stdlib.h>

int yywrap();
%}

id		[_a-zA-Z][_a-zA-Z0-9]*
special		[{}\(\)\[\];,\.+-\*]
iconstant	(+|-)?[0][0-9]+(o|x)?
char_constant	\'.\'
string_constant	\".*\"

%%

extern		return EXTERN;
register	return REGISTER;
void		return VOID;
int		return INT;
char		return CHAR;
{id}		return IDENT;
{special}	return yytext(0);
if 		return IF;
else		return ELSE;
for 		return FOR;
while 		return WHILE;
return 		return RETURN;
"&&"		return BOOLEAN_OP;
"||"		return BOOLEAN_OP;
"=="		return REL_OP;
"!="		return REL_OP;
">"		return REL_OP;
"<"		return REL_OP;
">="		return REL_OP;
"<="		return REL_OP;
"++"		return DOUBLE_OP;
"--"		return DOUBLE_OP;
		
%%
int yywrap()
{
	return 1;
}
