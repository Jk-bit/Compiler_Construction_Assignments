#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include "stack.h"

/* Global Variable */
char set_firsts[10][100];
char set_follow[10][100];
char production[10][100];
int is_first_done[10], is_follows_done[10];
char non_terminals[10], terminals[10];
int count_non_terminals=0, count_terminals=0,count_productions ;
char START_SYMBOl;

/* Utility Functions */
void read_non_terminals();
void read_terminals();
int is_terminal(char ch);
int is_non_terminal(char ch);
int is_epsilon(char ch);
int containts(char *array,char ch, int count);
int get_non_terminal_index(char ch);
int get_terminal_index(char ch);


/* FIRST and FOLLOWS */
void calculate_firsts();
void calculate_follows();
void find_firsts(char ch);
void find_follows(char ch);
int contains_epsilon_in_first(char ch);
void print_firsts();
void print_follows();


/* Parsing Table Generation */
int parsing_table[10][10][10];
int parsting_table_entries_count[10][10];
void create_parsing_table();
int is_dollar_in_follow(char ch);
void print_parsing_table();


/* LL1 Parser */
int cal_length(char* input_string);
int parse(char* input_string, int length);
int get_parse_table_entry(char X,char a);
void print_parser_state(struct Stack* stack, char* input_string, int input_idx, int production_rule_idx);



int main(){

	// count_productions = 8;	
	// strcpy(production[0], "E=TR");
	// strcpy(production[1], "R=+TR");
	// strcpy(production[2], "R=#");
	// strcpy(production[3], "T=FY");
	// strcpy(production[4], "Y=*FY");
	// strcpy(production[5], "Y=#");
	// strcpy(production[6], "F=(E)");
	// strcpy(production[7], "F=i");
	// char input_string[20] = "i+i*i";

	// char input_string[20] = "i+i*i";
	// char input_string[20] = "i+i*i+i";
	// char input_string[20] = "i+i*";
    

	count_productions = 3;	
	strcpy(production[0], "S=F");
	strcpy(production[1], "S=(S+F)");
	strcpy(production[2], "F=a");
	char input_string[20] = "(a+a)";

	// count_productions = 3;	
	// strcpy(production[0], "S=A");
	// strcpy(production[1], "S=a");
	// strcpy(production[2], "A=a");
	// // strcpy(production[3], "B=d");
	// // strcpy(production[4], "E=b");
	// // strcpy(production[5], "F=(E)");
	// // strcpy(production[6], "F=i");
	// // strcpy(production[7], "F=i");
	// char input_string[20] = "a";
	
	read_non_terminals();
	calculate_firsts();
	print_firsts();
	
	calculate_follows();
	print_follows();

    read_terminals();
    create_parsing_table();
    print_parsing_table();

	
    int length = cal_length(input_string);

    int accepted = parse(input_string, length);
    if(accepted){
        printf("\n STRING ACCEPTED\n");
    }else{
        printf("\n STRING  NOT ACCEPTED\n");
    }
	return 0;
}



