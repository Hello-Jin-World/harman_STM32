#include "main.h"

typedef struct
{
    double items[40];
    int top;
} t_Stack;

typedef t_Stack Stack;

extern uint8_t keypadScan();
void calculator_main(void);
void checkParenthesesButtons();
double evaluatePostfix(char* postfix);
