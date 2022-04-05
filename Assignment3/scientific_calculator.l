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


[-+=()/*%\n]			{
							return *yytext;
						}

[ \t]+					;

.						yyerror("Inavlid character");

sin						{ return SIN; }
cos						{ return COS; }
tan						{ return TAN; }
log						{ return LOG; }
sqrt					{ return SQRT; }
asin					{ return ASIN; }
acos					{ return ACOS; }
atan					{ return ATAN; }



%%

int yywrap(void){
	return 1;
}