/*
    1. For each production A -> B, do steps 2 and 3;
    2. For each terminal a in FIRST(B), add A -> B to M| A,a|
    3. If epsilon is in FIRST(B), add A->B to M|A,b| for each terminal b in
        FOLLOW(A). If epsilon is in FIRST(B) and $ is in FOLLOW(A), add A->B to M|A,$| 

*/
void create_parsing_table(){
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            for(int k=0;k<10;k++){
                parsing_table[i][j][k] = -1;
            }
        }
    }

    for(int production_idx=0;production_idx<count_productions;production_idx++){
        char lhs_non_terminal,rhs_element ;

        // Get LHS(non-terminal) and RHS element(non-terminal or terminal)
        lhs_non_terminal = production[production_idx][0];
        int lhs_index = get_non_terminal_index(lhs_non_terminal);
        rhs_element = production[production_idx][2];

        // Step 2: For each element in FIRST(RHS). add entry in parsing table
		if(is_epsilon(rhs_element)){
			int idx_in_follow = 0;
            char follow_char = set_follow[lhs_index][idx_in_follow];
            while(follow_char != '\0'){
				int index_of_follow = get_terminal_index(follow_char);
				int count_of_entries = parsting_table_entries_count[lhs_index][index_of_follow];
				parsing_table[lhs_index][index_of_follow][count_of_entries] = production_idx;
				parsting_table_entries_count[lhs_index][index_of_follow] = count_of_entries+1;

				idx_in_follow++;
				follow_char = set_follow[lhs_index][idx_in_follow];
			}
			if(is_dollar_in_follow(lhs_non_terminal)){
				parsing_table[lhs_index][count_terminals][0] = production_idx;
			}
		}
        else if(is_terminal(rhs_element)){
            int index_of_first = get_terminal_index(rhs_element);
            int count_of_entries = parsting_table_entries_count[lhs_index][index_of_first];
            parsing_table[lhs_index][index_of_first][count_of_entries] = production_idx;
            parsting_table_entries_count[lhs_index][index_of_first] = count_of_entries+1;
        }else if(is_non_terminal(rhs_element)){
            int temp_idx = 0;
            int rhs_index  = get_non_terminal_index(rhs_element);
            char first_char = set_firsts[rhs_index][temp_idx];
            while(first_char != '\0'){

                if(first_char == '#'){
                    
                    int idx_in_follow = 0;
                    char follow_char = set_follow[lhs_index][idx_in_follow];
                    while(follow_char != '\0'){
                        int index_of_follow = get_terminal_index(follow_char);
                        int count_of_entries = parsting_table_entries_count[lhs_index][index_of_follow];
                        parsing_table[lhs_index][index_of_follow][count_of_entries] = production_idx;
                        parsting_table_entries_count[lhs_index][index_of_follow] = count_of_entries+1;

                        idx_in_follow++;
                        follow_char = set_follow[lhs_index][idx_in_follow];
                    }
                    if(is_dollar_in_follow(lhs_non_terminal)){
                        parsing_table[lhs_index][count_terminals][0] = production_idx;
                    }
                    continue;
                }
                int index_of_first = get_terminal_index(first_char);
                int count_of_entries = parsting_table_entries_count[lhs_index][index_of_first];
                parsing_table[lhs_index][index_of_first][count_of_entries] = production_idx;
                parsting_table_entries_count[lhs_index][index_of_first] = count_of_entries+1;
                temp_idx++;
                first_char =  set_firsts[rhs_index][temp_idx];
            }
        }

        // Step 3: 

    }
}

void print_parsing_table(){
    printf("PARSING TABLE:\n\n");
    printf("\t\t");
    for(int i=0;i<count_terminals;i++){
        printf("%c\t\t", terminals[i]);
    }
	printf("$\t\t");
    printf("\n\n");

    for(int non_terminal_idx=0;non_terminal_idx<count_non_terminals;non_terminal_idx++){
        printf("%c\t\t", non_terminals[non_terminal_idx]);
        for(int terminal_idx=0;terminal_idx<count_terminals;terminal_idx++){
            int entries_count = parsting_table_entries_count[non_terminal_idx][terminal_idx];
            for(int k=0;k<entries_count;k++){
                int production_index = parsing_table[non_terminal_idx][terminal_idx][k];
                printf("%s, ", production[production_index]);
            }
			printf("\t\t");
        }
		// $ productions
		int production_index = parsing_table[non_terminal_idx][count_terminals][0];
		printf("%s", production[production_index]);
        printf("\n\n");
    }    
	printf("########################################################################################################################\n");
}

int is_dollar_in_follow(char ch){
    int temp_idx = 0;
    int index_non_terminal = get_non_terminal_index(ch);
    char follow_char = set_follow[index_non_terminal][temp_idx];
    while(follow_char != '\0'){
        if(follow_char == '#'){
            return 1;
        }

        temp_idx++;
        follow_char = set_follow[index_non_terminal][temp_idx];
    }
    return 0;
}

