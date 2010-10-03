%{
#include <stdio.h>
MODIFIER, VOID, INT, CHAR, IF, ELSE, FOR, WHILE, RETURN, IDENT, 
BOOLEAN_OP, REL_OP, DOUBLE_OP, ICONSTANT, CHAR_CONSTANT, STRING_CONSTANT
%}

id			[_a-zA-Z][_a-zA-Z0-9]*
special			[{}\(\)\[\];,\.+\-\*]
iconstant		[0-9]+
char_constant		\'.\'
string_constant		\".*\"

%%

extern			printf("MODIFIER\n");return MODIFIER;
register		printf("MODIFIER\n");return MODIFIER;
void			printf("VOID\n");return VOID;
int			printf("INT\n");return INT;
char			printf("CHAR\n");return CHAR;
if 			printf("IF\n");return IF;
else			printf("ELSE\n");return ELSE;
for 			printf("FOR\n");return FOR;
while 			printf("WHILE\n");return WHILE;
return 			printf("RETURN\n");return RETURN;
{id}			printf("IDENT\n");return IDENT;
{special}		printf("%s\n",yytext);return yytext[0];
"&&"			printf("BOOLEAN_OP\n");return BOOLEAN_OP;
"||"			printf("BOOLEAN_OP\n");return BOOLEAN_OP;
"=="			printf("REL_OP\n");return REL_OP;
"!="			printf("REL_OP\n");return REL_OP;
">"			printf("REL_OP\n");return REL_OP;
"<"			printf("REL_OP\n");return REL_OP;
">="			printf("REL_OP\n");return REL_OP;
"<="			printf("REL_OP\n");return REL_OP;
"++"			printf("DOUBLE_OP\n");return DOUBLE_OP;
"--"			printf("DOUBLE_OP\n");return DOUBLE_OP;
{iconstant}		printf("ICONSTANT\n");return ICONSTANT;
{char_constant}		printf("CHAR_CONSTANT\n");return CHAR_CONSTANT;
{string_constant}	printf("STRING_CONSTANT\n");return STRING_CONSTANT;
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
