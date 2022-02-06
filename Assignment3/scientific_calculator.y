%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%{
	#include <stdio.h>
	#include <math.h>
	int yylex(void);
	void yyerror(char *);
	#include "y.tab.h"
	int zero_div = 0;
	double sym_table[26];
%}

%union{
	double dval;
	int sym_index;
}

%token <dval> FLOAT SIN COS TAN COSINE SEC COT LOG SQRT ASIN ACOS ATAN 
%token <sym_index>  NAME

%type <dval> expr

%%

	program :
				program statement '\n'			
				|
				;
	
	statement :
				NAME '=' expr				 	{
													sym_table[$1] = $3;
												}
				| expr							{
													if(zero_div){
														zero_div = 0;
													}
													else{
														printf("%g\n", $1);
													}
												}
				| NAME							{ printf("%g\n", sym_table[$1]); }
				;


	expr :
				expr '+' expr				{ $$ = $1 + $3; }
				| expr '-' expr				{ $$ = $1 - $3; }
				| expr '*' expr				{ $$ = $1 * $3; }
				| expr '/'	expr			{ if( $3 == 0){
												yyerror("Cannot divide a number by zero\n");
												zero_div = 1;
											}
											  else
												$$ = $1 / $3;
											}
				| '(' expr ')'				{ $$ = $2; }
				| '-' expr %prec UMINUS		{ $$ = -$2; }					
				| FLOAT
				| SIN'(' expr ')'			{ $$ = sin($3); }
				| COS'(' expr ')'			{ $$ = cos($3); }
				| TAN'(' expr ')'			{ $$ = tan($3); }
				| SQRT'(' expr ')'			{ $$ = sqrt($3); }
				| LOG'(' expr ')'			{ $$ = sqrt($3); }
				| ASIN'(' expr ')'			{ $$ = asin($3); }
				| ACOS'(' expr ')'			{ $$ = acos($3); }
				| ATAN'(' expr ')'			{ $$ = atan($3); }
				;

%%

void yyerror(char *error){
	printf("%s\n", error);
}

int main(void){
	yyparse();
	return 0;
}
