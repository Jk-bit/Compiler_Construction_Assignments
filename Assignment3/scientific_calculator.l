%{
	#include <stdio.h>
	#include "y.tab.h"
	void yyerror(char *);
%}


%%

[0-9]+		{
				yylval = atoi(yytext);
				return INTEGER;
			}

[-+()/*\n]	{
				return *yytext;
			}

[ \t]+		;

.			yyerror("Inavlid character");

%%

int yywrap(void){
	return 1;
}

