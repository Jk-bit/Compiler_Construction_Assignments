%{
	#include <stdio.h>
%}

alpha		[a-zA-Z]
digit		[0-9]

%%

[a-zA-Z][a-zA-Z0-9]*					{
								printf("TOKEN : IDENTIFIER\n");
								printf("yytext : %s\n", yytext);
							}   

[0-9]+					{
								printf("TOKEN : NUMBER\n");
								printf("yytext : %s\n", yytext);
							}

[-+*/=]						{
								printf("TOKEN : OPERATOR\n");
								printf("yytext : %s\n", yytext);
							}

[ \t]						{
								printf("TOKEN : WHITESPACE\n");
								printf("yytext : %s\n", yytext);
							}

.							{
								printf("TOKEN : INVALID\n");
								printf("yytext : %s\n", yytext);
							}

%%

int yywrap(void){
	return 1;
}
