#include "stack.h"

void Stack_Init(Stack *S)
{
	S->size = 0;
}

char Stack_Top(Stack *S)
{
	if (S->size == 0) {
		fprintf(stderr, "Error: stack empty\n");
		return -1;
	} 
	return S->data[S->size-1];
}

void Stack_Push(Stack *S, char d)
{
	if (S->size < STACK_MAX)
		S->data[S->size++] = d;
	else
		fprintf(stderr, "Error: stack full\n");
}

void Stack_Pop(Stack *S)
{
	if (S->size == 0)
		fprintf(stderr, "Error: stack empty\n");
	else
		S->size--;
}
