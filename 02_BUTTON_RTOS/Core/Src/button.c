#include "button.h"

void button_check(void);
int get_button(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, int button_num);

unsigned char button_status[BUTTON_NUMBER] = {
		BUTTON_RELEASE
};



void button_check(void)
{
	static int ledon = 0;
	// check one click
	if (get_button(GPIOC, GPIO_PIN_13, BUTTON0) == BUTTON_PRESS)
	{
		ledon %= 16;
		HAL_GPIO_WritePin(GPIOB, 0x01 << ledon, 1);
		ledon++;

		if (ledon > 8)
		{
			HAL_GPIO_WritePin(GPIOB, 0x80 >> (ledon+6)%15, 0);
		}

	}
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
