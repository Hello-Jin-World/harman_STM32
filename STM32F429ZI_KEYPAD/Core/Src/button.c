#include "button.h"

void button_check(void);
int get_button(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, int button_num);

unsigned char button_status[BUTTON_NUMBER] = {
		BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE
};


//When press the button, turn on led 1 step. If led all on, turn off 1 step. loop
void button_check(void)
{
#if 0
	static int led_on = 0;
	static int mode = 0;


	if (get_button(GPIOC, GPIO_PIN_0, BUTTON0) == BUTTON_PRESS)
	{
		mode++;
		mode %= 4;
	}
	else if (get_button(GPIOC, GPIO_PIN_1, BUTTON1) == BUTTON_PRESS)
	{
		if (mode == 4)
		{
			mode = 5;
		}
		else
		{
			mode = 4;
		}

	}
	else if (get_button(GPIOC, GPIO_PIN_2, BUTTON2) == BUTTON_PRESS)
	{

		if (mode != 6)
		{
			mode = 6;
		}
		else if (mode == 6)
		{
			mode = 7;
		}
	}
	else if (get_button(GPIOC, GPIO_PIN_3, BUTTON3) == BUTTON_PRESS)
	{
		if (mode != 0)
		{
			mode = 0;
		}
		else if (mode == 0)
		{
			mode = 1;
		}
	}
	// check one click

	if (get_button(GPIOC, GPIO_PIN_13, BUTTON4) == BUTTON_PRESS)
	{
		led_on %= 16;
		HAL_GPIO_WritePin(GPIOB, 0x01 << led_on, 1);
		led_on++;

		if (led_on > 8)
		{
			HAL_GPIO_WritePin(GPIOB, 0x80 >> (led_on+6)%15, 0);
		}
	}
#endif
}
// if one click status, return 1
int get_button(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, int button_num) // ex) GPIOC, GPIO_PIN_13, 0
{
	int state;

	state = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin); // active : 0, inactive : 1
	if (state == BUTTON_PRESS && button_status[button_num] == BUTTON_RELEASE)
	{
		HAL_Delay(60); // For noise remove delay
		button_status[button_num] = BUTTON_PRESS; // Not first button press
		return BUTTON_RELEASE;
	}
	else if (button_status[button_num] == BUTTON_PRESS && state == BUTTON_RELEASE) // Previous, button is pressed but Now, inactive status
	{
		button_status[button_num] = BUTTON_RELEASE;
		HAL_Delay(60);
		return BUTTON_PRESS;
	}
	return BUTTON_RELEASE;
}
