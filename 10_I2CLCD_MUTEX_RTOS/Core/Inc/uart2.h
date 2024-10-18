#include "main.h" // for GPIO & HAl function calling
#define COMMAND_NUMBER 20
#define COMMAND_LENGTH 40

volatile uint8_t rx_buffer[COMMAND_NUMBER][COMMAND_LENGTH];
volatile int rear = 0;
volatile int front = 0;
