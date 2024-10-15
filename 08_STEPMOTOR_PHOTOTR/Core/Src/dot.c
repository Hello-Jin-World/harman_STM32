#include "dot.h"
#include "button.h"

extern SPI_HandleTypeDef hspi2;
extern volatile int TIM10_1ms_counter1;
extern int get_button(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, int button_num);

void dotmatrix_main_test();
int dotmatrix_main(void);
int dotmatrix_main_func(void);
void arrow_display_up(void);
int arrow_state = 1;
void init_arrow_up(void);
void init_arrow_down(void);
void arrow_display(void);

uint8_t allon[] = {			// Definition for all LEDs on
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111
};


uint8_t smile[] = {			// Definition for smiley face
		0b00111100,
		0b01000010,
		0b10010101,
		0b10100001,
		0b10100001,
		0b10010101,
		0b01000010,
		0b00111100 };

uint8_t hart[] = {		// Definition for heart shape
		0b00000000,
		0b01100110,
		0b10011001,
		0b10000001,
		0b10000001,
		0b01000010,
		0b00100100,
		0b00011000
};

uint8_t neko[] = {		// Definition for cat face
		0b01000010,
		0b10100101,
		0b01111110,
		0b10000001,
		0b10100101,
		0b10100101,
		0b10000001,
		0b01111110
};

uint8_t one[] =
{
		0b00011000,
		0b00111000,
		0b00011000,
		0b00011000,
		0b00011000,
		0b00011000,
		0b01111110,
		0b01111110
};


uint8_t col[4] = {0, 0, 0, 0};

void dotmatrix_main_test()
{
	// dotmatrix_main();

	while (1)
	{
		for (int i=0; i < 8; i++)
		{
			col[0] = ~(1 << i);  // 00000001  --> 11111110
			col[1] = neko[i];
			HAL_SPI_Transmit(&hspi2, col, 2, 1);
			GPIOB->ODR &= ~GPIO_PIN_15;   // Pull-down latch pin
			GPIOB->ODR |= GPIO_PIN_15;    // Pull-up latch pin
			HAL_Delay(1);
		}
	}
}

uint8_t number_data[20][10] =
{
		{
				0b01110000,	//0
				0b10001000,
				0b10011000,
				0b10101000,
				0b11001000,
				0b10001000,
				0b01110000,
				0b00000000
		},
		{
				0b01000000,	//1
				0b11000000,
				0b01000000,
				0b01000000,
				0b01000000,
				0b01000000,
				0b11100000,
				6   // Dot 0b00000110
		},
		{
				0b01110000,	//2
				0b10001000,
				0b00001000,
				0b00010000,
				0b00100000,
				0b01000000,
				0b11111000,
				6
		},
		{
				0b11111000,	//3
				0b00010000,
				0b00100000,
				0b00010000,
				0b00001000,
				0b10001000,
				0b01110000,
				6
		},
		{
				0b00010000,	//4
				0b00110000,
				0b01010000,
				0b10010000,
				0b11111000,
				0b00010000,
				0b00010000,
				6
		},
		{
				0b11111000,	//5
				0b10000000,
				0b11110000,
				0b00001000,
				0b00001000,
				0b10001000,
				0b01110000,
				6
		},
		{
				0b00110000,	//6
				0b01000000,
				0b10000000,
				0b11110000,
				0b10001000,
				0b10001000,
				0b01110000,
				6
		},
		{
				0b11111000,	//7
				0b10001000,
				0b00001000,
				0b00010000,
				0b00100000,
				0b00100000,
				0b00100000,
				6
		},
		{
				0b01110000,	//8
				0b10001000,
				0b10001000,
				0b01110000,
				0b10001000,
				0b10001000,
				0b01110000,
				6
		},
		{
				0b01110000,	//9
				0b10001000,
				0b10001000,
				0b01111000,
				0b00001000,
				0b00010000,
				0b01100000,
				6
		},
		{
				0b00000000,    // Heart shape
				0b01100110,
				0b11111111,
				0b11111111,
				0b11111111,
				0b01111110,
				0b00111100,
				0b00011000
		}
};

