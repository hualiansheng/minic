%{
#include <math.h>
#include <memory.h>
#include "symrec.h"
int yylex (void);
void yyerror (char const*);
int valtype;
symrec* cursym;
%}
%union {
	double val;
	symrec* tptr;
}
%token <val> NUM
%token <tptr> ID
%token VAR
%token LIST
%type <val> exp
%type <val> stmt

%left '+' '-'
%left '*' '/'

%%

input:	/*empty*/
	| input line
;
line:	'\n'
	| decl '\n'
	| stmt '\n'
;
decl: VAR {valtype = 1;} vlist //valtype is bogus now.

;

vlist:	ID	{
	 			if( getsym($1 -> name) != NULL )	//error: redeclearation
					printf( "The variable %s has already been decleared!\n"
					,$1 -> name );
				else
					putsym($1 -> name, 0);
				free( $1 -> name );
				free( $1 );
				
	 }
	 | vlist ',' ID{
		 			if( getsym($3 -> name) != NULL )	//error: redeclearation
						printf("The variable: %s has already been decleared!\n"
						,$3->name);
					else
						putsym($3 -> name, 0);
					free( $3 -> name );
	 }

;
stmt: ID '=' exp {
					cursym = getsym($1 -> name);
					if ( cursym == NULL ) //symbol not found
						printf("Identifier not found!\n");
					else
						cursym -> val = $3;
					$$ = $3;
					//free( $1 -> name );

	}
	| exp {printf("%lf\n",$1)};
	| LIST {printsym();}
;

exp:	NUM	{$$=$1;}
   	| 	ID {
			cursym = getsym($1 -> name);
			if( cursym == NULL)
				printf("Identifier not found!\n"), $$ = 0;
			else
				$$ = cursym -> val;
			printf("id: %s\n",$1->name);
			//free( $1 -> name );
	}
	| exp '+' exp 	{$$=$1+$3;}
	| exp '-' exp 	{$$=$1-$3;}
	| exp '*' exp	{$$=$1*$3;}
	| exp '/' exp	{$$=$1/$3;}
	| '(' exp ')'	{$$ = $2;}
;
%%

int main(void)
{
	init_sym_table();
	return yyparse();
}
#include<stdio.h>
void yyerror(char const * s)
{
	fprintf(stderr, "%s\n");
}
