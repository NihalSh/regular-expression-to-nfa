#include <stdlib.h>

#include "stack.h"

int stack_init(Stack *stack)
{
	stack->top = -1;
	stack->array = NULL;
	return 0;
}

int stack_destroy(Stack *stack)
{
	stack->top = -1;
	if (!stack->array) {
		free(stack->array);
		stack->array = NULL;
	}
	return 0;
}

int stack_push(Stack *stack, void * ptr)
{
	stack->top++;
	stack->array = (void **) realloc(stack->array, (stack->top + 1) * sizeof(void *));
	stack->array[stack->top] = ptr;
	return 0;
}

int stack_pop(Stack *stack, void ** ptr)
{
	if (stack->top > -1) {
		*ptr = stack->array[stack->top];
		stack->array = (void **) realloc(stack->array, stack->top * sizeof(void *));
		stack->top--;
	} else {
		*ptr = NULL;
	}
	return 0;
}