%{
	#include <stdio.h>
%}

alpha		[a-zA-Z]
digit		[0-9]
identifier  [a-zA-Z][a-zA-Z0-9]*
num			[0-9]+
ws			[ \t]*
expression	{ws}({identifier}|{num}){ws}[-+/*]{ws}({identifier}|{num}){ws}
statement   {ws}{identifier}{ws}={ws}{expression}{ws}	
%%

{statement}					{
								printf("A statememt has been parsed\n");
							}

while[(]({expression}|{statement}|{num})[)][{]{statement}[}]     {
															printf("While loop has been parsed\n");
														 }
	

{identifier}				{
								printf("TOKEN : IDENTIFIER\n");
								printf("yytext : %s\n", yytext);
							}   

{num}							{
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
