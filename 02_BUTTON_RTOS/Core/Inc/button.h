#include "main.h"

#define BUTTON0 0 // PC0 idle state 1(+5V), active low
#define BUTTON1 1 // PC1
#define BUTTON2 2 // PC2
#define BUTTON3 3 // PC3
#define BUTTON4 4 // Demo board button

#define BUTTON_PRESS 0 // active low
#define BUTTON_RELEASE 1 // active high

#define BUTTON_NUMBER 5 // button

void button_check(void);
int get_button(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, int button_num);
