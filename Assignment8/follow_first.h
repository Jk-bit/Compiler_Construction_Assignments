#ifndef _FOLLOW_FIRST_H
#define _FOLLOW_FIRST_H 1


#include<stdio.h>
#include<ctype.h>
#include<string.h>


/* Helper Functions */
void read_non_terminals();
int is_terminal(char ch);
int is_non_terminal(char ch);
int is_epsilon(char ch);
int containts(char *array,char ch, int count);
int get_non_terminal_index(char ch);

/* FIRST and FOLLOWS */
void calculate_firsts();
void calculate_follows();
void find_firsts(char ch);
void find_follows(char ch);
int contains_epsilon_in_first(char ch);
void print_firsts();
void print_follows();

#endif
