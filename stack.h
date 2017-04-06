#ifndef STACK_H
#define STACK_H

typedef struct {
	int top;
	void ** array;
} Stack;

int stack_init(Stack *stack);

int stack_destroy(Stack *stack);

int stack_push(Stack *stack, void * ptr);

int stack_pop(Stack *stack, void ** ptr);

#define stack_peek(stack) ((stack)->top < 0 ? NULL: (stack)->array[(stack)->top])

#define stack_size(stack) ((stack)->top + 1)

#endif