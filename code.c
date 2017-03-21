#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int max;
	int top;
	char *array;
} Stack;

int initialize(Stack *stack, int max)
{
	stack->max = max;
	stack->top = -1;
	stack->array = (char*) malloc(max*sizeof(char));
	return 0;
}
int push(Stack *stack, char c)
{
	if (stack->top < stack->max - 1) {
		stack->top++;
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

typedef struct {
	int id;
	char c;
} InputSymbol;

char * getInput() {
	char c;
	char *str = NULL;
	int lenstr = 0;
	printf("Regular Expression: ");
	while ((c = getchar()) != '\n'){
		lenstr++;
		str = (char *) realloc(str, sizeof(char)*(lenstr));
		str[lenstr - 1] = c;
	}
	str = (char *) realloc(str, sizeof(char)*(lenstr + 1));
	str[lenstr] = '\0';

	return str;
}

int main()
{
	char *str = NULL;
	Stack stack;
	InputSymbol input_symbols;
	initialize(&stack, 10);

	str = getInput();
	printf("%s\n", str);
	return 0;
}
