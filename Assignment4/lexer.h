
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define INPUT_SIZE	4096

/*int index_lex = 0;
char *yytext;
int yylen;
char operator_list[] = {'+', '-', '=', '*', '/'}; */

enum tokens {VARIABLE, NUMBER, OPERATOR, WS, INVALID};

void lexer(char *str);

int return_token(char *str);

bool is_lower_alpha(char ch);

bool is_cap_alpha(char ch);

bool is_digit(char ch);

bool is_var(char *str);

bool is_num(char *str);

bool is_operator(char *str);

void string_copy(char *src, int start, int size, char *dest);

bool is_ws( char *str );

void handle_invalid(char *str);
