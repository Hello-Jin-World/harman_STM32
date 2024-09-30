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

void (*fp1[])() =
{
	led_up_on,
	led_down_on,
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
}

void led_all_off(void)
{
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, 0);
	HAL_GPIO_WritePin(GPIOB, 0xff, 0);
}

void led_up_on(void)
{
#if 1
	static int i = 0;
	HAL_GPIO_WritePin(GPIOB, 0x01 << i, 1);

	if (TIM10_1ms_counter1 >= 200)
	{
		TIM10_1ms_counter1 = 0;
		i++;
		led_all_off();
		if (i >= 8)
		{
			fp1_index = 1;
		}
		i %= 8;
	}
}

#else // Original Code (NOT USED TIMER)
	for (int i = 0; i < 8; i++)
	{
		led_all_off();
		HAL_GPIO_WritePin(GPIOB, 0x01 << i, 1);
		HAL_Delay(50);
	}
}
#endif

void led_down_on(void)
{
#if 1
	static int i = 0;

	HAL_GPIO_WritePin(GPIOB, 0x80 >> i, 1);

	if (TIM10_1ms_counter1 >= 200)
	{
		TIM10_1ms_counter1 = 0;
		led_all_off();
		i++;
		if (i >= 8)
		{
			fp1_index = 2;
		}
		i %= 8;
	}

}
#else
	for (int i = 0; i < 8; i++)
	{
		led_all_off();
		HAL_GPIO_WritePin(GPIOB, 0x80 >> i, 1);
		HAL_Delay(50);
	}
}
#endif

void led_keep_on_up(void)
{
	for (int i = 0; i < 8; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x01 << i, 1);
		HAL_Delay(50);
	}
	led_all_off();
	HAL_Delay(50);
}

void led_keep_on_down(void)
{
	for (int i = 0; i < 8; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x80 >> i, 0);
		HAL_Delay(50);
	}
	led_all_on();
	HAL_Delay(50);
}

void flower_on(void)
{
#if 1
	static int i = 0;
	HAL_GPIO_WritePin(GPIOB, 0x08 >> i | 0x10 << i, 1);

	if (TIM10_1ms_counter1 >= 200)
	{
		TIM10_1ms_counter1 = 0;
		i++;
		if (i >= 4)
		{
			led_all_on();
			fp1_index = 3;
		}
		i %= 4;
	}
}
#else
	for (int i = 0; i < 4; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x08 >> i | 0x10 << i, 1);
		HAL_Delay(50);
	}
	led_all_off();
	HAL_Delay(50);
}
#endif

void flower_off(void)
{
#if 1
	static int i = 0;
	HAL_GPIO_WritePin(GPIOB, 0x01 << i | 0x80 >> i, 0);

	if (TIM10_1ms_counter1 >= 200)
	{
		TIM10_1ms_counter1 = 0;
		i++;
		if (i >= 4)
		{
			led_all_off();
			fp1_index = 0;
		}
		i %= 4;
	}
}
#else
	for (int i = 0; i < 4; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x01 << i | 0x80 >> i, 0);
		HAL_Delay(50);
	}
	led_all_on();
	HAL_Delay(50);
}
#endif

// per 300ms
// 1st : led_all_on
// 2nd : led_all_off
// 3rd : Demo board led toggle

void led_main(void)
{
	fp1[fp1_index]();

	/*
	if (TIM10_1ms_counter2 >= 1600 && TIM10_1ms_counter2 < 3200)
	{
		i = 1;
	}
	else if (TIM10_1ms_counter2 >= 3200)
	{
		TIM10_1ms_counter2 = 0;
		i = 0;
	}
	*/
	/*
	static int num = 0;
	num %= 8;
	HAL_GPIO_WritePin(GPIOB, 0x01 << num, 1);
	if (TIM10_1ms_counter1 >= 50)
	{
		num++;
		TIM10_1ms_counter1 = 0;
		led_all_off();
	}
	*/
}
