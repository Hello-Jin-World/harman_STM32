#include "led.h"

void led_main(void);
void led_all_on(void);
void led_all_off(void);
void led_up_on(void);
void led_down_on(void);
void led_keep_on_up(void);
void led_keep_on_down(void);
void flower_on(void);
void flower_off(void);
void ledbar0_toggle(void);

extern volatile int TIM3_10ms_counter;
extern void delay_ms(unsigned int ms);

void (*fp1[])() =
{
		led_all_on,
		led_all_off,
		led_up_on,
		led_down_on,
		led_keep_on_up,
		led_keep_on_down,
		flower_on,
		flower_off
};

extern volatile int TIM10_1ms_counter1;  // ADD_PSJ_0930

int fp1_index = 0;

void ledbar0_toggle(void)
{
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
}


void led_all_on(void)
{
	//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, 1);
	HAL_GPIO_WritePin(GPIOB, 0xff, 1);
	if (TIM3_10ms_counter >= 20)
	{
		TIM3_10ms_counter = 0;
		fp1_index = 1;
	}
}

void led_all_off(void)
{
	//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, 0);
	HAL_GPIO_WritePin(GPIOB, 0xff, 0);
	if (TIM3_10ms_counter >= 20)
	{
		TIM3_10ms_counter = 0;
		led_all_off();
		fp1_index = 2;
	}
}

void led_up_on(void)
{
	static int i = 0;

	HAL_GPIO_WritePin(GPIOB, 0x01 << i, 1);

#if 0
	if (TIM3_10ms_counter >= 20)
	{
		TIM3_10ms_counter = 0;
		i++;
		led_all_off();
		if(i >= 8)
		{
			led_all_off();
			fp1_index = 3;
		}
	}
#else
	for (int i = 0; i < 8; i++)
		{
			led_all_off();
			HAL_GPIO_WritePin(GPIOB, 0x01 << i, 1);
			delay_ms(200);
		}
#endif
}

void led_down_on(void)
{
#if 0
	static int i = 0;

	HAL_GPIO_WritePin(GPIOB, 0x80 >> i, 1);


	if (TIM3_10ms_counter >= 20)
	{
		TIM3_10ms_counter = 0;
		i++;
		led_all_off();
		if(i >= 8)
		{
			led_all_off();
			fp1_index = 4;
		}
	}
#else
	for (int i = 0; i < 8; i++)
	{
		led_all_off();
		HAL_GPIO_WritePin(GPIOB, 0x80 >> i, 1);
		delay_ms(200);
	}
#endif
}

void led_keep_on_up(void)
{
#if 0
	static int i = 0;
	HAL_GPIO_WritePin(GPIOB, 0x01 << i, 1);

	if (TIM3_10ms_counter >= 20)
	{
		TIM3_10ms_counter = 0;
		i++;
		if(i >= 8)
		{
			led_all_on();
			fp1_index = 5;
		}
		i %= 8;
	}
#else
	for (int i = 0; i < 8; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x01 << i, 1);
		delay_ms(200);
	}
#endif
}

void led_keep_on_down(void)
{
#if 0
	static int i = 0;
	HAL_GPIO_WritePin(GPIOB, 0x80 >> i, 0);

	if (TIM3_10ms_counter >= 20)
	{
		TIM3_10ms_counter = 0;
		i++;
		if(i >= 8)
		{
			led_all_off();
			fp1_index = 6;
		}
		i %= 8;
	}
#else
	for (int i = 0; i < 8; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x80 >> i, 0);
		delay_ms(200);
	}
#endif
}

void flower_on(void)
{
#if 0
	static int i = 0;
	HAL_GPIO_WritePin(GPIOB, 0x08 >> i | 0x10 << i, 1);

	if (TIM3_10ms_counter >= 20)
	{
		TIM3_10ms_counter = 0;
		i++;
		if (i >= 4)
		{
			led_all_on();
			fp1_index = 7;
			i=0;
		}
		i %= 4;
	}
#else
	for (int i = 0; i < 4; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x08 >> i | 0x10 << i, 1);
		delay_ms(200);
	}
	led_all_on();
	delay_ms(200);
#endif
}


void flower_off(void)
{
#if 0
	static int i = 0;
	HAL_GPIO_WritePin(GPIOB, 0x01 << i | 0x80 >> i, 0);

	if (TIM3_10ms_counter >= 20)
	{
		TIM3_10ms_counter = 0;
		i++;
		if (i >= 4)
		{
			led_all_off();
			fp1_index = 0;
			i=0;
		}
		i %= 4;
	}
#else
	for (int i = 0; i < 4; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x01 << i | 0x80 >> i, 0);
		delay_ms(200);
	}
	led_all_off();
	delay_ms(200);
#endif
}

// per 300ms
// 1st : led_all_on
// 2nd : led_all_off
// 3rd : Demo board led toggle

void led_main(void)
{
#if 0
	while(1)
	{
		fp1[fp1_index]();
	}
#else
	while(1)
	{
		led_all_on();
		delay_ms(200);
		led_all_off();
		delay_ms(200);
		led_up_on();
		led_down_on();
		led_keep_on_up();
		led_keep_on_down();
		flower_on();
		flower_off();
	}

#endif
}
