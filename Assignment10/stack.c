#include "stack.h"

struct Stack* createStack(){
    struct Stack* stack = (struct Stack*) malloc(sizeof(sizeof(struct Stack)));
    stack->capacity = MAX;
    stack->top = -1;
    return stack;
}

int isFull(struct Stack* stack){
    return stack->top == stack->capacity-1;
}


int isEmpty(struct Stack* stack)
{
    return stack->top == -1;
}
 
void push(struct Stack* stack, char item)
{
    if (isFull(stack))
        return;
    stack->array[++stack->top] = item;
    // printf("%c pushed to stack\n", item);
}

char pop(struct Stack* stack)
{
    if (isEmpty(stack))
        return '\0';
    return stack->array[stack->top--];
}
 

char peek(struct Stack* stack)
{
    if (isEmpty(stack))
        return '\0';
    return stack->array[stack->top];
}

void print_stack(struct Stack* stack){
    for(int i=0;i<=stack->top;i++){
        printf("%c",stack->array[i]);
    }
}
