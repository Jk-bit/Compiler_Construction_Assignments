#include "follow_first.h"

char set_firsts[10][100];
char set_follow[10][100];
char production[10][100];
int is_first_done[10], is_follows_done[10];
char non_terminals[10], terminals[10];
int count_non_terminals=0, count_terminals=0,count_productions=0;
char START_SYMBOl;


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
	// similar to initialization part in find_firsts
	for(int i=0;i<count_productions;i++){
		if(production[i][0] == ch){
			strcpy(nt_productions[count], production[i]+2);
			count++;
		}
	}
	char follows_set[10];
	int count_of_follows = 0;
	// RULE 1 : Start symbol contains $
	if(ch == START_SYMBOl){
		follows_set[count_of_follows] = '$';
		count_of_follows++;
	}

	/* RULE 2: Searching for the pattern : A -> aBC
	everything in FIRST(C) except for epsilon is placed in FOLLOW(B)
	*/
	
	// Traversing through all the productions
	for(int production_index=0;production_index<count_productions;production_index++){
		int production_end = 0;
		char temp_ch = production[production_index][production_end];
		// Getting the length of the particular production
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
					// copying everything in first(C) in follow(B) except epsilon(#)
					else if(is_non_terminal(c))
					{
						int index_C = get_non_terminal_index(c);
						int k = 0;
						char first = set_firsts[index_C][k];
						while(first != '\0'){
							if(first != '#'){
								//checking if the follow_set contains the character first
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
					// checking if follow A has been completed
					if(is_follows_done[lhs_non_terminal_idx] == 1){
						;
					}else{
						find_follows(lhs_non_terminal);
						is_follows_done[lhs_non_terminal_idx] = 1;
					}
					// copying everything in follow(A) into follow(B)
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


// Traversing through the non_terminal_array and calculating their firsts
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


void find_firsts(char ch){
	char nt_productions[10][100];
	int count = 0;
	// Getting count of productions and storing them with RHS
	// in nt_productions
	for(int i=0;i<count_productions;i++){
		if(production[i][0] == ch){
			strcpy(nt_productions[count], production[i]+2);
			count++;
		}
	}
	char firsts_set[10];
	int count_of_firsts = 0;
	// Traversing through nt_productions and checking the first char
	// of RHS.
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
			// looking for firsts of that first_char
			if(is_first_done[index] == 0){
				find_firsts(first_char);
				is_first_done[index] = 1;
			}
			int i = 0;
			char ch = set_firsts[index][i];
			// copying the firsts of first_char into our array indexed by terminal
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


// stores the number of non terminal present in the input grammar
void read_non_terminals(){
	for(int i=0;i<count_productions;i++){
		char first_char = production[i][0];
		if(is_non_terminal(first_char)){
			if(i == 0){
				START_SYMBOl = first_char;
			}
			int flag = 0;
			// checking if the non terminal was present earlier
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


#include "parsing_table.h"

int parsing_table[10][10][10];
int parsting_table_entries_count[10][10];


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

