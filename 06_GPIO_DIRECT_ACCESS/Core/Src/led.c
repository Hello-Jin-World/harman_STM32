#include "led.h"

void led_all_on(void);
void led_all_off(void);
void led_up_on(void);
void led_down_on(void);
void led_keep_on_up(void);
void led_keep_on_down(void);
void led_main(void);
void flower_on(void);
void flower_off(void);


void led_all_on(void)
{
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, 1);
	HAL_GPIO_WritePin(GPIOB, 0xff, 1);
}

void led_all_off(void)
{
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, 0);
	HAL_GPIO_WritePin(GPIOB, 0xff, 0);
}

void led_up_on(void)
{
	for (int i = 0; i < 8; i++)
	{
		led_all_off();
		HAL_GPIO_WritePin(GPIOB, 0x01 << i, 1);
		HAL_Delay(200);
	}
	led_all_off();
	HAL_Delay(200);
}

void led_down_on(void)
{
	for (int i = 0; i < 8; i++)
	{
		led_all_off();
		HAL_GPIO_WritePin(GPIOB, 0x80 >> i, 1);
		HAL_Delay(200);
	}
	led_all_off();
	HAL_Delay(200);
}

void led_keep_on_up(void)
{
	for (int i = 0; i < 8; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x01 << i, 1);
		HAL_Delay(200);
	}
	HAL_Delay(200);
}

void led_keep_on_down(void)
{
	for (int i = 0; i < 8; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x80 >> i, 0);
		HAL_Delay(200);
	}
	HAL_Delay(200);
}

void flower_on(void)
{
	for (int i = 0; i < 4; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x08 >> i | 0x10 << i, 1);
		HAL_Delay(200);
	}
}

void flower_off(void)
{
	for (int i = 0; i < 4; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x01 << i | 0x80 >> i, 0);
		HAL_Delay(200);
	}
}

void led_main(void)
{
	while(1)
	{
//		led_all_on();
//		HAL_Delay(500);
//		led_all_off();
//		HAL_Delay(500);
//		led_up_on();
//		led_down_on();
//		flower_on();
//		flower_off();
//		led_keep_on_up();
//		led_keep_on_down();

		// Direct GPIO Access
#if 1
		// per 500ms, led all toggle
		for (int i = 0; i < 8; i++)
		{
			GPIOB->ODR ^= (GPIO_PIN_0 << i);
		}
		//(unsigned short *) 0x40000000 ^= GPIO_PIN_0 | ~~~~GPIO_PIN_7    (same)
		HAL_Delay(500);
#else
		GPIOB->ODR |= GPIO_PIN_0; // led0 on
		HAL_Delay(500);
		GPIOB->ODR &= ~GPIO_PIN_0; // led0 off
		GPIOB->ODR ^= GPIO_PIN_1; // led1 toggle (easy method)
		HAL_Delay(500);
#endif
	}
}
