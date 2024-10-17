#include "led.h"
#include "button.h"

void led_all_on(void);
void led_all_off(void);
void led_up_on(void);
void led_down_on(void);
void led_keep_on_up(void);
void led_keep_on_down(void);
void led_main(void);
void flower_on(void);
void flower_off(void);

extern int get_button(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, int button_num);


void led_all_on(void)
{
	//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, 1);
	//HAL_GPIO_WritePin(GPIOB, 0xff, 1);
	*(unsigned int *) 0x40020414 |= 0x00ff;
}

void led_all_off(void)
{
	//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, 0);
	//HAL_GPIO_WritePin(GPIOB, 0xff, 0);
	*(unsigned int *) 0x40020414 &= 0xff00; // why use 0xff00? don't used 0x0000.
	// &= ~0x00ff <-- recommend this method.
}

void led_up_on(void)
{
	for (int i = 0; i < 8; i++)
	{
		led_all_off();
		*(unsigned int *) 0x40020414 |= 0x0001 << i;
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
		*(unsigned int *) 0x40020414 |= 0x0080 >> i;
		HAL_Delay(200);
	}
	led_all_off();
	HAL_Delay(200);
}

void led_keep_on_up(void)
{
	for (int i = 0; i < 8; i++)
	{
		*(unsigned int *) 0x40020414 |= 0x0001 << i;
		HAL_Delay(200);
	}
	HAL_Delay(200);
}

void led_keep_on_down(void)
{
	for (int i = 0; i < 8; i++)
	{
		*(unsigned int *) 0x40020414 &= 0xff7f >> i;
		HAL_Delay(200);
	}
	HAL_Delay(200);
}

void flower_on(void)
{
#if 1
	for (int i = 0; i < 4; i++)
	{
		*(unsigned int *) 0x40020414 |= 0x0008 >> i | 0x0010 << i;
		//HAL_GPIO_WritePin(GPIOB, 0x08 >> i | 0x10 << i, 1);
		HAL_Delay(200);
	}
#else // org
	for (int i = 0; i < 4; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x08 >> i | 0x10 << i, 1);
		HAL_Delay(200);
	}
#endif
}

void flower_off(void)
{
	for (int i = 0; i < 4; i++)
	{
		*(unsigned int *) 0x40020414 &= (0xff0e << i & 0xff0f) | (0xff70 >> i & 0xfff0);
		//HAL_GPIO_WritePin(GPIOB, 0x01 << i | 0x80 >> i, 0);
		HAL_Delay(200);
	}
	led_all_off();
}

void led_main(void)
{
	printf("GPIOB -> IDR : %0x\n", &GPIOB->IDR);
	printf("GPIOB -> ODR : %0x\n", &GPIOB->ODR);
	printf("GPIOC -> ODR : %0x\n", &GPIOC->ODR);
	printf("GPIOC -> IDR : %0x\n", &GPIOC->IDR);
	printf("GPIOB -> ODR : %0x\n", &GPIOB->ODR);
		printf("GPIOB -> IDR : %0x\n", &GPIOB->IDR);

	static int index = 0;

	while(1)
	{
		if (index)
		{
			*(unsigned int *) 0x40020414 |= 0x0001;
		}
		else
		{
			*(unsigned int *) 0x40020414 &= 0xff00;
		}

		if (get_button(GPIOC, GPIO_PIN_0, BUTTON0) == BUTTON_PRESS)
		{
			GPIOA->ODR ^= GPIO_PIN_5;
			index = !index;
		}
		/*
				led_all_on();
				HAL_Delay(500);
				led_all_off();
				HAL_Delay(500);
				led_up_on();
				led_down_on();
				flower_on();
				flower_off();
				led_keep_on_up();
				led_keep_on_down();*/
/*
		// Direct GPIO Access
		/////////////////////GPIOB 0x4002 0400 ~ 0x4002 07FF
#if 1
		// per 500ms, led all toggle
		//			GPIOB->ODR ^= GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 |
		//					 GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
		*(unsigned int *) 0x40020414 ^= GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 |
				GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
		// unsinged : using raw data. using 32bit
		//(unsigned short *) 0x40000000 ^= GPIO_PIN_0 | ~~~~GPIO_PIN_7    (same)
		HAL_Delay(500);
#else
		GPIOB->ODR |= GPIO_PIN_0; // led0 on
		HAL_Delay(500);
		GPIOB->ODR &= ~GPIO_PIN_0; // led0 off
		GPIOB->ODR ^= GPIO_PIN_1; // led1 toggle (easy method)
		HAL_Delay(500);
#endif*/
	}
}
