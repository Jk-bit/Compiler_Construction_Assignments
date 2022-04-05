
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

/*extern int index_lex;
extern char operator_list[]; */

int index_lex = 0;
char *yytext;
int yylen;
char operator_list[] = {'+', '-', '=', '*', '/'};


void lexer(char *str){
	index_lex = 0;
	while( str[index_lex] != '\0'){
		int token = return_token(str);	
		printf("Token : %d\n", token);
	}
	printf("\n");
}

int return_token(char *str){
	free(yytext);
	if( is_var(str) ){
		printf("TOKEN VARIABLE \n");
		return VARIABLE;
	}
	else if( is_num(str) ){
		printf("TOKEN NUMBER \n");
		return NUMBER;
	}
	else if( is_operator(str) ){
		printf("TOKEN OPERATOR \n");
		return OPERATOR;
	}
	else if( is_ws(str) ){
		return WS;
	}
	else{
		printf("Invalid Token\n");
		handle_invalid(str);
		return INVALID;
	}
	printf("\n");
}

void handle_invalid(char *str){
	yytext = (char *)malloc( 2 * sizeof(char));
	string_copy(str, index_lex, 2, yytext);
	printf("yytext : %s\n", yytext);
	index_lex++;
}

bool is_lower_alpha(char ch){
	return ( ch >= 97 && ch <= 122 );
}

bool is_cap_alpha(char ch){
	return ( ch >= 65 && ch <= 90 );
}

bool is_digit( char ch ){
	return ( ch >= 48 && ch <= 57 );
}

bool is_var(char *str){
	int mov_index = index_lex;
	if( is_lower_alpha(str[mov_index]) || is_cap_alpha(str[mov_index]) ){
		mov_index++;
	}
	else{
		return false;
	}
	while(1){
		char ch = str[mov_index];
		if( ch == '_'){
			mov_index++;
		}
		else if( is_lower_alpha(ch) || is_cap_alpha(ch) ){
			mov_index++;	
		}
		else if( is_digit(ch) ){
			mov_index++;	
		}
		else{
			yytext = (char *)malloc( (mov_index - index_lex + 1) * sizeof(char));
			string_copy(str, index_lex, (mov_index - index_lex), yytext);
			printf("yytext : %s\n", yytext);
			index_lex = mov_index;
			break;
		}
	}
	return true;
}

void string_copy(char *src, int start, int size, char *dest){
	for(int  i = start, j = 0; i <= size; i++, j++){
		dest[j] = src[i];
	}
	dest[size+1] = '\0';
}

bool is_num(char *str){
	int mov_index = index_lex;
	while(1) {
		char ch = str[mov_index];
		if( is_digit(ch) ){
			mov_index++;
		}
		else{
			if(index_lex == mov_index){
				return false;
			}
			else{
				yytext = (char *)malloc( (mov_index - index_lex + 1) * sizeof(char));
				string_copy(str, index_lex, (mov_index - index_lex), yytext);
				printf("yytext : %s\n", yytext);
				index_lex = mov_index;
				return true;
			}
		}
	}
}

bool is_operator(char *str){
	char ch = str[index_lex];
	if(ch == '+' || ch == '-' || ch == '=' || ch == '*' || ch == '\0'){
			index_lex++;
			yytext = (char *)malloc( 2 * sizeof(char));
			string_copy(str, index_lex, 2, yytext);
			printf("yytext : %s\n", yytext);
			return true;
	}
	return false;
}

bool is_ws( char *str ){
	char ch = str[index_lex];
	if( ch == ' ' || ch == '\t'){
		index_lex++;
		yytext = (char *)malloc( 2 * sizeof(char));
		string_copy(str, index_lex, 2, yytext);
		printf("yytext : %s\n", yytext);
		return true;
	}
	return false;
	
}
