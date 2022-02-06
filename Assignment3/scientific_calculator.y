%token INTEGER
%left '+' '-'
%left '*' '/'

%{
	#include <stdio.h>
	int yylex(void);
	void yyerror(char *);
	#include "y.tab.h"
%}

%%

	program :
				program expr '\n'			{ printf("%d\n", $2); }
				|
				;

	expr :
				expr '+' expr				{ $$ = $1 + $3; }
				| expr '-' expr				{ $$ = $1 - $3; }
				| expr '*' expr				{ $$ = $1 * $3; }
				| expr '/'	expr			{ $$ = $1 / $3; }
				| '(' expr ')'				{ $$ = $2; }
				| INTEGER
				;

%%

void yyerror(char *error){
	printf("%s\n", error);
}

int main(void){
	yyparse();
	return 0;
}
