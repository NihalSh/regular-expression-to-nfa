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
	int * transitions;
} State;
//change it array of strings
typedef struct {
	State * start;
	State * final;
} NFA;

char * getInput();

int ifInputSymbol(char c);

int prepareInputSymbols(char * str, InputSymbol * symbols);

int pushInputSymbol(char c, Stack * symbolStack, InputSymbol * symbols, State *** states, int * numberStates, int numberInputSymbols);

int main()
{
	char *str = NULL;
	InputSymbol symbols[27];
	int i, j;
	int numberInputSymbols;

	State ** states = NULL;
	int numberStates = 0;

	Stack *symbolStack = (Stack *) malloc(sizeof(Stack));//will hold 'InputSymbol'
	Stack *opStack = (Stack *) malloc(sizeof(Stack));//will hold 'char'
	//printf("stack top: %d\n", symbolStack.top);
	//printf("stack top: %d\n", symbolStack.top);
	initialize(symbolStack, 10);
	initialize(opStack, 10);
	str = getInput();
	numberInputSymbols = prepareInputSymbols(str, symbols);

	for(i = 0; i < strlen(str); i++) {
		if (ifInputSymbol(str[i])) {
			pushInputSymbol(str[i], symbolStack, symbols, &states, &numberStates, numberInputSymbols);
		} else if (opStack->top == -1) {
			char * ptr = (char *) malloc(sizeof(char));
			*ptr = str[i];
			push(opStack, ptr);
			evaluate(symbolStack, opStack, &states, &numberStates, numberInputSymbols);
		} else if (str[i] == '(') {
			;
		} else if (str[i] == ')') {
			;
		} else {
			;
		}
	}

	for(i = 0; i < numberStates; i++) {
		printf("%2d", states[i]->id);
		for(j = 0; j < numberInputSymbols; j++) {
			printf("\t%2d", states[i]->transitions[j]);
		}
		printf("\n");
	}

	return 0;
}

State * createState(State *** states, int * numberStates, int numberInputSymbols)
{
	int i;
	//create new state
	State * s1 = (State *) malloc(sizeof(State));
	*numberStates = *numberStates + 1;
	s1->id = *numberStates;
	s1->transitions = (int *) malloc(numberInputSymbols*sizeof(int));
	for (i = 0; i < numberInputSymbols; i++) {
		s1->transitions[i] = 0;
	}

	//push to list of states
	*states = (State **) realloc(*states, sizeof(State *)*(*numberStates));
	(*states)[*numberStates-1] = s1;

	return s1;
}

int evaluate(Stack * symbolStack, Stack * opStack, State *** states, int * numberStates, int numberInputSymbols)
{
	char * op = NULL;
	op = (char *)pop(opStack);
	//printf("%p\n", op);
	if (*op == '*') {
		NFA * n = (NFA *)pop(symbolStack);
		State * s1;
		State * s2;
		int i;

		//create state 1
		s1 = createState(states, numberStates, numberInputSymbols);

		//create state 2
		s2 = createState(states, numberStates, numberInputSymbols);		

		n->final->transitions[0] = (n->start->id)*100 + s2->id;
		s1->transitions[0] = (n->start->id)*100 + s2->id;
		//push NFA later
	} else if (*op == '.') {
		//printf("%d\n", symbolStack->top);
		NFA * n2 = (NFA *)pop(symbolStack);
		NFA * n1 = (NFA *)pop(symbolStack);
		n1->final->transitions[0] = n2->start->id;
		//push NFA later
	} else if (*op == '|') {
		NFA * n2 = (NFA *)pop(symbolStack);
		NFA * n1 = (NFA *)pop(symbolStack);
		State * s1;
		State * s2;
		int i;

		//create state 1
		s1 = createState(states, numberStates, numberInputSymbols);

		//create state 2
		s2 = createState(states, numberStates, numberInputSymbols);		

		n1->final->transitions[0] = s2->id;
		n2->final->transitions[0] = s2->id;
		s1->transitions[0] = (n1->start->id)*100 + n2->start->id;
		//push NFA later
	} 


	return 0;
}

int pushInputSymbol(char c, Stack * symbolStack, InputSymbol * symbols, State *** states, int * numberStates, int numberInputSymbols)
{
	State * s1;
	State * s2;
	NFA * n;
	int i;

	//create state 1
	s1 = createState(states, numberStates, numberInputSymbols);

	//create state 2
	s2 = createState(states, numberStates, numberInputSymbols);

	//create transition
	s1->transitions[symbols[c - 96].id] = s2->id;
	
	//create NFA
	n = (NFA *) malloc(sizeof(NFA));
	n->start = s1;
	n->final = s2;

	//push NFA
	push(symbolStack, n);

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
	printf("%2d", 00);
	printf("\tepsilon");
	while(str[i] != '\0') {
		if(ifInputSymbol(str[i])) {
			symbols[str[i]-96].id = id;
			id++;
			symbols[str[i]-96].c = str[i];
			printf("\t%2c", symbols[str[i]-96].c);
		}
		i++;
	}
	printf("\n");
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
		stack->top = stack->top + 1;
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