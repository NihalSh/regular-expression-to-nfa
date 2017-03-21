#include <stdio.h>
#include <stdlib.h>

typedef struct {
	unsigned int max;
	int top;
	char *array;
} Stack;

int initialize(Stack *stack, unsigned int max)
{
	stack->max = max;
	stack->top = -1;
	stack->array = (char*) malloc(max*sizeof(char *))
	return 0;
}
int push(Stack *stack, char c)
{
	if (stack->top < (stack->max - 1)) {
		stack->top = stack->top + 1;
		stack->array[stack->top] = c;
		return 0;
	} else {
		return -1;
	}
}
int pop(Stack *stack)
{
	if (stack->top > -1) {
		int c = stack->array[stack->top];
		stack->top--;
		return c;
	} else {
		return -1;
	}
}

int main()
{
	return 0;
}
