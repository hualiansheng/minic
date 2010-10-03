%{
#include "symrec.h"
#include"calc.tab.h"
#include<stdlib.h>

int yywrap();
%}


DIGIT	[0-9]
SYMBOL [+-/*\n()=]
ID [_a-zA-Z][_a-zA-Z0-9]*

%%
"var"	return VAR;
"list"	return LIST;
{DIGIT}+	yylval.val=atof(yytext);return NUM;

{DIGIT}+"."{DIGIT}*	yylval.val=atof(yytext);  return NUM;

{SYMBOL}	return yytext[0];

{ID}		{	
				//printf("id detected! len=%d \n",yyleng);
				yylval.tptr = (symrec*) malloc(sizeof(symrec));
				yylval.tptr->name = (char*)calloc(1 , yyleng + 1);
				//printf("malloc complete!\n");
				strncpy(yylval.tptr->name, yytext, yyleng);
				//printf("cpy complete!\n");
				return ID;
				}
		
%%
int yywrap()
{
	return 1;
}
