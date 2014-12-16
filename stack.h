#ifndef STACK_H_INC
#define STACK_H_INC
#include <stdio.h>
#define STACK_MAX 2097152

struct Stack {
	char    data[STACK_MAX];
	int     size;
};
typedef struct Stack Stack;

void Stack_Push(Stack *S, char d);
void Stack_Pop(Stack *S);
void Stack_Init(Stack *S);
char Stack_Top(Stack *S);
#endif
