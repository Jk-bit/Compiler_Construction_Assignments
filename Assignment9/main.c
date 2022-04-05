#include "parsing_table.h"


extern int count_productions;
extern char production[10][100];


int main(){
	count_productions = 3;	
	strcpy(production[0], "S=F");
	strcpy(production[1], "S=(S+F)");
	strcpy(production[2], "F=a");

	/*count_productions = 8;	
	strcpy(production[0], "E=TR");
	strcpy(production[1], "R=+TR");
	strcpy(production[2], "R=#");
	strcpy(production[3], "T=FY");
	strcpy(production[4], "Y=*FY");
	strcpy(production[5], "Y=#");
	strcpy(production[6], "F=(E)");
	strcpy(production[7], "F=i"); */
	read_non_terminals();
	calculate_firsts();
	print_firsts();
	
	calculate_follows();
	print_follows();

    read_terminals();
    create_parsing_table();
    print_parsing_table();
	return 0;
}




