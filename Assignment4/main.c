#include "lexer.h"



int main(){
	char input[INPUT_SIZE];
	while(1){
		scanf("%[^\n]%*c", input);
		lexer(input);
	}
	return 0;
}