uint8_t my_name[20][10] =
{
		{
				0B00000000,
				0B01010100,
				0B01110110,
				0B01010100,
				0B01110100,
				0B00000000,
				0B00111100,
				0B00000100
		},

		{
				0B00000000,
				0B00100010,
				0B01010010,
				0B01010110,
				0B00000010,
				0B00001000,
				0B00010100,
				0B00001000
		},

		{
				0B00000000,
				0B01110100,
				0B00100100,
				0B01010100,
				0B00000000,
				0B00100000,
				0B00111100,
				0B00000000
		}
};

uint8_t arrow_up[] =
{
		0B00010000,
		0B00101000,
		0B01000100,
		0B11101110,
		0B00101000,
		0B00101000,
		0B00111000,
		0B00101000
};

uint8_t arrow_down[] =
{
		0B00101000,
		0B00111000,
		0B00101000,
		0B00101000,
		0B11101110,
		0B01000100,
		0B00101000,
		0B00010000
};


unsigned char display_data[8];  // Final 8x8 output data
unsigned char display_data_up[8];
unsigned char display_data_down[8];
unsigned char scroll_buffer[50][8] = {0};  // Buffer for scrolling data
int number_of_character = 11;  // Number of characters to display

// Initialization tasks
// 1. Copy number_data[0] contents to display_data
// 2. Copy number_data to scroll_buffer
// 3. Turn off LEDs on dot matrix
void init_dotmatrix(void)
{
	for (int i=0; i < 8; i++)
	{
		display_data[i] = number_data[i];
	}
	for (int i=1; i < number_of_character-6; i++)
	{
		for (int j=0; j < 8; j++) // scroll_buffer[0] = blank
		{
			scroll_buffer[i][j] = number_data[i-1][j];
		}
	}
}

#if 1
void init_arrow_up(void)
{
	for (int i=0; i < 8; i++)
	{
		display_data_up[i] = arrow_up[i];
	}
	for (int i=0; i < 8; i++)
	{
		scroll_buffer[0][i] = arrow_up[i]; // Store the arrow_up data scroll_buffer
	}
}
void init_arrow_down(void)
{
	for (int i=0; i < 8; i++)
	{
		display_data_down[i] = arrow_down[i];
	}
	for (int i=0; i < 8; i++)
	{
		scroll_buffer[0][i] = arrow_down[i]; // Store the arrow_down data scroll_buffer
	}
}

void arrow_display_up(void)
{
	static uint32_t past_time=0;  // get pre_tick value
	uint32_t now = HAL_GetTick();  // Current time

	// Shift arrow upwards every 500ms
	if (now - past_time >= 200)
	{
		past_time = now;

		// Shift display_data_up one position up
		uint8_t temp = display_data_up[0]; // Store the first row
		for (int i = 0; i < 7; i++) // Shift each row up
		{
			display_data_up[i] = display_data_up[i + 1];
		}
		display_data_up[7] = temp; // Move the first row to the last row
	}

	// Output display_data
	for (int i = 0; i < 8; i++)
	{
		col[0] = ~(1 << i);  // 00000001  --> 11111110
		col[1] = display_data_up[i];
		HAL_SPI_Transmit(&hspi2, col, 2, 1);
		GPIOB->ODR &= ~GPIO_PIN_15;   // Pull latch pin down
		GPIOB->ODR |= GPIO_PIN_15;    // Pull latch pin up
		HAL_Delay(1);
	}
}

