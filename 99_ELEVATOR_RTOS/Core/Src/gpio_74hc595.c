#include "gpio_74hc595.h"

extern void delay_us(unsigned int us);
// 0x55  0b01010101
// 0xAA  0b10101010

void gpio_74hc595_main(void);
void shift_clock(void);
void latch_clock(void);
void write_ds_75hc595(unsigned char data);

void gpio_74hc595_main(void)
{
	static int past_time = 0;
	static int led_index = 0;

	while (1)
	{
		/*while(1)
		{
			write_ds_75hc595(0b00011000);
		}*/
		uint32_t now_time = HAL_GetTick();
		// 1. past_time = 0, now_time = 500
		if (now_time - past_time >= 500)
		{
			past_time = now_time;

#if 0
		uint8_t pattern = 0;

			if (pattern < 8)
			{
				write_ds_75hc595(0x01 << pattern);
				pattern++;
			}
			else
			{
				pattern++;
				write_ds_75hc595(0x80 >> pattern % 8);
				pattern %= 15;
			}

#else
			switch(led_index)
			{
			case 0:  // 0x55  0b01010101
				write_ds_75hc595(0b01010101);
				write_ds_75hc595(0b10100101);
				write_ds_75hc595(0b01111110);
				write_ds_75hc595(0b10000001);
				write_ds_75hc595(0b10100101);
				write_ds_75hc595(0b10100101);
				write_ds_75hc595(0b10000001);
				write_ds_75hc595(0b01111110);
				break;

			case 1:  // 0xAA  0b10101010
				write_ds_75hc595(0b00000000);
				break;
			}
			led_index = !led_index;
#endif
		}
	}
}

void write_ds_75hc595(unsigned char data)
{
	unsigned char temp = data;

	//read only MSB
	for (int i = 0; i < 8; i++)
	{
		if (temp & 0b10000000) // check MSB is High
		{
			//HAL_GPIO_WritePin(DS_74HC595_GPIO_Port, DS_74HC595_Pin, 1);
			*(unsigned int *) 0x40020414 |= 0x4000;

		}
		else
		{
			//HAL_GPIO_WritePin(DS_74HC595_GPIO_Port, DS_74HC595_Pin, 0);
			*(unsigned int *) 0x40020414 &= ~0x4000;
		}

		shift_clock();
		temp = temp << 1;
	}

	//latch_clock();
	/* 76543210 i  MSB
	 * 10101010 0   1
	 * 01010100 1	0
	 * 10101000 2	1
	 * 01010000 3	0
	 * 10100000 4	1
	 * 01000000 5	0
	 * 10000000 6	1
	 * 00000000 7	0
	 */
}

void shift_clock(void)
{

	*(unsigned int *) 0x40020414 |= 0x0400;
	*(unsigned int *) 0x40020414 &= ~0x0400;
	//HAL_GPIO_WritePin(CLK_74HC595_GPIO_Port, CLK_74HC595_Pin, 1);
	//HAL_GPIO_WritePin(CLK_74HC595_GPIO_Port, CLK_74HC595_Pin, 0);
}

void latch_clock(void)
{
	*(unsigned int *) 0x40020414 |= 0x8000;
	*(unsigned int *) 0x40020414 &= ~0x8000;
	//HAL_GPIO_WritePin(LATCH_74HC595_GPIO_Port, LATCH_74HC595_Pin, 1);
	//HAL_GPIO_WritePin(LATCH_74HC595_GPIO_Port, LATCH_74HC595_Pin, 0);
}
