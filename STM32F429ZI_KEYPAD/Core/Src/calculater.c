#include "keypad.h"
#include "circularQueue.h"
#include "button.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern Queue keypad_queue;

void get_mathequation(void);
void calculator_stm(char *postfix);
void in_to_post(char* infix, char* postfix);

#define STACK_SIZE 100

typedef double element;

typedef struct
{
	element stack[STACK_SIZE];
	int top;
} StackType;

void init(StackType* s)
{
	s->top = -1;
}

int is_empty(StackType* s)
{
	return (s->top == -1);
}

int is_full(StackType* s)
{
	return (s->top == (STACK_SIZE - 1));
}

void push(StackType* s, element item)
{
	if (is_full(s))
	{
		//fprintf(stderr, "Stack overflow error\n");
		return;
	}
	else {
		s->stack[++(s->top)] = item;
	}
}

element peek(StackType* s)
{
	if (is_empty(s))
	{
		//fprintf(stderr, "Stack underflow error\n");
		exit(1);
	}
	else
	{
		return s->stack[s->top];
	}
}

element pop(StackType* s)
{
	if (is_empty(s))
	{
		//printf(stderr, "Stack underflow error\n");
		exit(1);
	}
	else
	{
		return s->stack[(s->top)--];
	}
}

int prec(char op)
{
	switch (op)
	{
	case '(': case ')': return 0;
	case '+': case '-': return 1;
	case '*': case '/': return 2;
	}
	return -1;
}

void in_to_post(char* infix, char* postfix)
{
	StackType s;
	init(&s); // Declare and initialize stack

	while (*infix != '\0')
	{ // Repeat until NULL value in input string
		if (*infix == ' ')
		{
			infix++; // Ignore ' ' space
		}
		else if (*infix == '(')
		{
			push(&s, *infix);
			infix++;
		}
		else if (*infix == ')')
		{
			while (!is_empty(&s) && peek(&s) != '(')
			{
				*postfix++ = pop(&s);
				*postfix++ = ' ';
			}
			if (!is_empty(&s) && peek(&s) == '(')
				pop(&s);


			infix++;

		}
		else if (*infix == '+' || *infix == '-' || *infix == '*' || *infix == '/')
		{
			while (!is_empty(&s) && (prec(*infix) <= prec(peek(&s))))
			{
				*postfix++ = pop(&s);
				*postfix++ = ' ';
			}
			push(&s, *infix);
			infix++;
		}
		else if (*infix >= '0' && *infix <= '9')
		{
			do
			{
				*postfix++ = *infix++;
			} while (*infix >= '0' && *infix <= '9');
			*postfix++ = ' ';
		}
		else
		{
			infix++;
		}
	}
	while (!is_empty(&s))
	{
		*postfix++ = pop(&s);
		*postfix++ = ' ';
	}
	postfix--;
	*postfix = '\0';
}

void calculator_stm(char *postfix)
{
#if 1 // use stack
	static int num = 0;
	int num1, num2;

	StackType s;
	init(&s); // Declare and initialize stack

	for (int i = 0; i < strlen(postfix); i++)
	{
		if (postfix[i] >= '0' && postfix[i] <= '9')
		{
			num = num * 10 + (postfix[i] - '0');
			if (postfix[i+1] == ' ')
			{
				push(&s, num);
				num = 0;
				i++;
			}
		}

		else if(postfix[i] == '+')
		{
			num2 = pop(&s);
			num1 = pop(&s);
			push(&s, num1 + num2);
		}

		else if(postfix[i] == '-')
		{
			num2 = pop(&s);
			num1 = pop(&s);
			push(&s, num1 - num2);
		}

		else if(postfix[i] == '*')
		{
			num2 = pop(&s);
			num1 = pop(&s);
			push(&s, num1 * num2);
		}

		else if(postfix[i] == '/')
		{
			num2 = pop(&s);
			num1 = pop(&s);
			push(&s, num1 / num2);
		}
	}

	num = pop(&s);
	printf("result : %d\n", num);


#else
	static int num1 = 0, num2 = 0, state = 0;

	printf("Postfix : %s\n", postfix);
	printf("%d\n", strlen(postfix));

	for (int i = 0; i < strlen(postfix); i++)
	{
		if (postfix[i] >= '0' && postfix[i] <= '9')
		{
			if (state == 0)
			{
				num1 = num1 * 10 + (postfix[i] - '0');
			}
			else if (state == 1)
			{
				num2 = num2 * 10 + (postfix[i] - '0');
			}
			else if (state == 2)
			{
				num1 = num2;
				num2 = 0;
				num2 = num2 * 10 + (postfix[i] - '0');
				state = 1;
			}
		}

		else if (postfix[i] == ' ')
		{
			state++;
			printf("spcae!!!\n");
		}

		else if(postfix[i] == '+')
		{
			num1 = num1 + num2;
			num2 = 0;
			state = 1;
			i++;
			printf("%c\n", postfix[i]);
			printf("result : %d\n", num1);
		}

		else if(postfix[i] == '-')
		{
			num1 = num1 - num2;
			num2 = 0;
			state = 1;
			i++;
			printf("%c\n", postfix[i]);
			printf("result : %d\n", num1);
		}

		else if(postfix[i] == '*')
		{
			num1 = num1 * num2;
			num2 = 0;
			state = 1;
			i++;
			printf("%c\n", postfix[i]);
			printf("result : %d\n", num1);
		}

		else if(postfix[i] == '/')
		{
			num1 = num1 / num2;
			num2 = 0;
			state = 1;
			i++;
			printf("%c\n", postfix[i]);
			printf("result : %d\n", num1);
		}

	}
	printf("result : %d\n", num1);
#endif
}

void get_mathequation(void)
{
	static char input[100];
	static char postfix[100];
	static int input_index = 0;

	if (QIsEmpty(&keypad_queue) != TRUE)
	{
		uint8_t data;
		data = Dequeue(&keypad_queue);

		input[input_index] = data;
		printf("Entered : %s\n", input);
		input_index++;

		if (data == '=')
		{
			input[strcspn(input, "\n")] = '\0';

			in_to_post(input, postfix);
			calculator_stm(postfix);
			printf("Postfix result : %s\n", postfix);

			memset(input, 0, sizeof(input));
						input_index = 0;
		}

	}

	if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, BUTTON0) == BUTTON_PRESS)
	{
		input[input_index] = '(';
		printf("Entered : %s\n", input);
		input_index++;
	}

	if (get_button(BUTTON1_GPIO_Port, BUTTON1_Pin, BUTTON1) == BUTTON_PRESS)
	{
		input[input_index] = ')';
		printf("Entered : %s\n", input);
		input_index++;
	}

}