void print_firsts(){
	
	for(int i=0;i<count_non_terminals;i++){
		printf("FIRST(%c) => ", non_terminals[i]);
		int count = 0;
		char temp_ch = set_firsts[i][count];
		while(temp_ch != '\0'){
			count++;
			temp_ch = set_firsts[i][count];
		}
		printf("{ ");
		for(int j=0;j<count-1;j++){
			printf("%c, ", set_firsts[i][j]);
		}
		printf("%c }\n", set_firsts[i][count-1]);
	}
	printf("\n\n\n");
}


void print_follows(){
	for(int i=0;i<count_non_terminals;i++){
		printf("FOLLOW(%c) => ", non_terminals[i]);
		int count = 0;
		char temp_ch = set_follow[i][count];
		while(temp_ch != '\0'){
			count++;
			temp_ch = set_follow[i][count];
		}
		printf("{ ");
		for(int j=0;j<count-1;j++){
			printf("%c, ", set_follow[i][j]);
		}
		printf("%c }\n", set_follow[i][count-1]);
	}
	printf("\n\n\n");
}

void find_follows(char ch){
	char nt_productions[10][100];
	int count = 0;
	for(int i=0;i<count_productions;i++){
		if(production[i][0] == ch){
			strcpy(nt_productions[count], production[i]+2);
			count++;
		}
	}
	char follows_set[10];
	int count_of_follows = 0;
	if(ch == START_SYMBOl){
		follows_set[count_of_follows] = '$';
		count_of_follows++;
	}

	/* RULE 2: Searchig for the pattern : A -> aBC
	everything in FIRST(C) except for epsilon is placed in FOLLOW(B)
	*/

	for(int production_index=0;production_index<count_productions;production_index++){
		int production_end = 0;
		char temp_ch = production[production_index][production_end];
		while(temp_ch != '\0'){
			production_end++;
			temp_ch = production[production_index][production_end];
		}

		for(int i=2;i<production_end;i++){
			char current_char = production[production_index][i];
			if(current_char == ch ){
				char c = production[production_index][i+1];
				if(c != '\0')
				{
					/* everything in FIRST(C) except for epsilon is placed in FOLLOW(B) */
					if(is_terminal(c))
					{
						follows_set[count_of_follows] = c;
						count_of_follows++;
					}
					else if(is_non_terminal(c))
					{
						int index_C = get_non_terminal_index(c);
						int k = 0;
						char first = set_firsts[index_C][k];
						while(first != '\0'){
							if(first != '#'){
								if(containts(follows_set, first, count_of_follows) != 1){
									follows_set[count_of_follows] = first;
									count_of_follows++;
								}
								
							}
							k++;
							first = set_firsts[index_C][k];
						}
					}
										
				}
			}
		}
	}

	/* RULE 3:	if there is a prodcution A -> mB or a production A -> mBC where FIRST(C) conatins epsilon,
				then everything in FOLLOW(A) is in FOLLOW(B)
	*/
	for(int production_index=0;production_index<count_productions;production_index++){
		int production_end = 0;
		char temp_ch = production[production_index][production_end];
		while(temp_ch != '\0'){
			production_end++;
			temp_ch = production[production_index][production_end];
		}

		for(int i=2;i<production_end;i++){
			char current_char = production[production_index][i];
			if(current_char == ch){
				char c = production[production_index][i+1];
				if(c == '\0' || contains_epsilon_in_first(c)){
					char lhs_non_terminal = production[production_index][0];
					if(lhs_non_terminal == ch){
						continue;
					}
					int lhs_non_terminal_idx = get_non_terminal_index(lhs_non_terminal);
					if(is_follows_done[lhs_non_terminal_idx] == 1){
						;
					}else{
						find_follows(lhs_non_terminal);
						is_follows_done[lhs_non_terminal_idx] = 1;
					}
					int temp = 0;
					char temp_ch = set_follow[lhs_non_terminal_idx][temp];
					while(temp_ch != '\0'){
						if(containts(follows_set, temp_ch, count_of_follows) != 1){
							follows_set[count_of_follows] = temp_ch;
							count_of_follows++;
						}
						
						temp++;
						temp_ch  = set_follow[lhs_non_terminal_idx][temp];
					}
				}	
			}
		}
	}

	follows_set[count_of_follows]  = '\0';
	int index = get_non_terminal_index(ch);
	strcpy(set_follow[index], follows_set);
}

