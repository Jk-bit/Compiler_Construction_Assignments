
#include "follow_first.h"

extern int count_productions;
extern char production[10][100];

int main(){

	// Storing the input grammar	
	count_productions = 3;	
	strcpy(production[0], "S=F");
	strcpy(production[1], "S=(S+F)");
	strcpy(production[2], "F=a");
	
	
	read_non_terminals();
	calculate_firsts();
	print_firsts();
	
	calculate_follows();
	print_follows();
	return 0;
}