void arrow_display_down(void)
{
	static uint32_t past_time = 0;  // Store previous tick value
	uint32_t now = HAL_GetTick();  // Current time

	// Shift arrow downwards every 500ms
	if (now - past_time >= 200)
	{
		past_time = now;

		// Shift display_data_down one position down
		uint8_t temp = display_data_down[7]; // Store the last row
		for (int i = 7; i > 0; i--) // Shift each row down
		{
			display_data_down[i] = display_data_down[i - 1];
		}
		display_data_down[0] = temp; // Move the last row to the first row
	}

	// Output display_data
	for (int i = 0; i < 8; i++)
	{
		col[0] = ~(1 << i);  // 00000001  --> 11111110
		col[1] = display_data_down[i];
		HAL_SPI_Transmit(&hspi2, col, 2, 1);
		GPIOB->ODR &= ~GPIO_PIN_15;   // Pull latch pin down
		GPIOB->ODR |= GPIO_PIN_15;    // Pull latch pin up
		HAL_Delay(1);
	}
}

void arrow_display(void)
{
	static int arrow_clear = 1;
	if (arrow_clear)
	{
		for (int i = 0; i < 8; i++)
		{
			display_data[i] = 0;
		}
	}
	if(get_button(GPIOC, GPIO_PIN_0, BUTTON0) == BUTTON_PRESS)
	{
		arrow_clear = 0;
		arrow_state = !arrow_state;
	}
	if (arrow_state == 0 && arrow_clear == 0)
	{
		arrow_display_up();
	}
	if (arrow_state == 1 && arrow_clear == 0)
	{
		arrow_display_down();
	}
}
#else
	int arrow_up = 0;
	int arrow_down = 0;

	void arrow_dispaly(void)
	{
		static int j = 1;
		static int k = 0;
		static int countj=0;  // Column count
		static int countk=0;  // Column count
		static int index=0;  // 2D index value of scroll_buffer
		static uint32_t past_time=0;  // Store previous tick value

		uint32_t now = HAL_GetTick();  // 1ms
		// 1. At start, past_time=0; now: 500 --> past_time=500
		if (arrow_up)
		{
			if (now - past_time >= 100) // 500ms scroll
			{

				past_time = now;
				for (int i=0; i < 8; i++)
				{
					display_data[i] = (scroll_buffer[index][j++]) |
							(scroll_buffer[index][j]);

				}
				j++;
				j%=9;
			}
		}
		else if (arrow_down)
		{
			if (now - past_time >= 100) // 500ms scroll
			{

				past_time = now;
				for (int i=0; i < 8; i++)
				{
					display_data[i] = (scroll_buffer[index][j]) |
							(scroll_buffer[index][j--]);

				}
				j--;
				if (j <= 0)
				{
					j = 100;
				}
			}
		}

		if (arrow_up == 0 && arrow_down == 0)
		{
			for (int i=0; i < 8; i++)
			{
				display_data[i] = 0;
			}
		}
		for (int i=0; i < 8; i++)
		{
			// Common anode mode
			// Column should be 0 and Row should be 1 for the corresponding LED to turn on.
			col[0] = ~(1 << i);  // 00000001  --> 11111110
			col[1] = display_data[i];
			HAL_SPI_Transmit(&hspi2, col, 2, 1);
			GPIOB->ODR &= ~GPIO_PIN_15;   // Pull-down latch pin
			GPIOB->ODR |= GPIO_PIN_15;   // Pull-up latch pin
			HAL_Delay(1);
		}
		if (get_button(GPIOC, GPIO_PIN_0, BUTTON0) == BUTTON_PRESS && arrow_up == 0)
		{
			for (int i=0; i < 8; i++)
			{
				display_data[i] = 0;
			}
			arrow_down = 0;
			arrow_up = 1;
			for (int i=0; i < 8; i++)
			{
				display_data[i] = arrow[i];
			}
			for (int i=1; i < number_of_character+1; i++)
			{
				for (int j=0; j < 8; j++) // scroll_buffer[0] = blank
				{
					scroll_buffer[i][j] = arrow[0][j];
				}
			}
		}

		else if (get_button(GPIOC, GPIO_PIN_0, BUTTON0) == BUTTON_PRESS && arrow_down == 0)
		{
			for (int i=0; i < 8; i++)
			{
				display_data[i] = 0;
			}
			arrow_down = 1;
			arrow_up = 0;
			for (int i=0; i < 8; i++)
			{
				display_data[i] = arrow[i];
			}
			for (int i=1; i < number_of_character+1; i++)
			{
				for (int j=0; j < 8; j++) // scroll_buffer[0] = blank
				{
					scroll_buffer[i][j] = arrow[1][j];
				}
			}
		}


	}