int containts(char *array,char ch, int count){
	for(int i=0;i<count;i++){
		if(array[i] == ch) return 1;
	}
	return 0;
}

int contains_epsilon_in_first(char ch){
	if(is_terminal(ch)){
		return 0;
	}
	int non_terminal_index = get_non_terminal_index(ch);
	int j = 0;
	char temp_ch = set_firsts[non_terminal_index][j];
	while(temp_ch != '\0'){
		if(temp_ch == '#'){
			return 1;
		}
		j++;	
		temp_ch = set_firsts[non_terminal_index][j];
	}
	return 0;
}


void calculate_follows(){
	for(int non_terminal_index=0;non_terminal_index<count_non_terminals;non_terminal_index++){
		char non_terminal = non_terminals[non_terminal_index];
		if(is_follows_done[non_terminal_index] == 0){
			find_follows(non_terminal);
			is_follows_done[non_terminal_index] = 1;
		}
	}
}



void calculate_firsts(){
	for(int non_terminal_index=0;non_terminal_index<count_non_terminals;non_terminal_index++){
		char non_terminal = non_terminals[non_terminal_index];
		if(is_first_done[non_terminal_index] == 0){
			find_firsts(non_terminal);
			is_first_done[non_terminal_index] = 1;
		}
		
	}
}

int get_non_terminal_index(char ch){
	int index = -1;
	for(int i=0;i<count_non_terminals;i++){
		if(non_terminals[i] == ch){
			index = i;
			break;
		}
	}
	return index;
}

int get_terminal_index(char ch){
	if(ch == '$'){
		return count_terminals;
	}
    int index = -1;
    for(int i=0;i<count_terminals;i++){
        if(terminals[i] == ch){
            index = i;
            break;
        }
    }
    return index;
}


void find_firsts(char ch){
	char nt_productions[10][100];
	int count = 0;
	for(int i=0;i<count_productions;i++){
		if(production[i][0] == ch){
			strcpy(nt_productions[count], production[i]+2);
			count++;
		}
	}
	char firsts_set[10];
	int count_of_firsts = 0;
	for(int production_no=0;production_no<count;production_no++){
		char first_char = nt_productions[production_no][0];
		if(is_terminal(first_char) == 1){
			firsts_set[count_of_firsts] = first_char;
			count_of_firsts++;
		}
		if(is_epsilon(first_char)){
			firsts_set[count_of_firsts] = first_char;
			count_of_firsts++;
		}
		if(is_non_terminal(first_char)){
			if(first_char == ch){
				return ;
			}
			// first_char is non_terminal here. Recursively calculate first of it 
			// then add set in parent
			
			int index = get_non_terminal_index(first_char);
			if(is_first_done[index] == 0){
				find_firsts(first_char);
				is_first_done[index] = 1;
			}
			int i = 0;
			char ch = set_firsts[index][i];
			while(ch != '\0'){
				firsts_set[count_of_firsts] = ch;
				count_of_firsts++;
				i++;
				ch = set_firsts[index][i];
			}
		}
	}
	firsts_set[count_of_firsts] = '\0';
	int index = get_non_terminal_index(ch);
	strcpy(set_firsts[index], firsts_set);
}



int is_epsilon(char ch){
	if(ch == '#')return 1;
	return 0;
}

