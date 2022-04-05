#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
 
#define MAX 30

struct Stack{
    char array[MAX];
    int top;
    int capacity;
};

struct Stack* createStack(void);
int isFull(struct Stack* stack);
int isEmpty(struct Stack* stack);
void push(struct Stack* stack, char item);
char pop(struct Stack* stack);
char peek(struct Stack* stack);
 
void print_stack(struct Stack* stack);