#endif

	int dotmatrix_command = 0;
	int dotmatrix_clear = 1;
	int dotmatrix_main_func(void)
	{
		static int count=0;  // Column count
		static int index=0;  // 2D index value of scroll_buffer
		static uint32_t past_time=0;  // Store previous tick value

		uint32_t now = HAL_GetTick();  // 1ms
		// 1. At start, past_time=0; now: 500 --> past_time=500
		if (dotmatrix_command)
		{
			if (now - past_time >= 500) // 500ms scroll
			{
				past_time = now;
				for (int i=0; i < 8; i++)
				{
					display_data[i] = (scroll_buffer[index][i] >> count) |
							(scroll_buffer[index+1][i] >> 8 - count);
				}
				if (++count == 8) // If all 8 columns are processed, move to next scroll_buffer
				{
					count =0;
					index++;  // Move to next scroll_buffer
					if (index == number_of_character+1) index=0;
					// After processing all 11 characters, move to scroll_buffer 0
				}
			}
		}
		if (dotmatrix_clear)
		{
			for (int i=0; i < 8; i++)
			{
				display_data[i] = 0;
			}
			count = 0;
			index = 0;

		}
		for (int i=0; i < 8; i++)
		{
			// Common anode mode
			// Column should be 0 and Row should be 1 for the corresponding LED to turn on.
			col[0] = ~(1 << i);  // 00000001  --> 11111110
			col[1] = display_data[i];
			HAL_SPI_Transmit(&hspi2, col, 2, 1);
			GPIOB->ODR &= ~GPIO_PIN_15;   // Pull-down latch pin
			GPIOB->ODR |= GPIO_PIN_15;   // Pull-up latch pin
			HAL_Delay(1);
		}
	}


	// Scroll text display program
	int dotmatrix_main(void)
	{
		static int count=0;  // Column count
		static int index=0;  // 2D index value of scroll_buffer
		static uint32_t past_time=0;  // Store previous tick value

		init_dotmatrix();

		while(1)
		{
			uint32_t now = HAL_GetTick();  // 1ms
			// 1. At start, past_time=0; now: 500 --> past_time=500
			if (now - past_time >= 500) // 500ms scroll
			{
				past_time = now;
				for (int i=0; i < 8; i++)
				{

					display_data[i] = (scroll_buffer[index][i] >> count) |
							(scroll_buffer[index+1][i] << 8 - count);
				}
				if (++count == 8) // If all 8 columns are processed, move to next scroll_buffer
				{
					count =0;
					index++;  // Move to next scroll_buffer
					if (index == number_of_character+1) index=0;
					// After processing all 11 characters, move to scroll_buffer 0
				}
			}
			for (int i=0; i < 8; i++)
			{
				// Common anode mode
				// Column should be 0 and Row should be 1 for the corresponding LED to turn on.
				col[0] = ~(1 << i);  // 00000001  --> 11111110
				col[1] = display_data[i];
				HAL_SPI_Transmit(&hspi2, col, 2, 1);
				GPIOB->ODR &= ~GPIO_PIN_15;   // Pull-down latch pin
				GPIOB->ODR |= GPIO_PIN_15;   // Pull-up latch pin
				HAL_Delay(1);
			}
		}
		return 0;
	}

