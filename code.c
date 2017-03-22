#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int max;
	int top;
	void ** array;
} Stack;

int initialize(Stack *stack, int max);

int push(Stack *stack, void * ptr);

void * pop(Stack *stack);

void * peek(Stack *stack);

typedef struct {
	int id;
	char c;


} InputSymbol;

typedef struct {
	int id;
	int * transitions;//will store state ids
} State;

typedef struct {
	State start;
	State final;
} NFA;

char * getInput();

int ifInputSymbol(char c);

int prepareInputSymbols(char * str, InputSymbol * symbols);

int main()
{
	char *str = NULL;
	InputSymbol symbols[27];
	int i;
	int numberInputSymbols;

	State * states = NULL;
	int numberStates = 0;

	Stack symbolStack;//will hold 'InputSymbol'
	Stack opStack;//will hold 'char'
	initialize(&symbolStack, 10);
	initialize(&opStack, 10);
	str = getInput();
	numberInputSymbols = prepareInputSymbols(str, symbols);
	printf("%d\n", numberInputSymbols);
	for(i = 0; i < strlen(str); i++) {
		if (ifInputSymbol(str[i])) {

		} else if (opStack.top == -1) {

		} else if (str[i] == '(') {

		} else if (str[i] == ')') {

		} else {

		}
	}

	return 0;
}

int evaluate(Stack symbolStack, Stack opStack, State * states, int * numberStates, int numberInputSymbols)
{

	return 0;
}

int pushInputSymbol(char c, Stack symbolStack, InputSymbol * symbols, int * numberStates, int numberInputSymbols)
{
	State * s1;
	State * s2;
	int i;

	//create state 1
	s1 = (State *) malloc(sizeof(State));
	*numberStates = *numberStates + 1;
	s1->id = *numberStates;
	s1->transitions = (int *) malloc(numberInputSymbols*sizeof(int));
	for (i = 0; i < numberInputSymbols; i++) {
		s1->transitions[i] = 0;
	}

	//create state 2
	s2 = (State *) malloc(sizeof(State));
	*numberStates = *numberStates + 1;
	s2->id = *numberStates;
	s2->transitions = (int *) malloc(numberInputSymbols*sizeof(int));
	for (i = 0; i < numberInputSymbols; i++) {
		s2->transitions[i] = 0;
	}

	//create transition
	s1->transitions[symbols[c - 96].id] = s2->id;
	
	//push NFA
	return 0;
}

int ifInputSymbol(char c)
{
	if ((c > 96) && (c < 123)) {
		return 1;
	} else {
		return 0;
	}
}

int ifOperationSymbol(char c)
{
	if ((c == '|') || (c == '.') || (c == '|')) {
		return 1;
	} else {
		return 0;
	}
}

int prepareInputSymbols(char * str, InputSymbol * symbols)
{
	int id = 1;
	int i = 0;
	symbols[0].id = 0;
	symbols[0].c = '0';
	printf("Input Symbols: \n");
	printf("\tepsilon -> %d\n", symbols[0].id);
	while(str[i] != '\0') {
		if(ifInputSymbol(str[i])) {
			symbols[str[i]-96].id = id;
			id++;
			symbols[str[i]-96].c = str[i];
			printf("\t%c -> %d\n", symbols[str[i]-96].c, symbols[str[i]-96].id);
		}
		i++;
	}
	return id;
}

char * getInput()
{
	char c;
	char *str = NULL;
	int lenstr = 0;
	printf("Regular Expression: ");
	while (((c = getchar()) != '\n') && (c != EOF)){
		lenstr++;
		str = (char *) realloc(str, sizeof(char)*(lenstr));
		str[lenstr - 1] = c;
	}
	str = (char *) realloc(str, sizeof(char)*(lenstr + 1));
	str[lenstr] = '\0';

	return str;
}

int initialize(Stack *stack, int max)
{
	stack->max = max;
	stack->top = -1;
	stack->array = (void**) malloc(max*sizeof(void*));
	return 0;
}

int push(Stack *stack, void * ptr)
{
	if (stack->top < stack->max - 1) {
		stack->top++;
		stack->array[stack->top] = ptr;
		return 0;
	} else {
		return -1;
	}
}

void * pop(Stack *stack)
{
	if (stack->top > -1) {
		void * ptr = stack->array[stack->top];
		stack->top--;
		return ptr;
	} else {
		return NULL;
	}
}

void * peek(Stack *stack)
{
	if (stack->top > -1) {
		return stack->array[stack->top];
	} else {
		return NULL;
	}
}