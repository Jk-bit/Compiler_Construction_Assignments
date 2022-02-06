%{
	#include <stdio.h>
	#include "y.tab.h"
	void yyerror(char *);
%}


%%
[0-9]+[.][0-9]+			{
							yylval.dval = atof(yytext);
							return FLOAT;
						}

[0-9]+					{
							yylval.dval = atoi(yytext);
							return FLOAT;
						}

[a-z]					{
							yylval.sym_index = *yytext - 'a';
							return NAME;
						}


[-+=()/*\n]				{
							return *yytext;
						}

[ \t]+					;

.						yyerror("Inavlid character");

%%

int yywrap(void){
	return 1;
}
