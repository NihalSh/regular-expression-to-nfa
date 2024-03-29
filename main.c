#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

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
	
	stack_init(symbolStack);
	stack_init(opStack);

	str = getInput();
	numberInputSymbols = prepareInputSymbols(str, symbols);

	for(i = 0; i < strlen(str); i++) {
		if (ifInputSymbol(str[i])) {
			pushInputSymbol(str[i], symbolStack, symbols, &states, &numberStates, numberInputSymbols);
		} else if (opStack->top == -1) {
			char * ptr = (char *) malloc(sizeof(char));
			*ptr = str[i];
			stack_push(opStack, ptr);
			//evaluate(symbolStack, opStack, &states, &numberStates, numberInputSymbols);
		} else if (str[i] == '(') {
			char * ptr = (char *) malloc(sizeof(char));
			*ptr = str[i];
			stack_push(opStack, ptr);
		} else if (str[i] == ')') {
			char * ptr = (char *) malloc(sizeof(char));
			while (*((char *) stack_peek(opStack)) != '(') {
				evaluate(symbolStack, opStack, &states, &numberStates, numberInputSymbols);
			}
			stack_pop(opStack, (void **) &ptr);
		} else {
			char * ptr = (char *) malloc(sizeof(char));
			*ptr = str[i];
			if (*((char *) stack_peek(opStack)) != '(') {
				//check precedence
				if (checkPrecedence(opStack, ptr)) {
					stack_push(opStack, ptr);
					evaluate(symbolStack, opStack, &states, &numberStates, numberInputSymbols);
				} else {
					evaluate(symbolStack, opStack, &states, &numberStates, numberInputSymbols);
					stack_push(opStack, ptr);
				}
			} else {
				stack_push(opStack, ptr);
			}
		}
	}

	for(i = 0; i < numberStates; i++) {
		printf("%2d", states[i]->id);
		for(j = 0; j < numberInputSymbols; j++) {
			printf("\t%2d", states[i]->transitions[j]);
		}
		printf("\n");
	}
	if (symbolStack->top == 0) {
		NFA * nfa = NULL;
		stack_pop(symbolStack, (void **) &nfa);
		printf("Start State: %d\n", nfa->start->id);
		printf("Final State: %d\n", nfa->final->id);
	}
	return 0;
}

int checkPrecedence(Stack * opStack, char *ptr) {
	if (*ptr == '$') {
		return 0;
	} else if (*((char *) stack_peek(opStack)) == *ptr) {
		return 0;
	} else if (*((char *) stack_peek(opStack)) == '*') {
		return 0;
	} else if (*ptr == '*') {
		return 1;
	} else if (*((char *) stack_peek(opStack)) == '|') {
		return 1;
	} else if (*ptr == '|') {
		return 0;
	}
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
	stack_pop(opStack, (void **) &op);
	//printf("%p\n", op);
	if (*op == '*') {
		NFA * new = (NFA *) malloc(sizeof(NFA));
		NFA * n = NULL;
		stack_pop(symbolStack, (void **) &n);
		State * s1;
		State * s2;
		int i;

		//create state 1
		s1 = createState(states, numberStates, numberInputSymbols);

		//create state 2
		s2 = createState(states, numberStates, numberInputSymbols);		

		n->final->transitions[0] = (n->start->id)*100 + s2->id;
		s1->transitions[0] = (n->start->id)*100 + s2->id;
		//free memory to avoid leak
		free(n);
		n = NULL;
		//push NFA later
		new->start = s1;
		new->final = s2;
		stack_push(symbolStack, (void *) new);
	} else if (*op == '.') {
		NFA * n2 = NULL;
		NFA * n1 = NULL;
		stack_pop(symbolStack, (void **) &n2);
		stack_pop(symbolStack, (void **) &n1);
		n1->final->transitions[0] = n2->start->id;
		//push NFA later
		n1->final = n2->final;
		free(n2);
		n2 = NULL;
		stack_push(symbolStack, (void *) n1);
	} else if (*op == '|') {
		NFA * new = (NFA *) malloc(sizeof(NFA));
		NFA * n2 = NULL;
		NFA * n1 = NULL;
		State * s1;
		State * s2;
		int i;

		stack_pop(symbolStack, (void **) &n2);
		stack_pop(symbolStack, (void **) &n1);
		//create state 1
		s1 = createState(states, numberStates, numberInputSymbols);

		//create state 2
		s2 = createState(states, numberStates, numberInputSymbols);		

		n1->final->transitions[0] = s2->id;
		n2->final->transitions[0] = s2->id;
		s1->transitions[0] = (n1->start->id)*100 + n2->start->id;
		//push NFA later
		new->start = s1;
		new->final = s2;
		stack_push(symbolStack, (void *) new);
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
	stack_push(symbolStack, n);

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
	int i;
	for (i = 0; i < 27; i++) {
		symbols[i].id = -1;
	}
	i = 0;
	symbols[0].id = 0;
	symbols[0].c = '0';
	printf("%2d", 00);
	printf("\tepsilon");
	while(str[i] != '\0') {
		if(ifInputSymbol(str[i])) {
			if (symbols[str[i]-96].id == -1) {
				symbols[str[i]-96].id = id;
				id++;
				symbols[str[i]-96].c = str[i];
				printf("\t%2c", symbols[str[i]-96].c);
			}
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
	str = (char *) realloc(str, sizeof(char)*(lenstr + 2));
	str[lenstr] = '$';
	str[lenstr + 1] = '\0';

	return str;
}