void read_non_terminals(){
	for(int i=0;i<count_productions;i++){
		char first_char = production[i][0];
		if(is_non_terminal(first_char)){
			if(i == 0){
				START_SYMBOl = first_char;
			}
			int flag = 0;
			for(int j=0;j<count_non_terminals;j++){
				if(non_terminals[j] == first_char){
					flag = 1;
					break;
				}
			}
			if(flag == 0){
				non_terminals[count_non_terminals] = first_char; 
				count_non_terminals++;
			}
			
		}
	}
}


void read_terminals(){
    for(int i=0;i<count_productions;i++){
        int production_size =0 ;
        char temp_ch = production[i][production_size];
        while(temp_ch != '\0'){
            production_size++;  
            temp_ch = production[i][production_size];
        }
        for(int j=0;j<production_size;j++){
            char ch = production[i][j];
            if(is_terminal(ch) && !containts(terminals, ch, count_terminals)){
                terminals[count_terminals] = ch;
                count_terminals++;

            }
        }
    }
}


int get_parse_table_entry(char X,char a){
	int non_terminal_index = get_non_terminal_index(X);
	int terminal_index = get_terminal_index(a);
	int production_rule_idx = parsing_table[non_terminal_index][terminal_index][0];
	return production_rule_idx;
}

void print_parser_state(struct Stack* stack, char* input_string, int input_idx, int production_rule_idx)
{
	print_stack(stack);
	printf("\t\t\t");
	int input_size = cal_length(input_string);
	for(int i=input_idx;i<input_size;i++){
		printf("%c", input_string[i]);
	}
	printf("\t\t\t");
	if(production_rule_idx != -1){
		int production_size = cal_length(production[production_rule_idx]);
		for(int i=0;i<production_size;i++){
			printf("%c",production[production_rule_idx][i]);
		}
	}
	printf("\n");
}

int parse(char* input_string, int size){
    int accepted = 0;
    /*Add $ at the end of the input string*/
    input_string[size] = '$';
    input_string[size+1] = '\0';
    size = size+1;
	/*push $ at the bottom of the  stack*/
	struct Stack* stack = createStack();
	push(stack, '$');
	push(stack, START_SYMBOl);

	/*
		X: Symbol on top of the stack
		a: current input symbol
	*/
	int input_idx = 0;
	char X = peek(stack);
	char a = input_string[input_idx];

	printf("STACK\t\t\tINPUT\t\t\tOUTPUT\n\n");
	print_parser_state(stack, input_string, input_idx, -1);
	while(1){
		if((X == a) && (a == '$')){
			print_parser_state(stack, input_string, input_idx, -1);
			return 1;
		}
		else if(X==a){
			pop(stack);
			X = peek(stack);
			input_idx++;
			a = input_string[input_idx];
			continue;
		}
		else{
			if(is_non_terminal(X)){
				int productio_rule_idx = get_parse_table_entry(X,a);
				print_parser_state(stack, input_string, input_idx, productio_rule_idx);
				if(productio_rule_idx == -1){
					printf("Error\n");
					return 0;
				}
				int production_size = cal_length(production[productio_rule_idx]);
				// printf("%s\n", production[productio_rule_idx]);
				pop(stack);
				for(int i=production_size-1;i>=2;i--){
					char ch = production[productio_rule_idx][i];
					if(ch != '#'){
						push(stack, ch);
					}
					
				}
				X = peek(stack);
			}
		}

	}
	return accepted;
}


int is_terminal(char ch){
	if(ch >= 'a' && ch <= 'z')return 1;
	char str[5] = "-+*()";
	for(int i=0;i<5;i++){
		if(ch == str[i]) return 1;
	}
	return 0;
}


int is_non_terminal(char ch){
	if(ch >= 'A' && ch <= 'Z')return 1;
	else return 0;
}

int cal_length(char* input_string){
    int i = 0;
    char ch = input_string[i];
    while(ch != '\0'){
        i++;
        ch = input_string[i];
    }
    return i;
}


