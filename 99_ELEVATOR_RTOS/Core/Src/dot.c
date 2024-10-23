#include "dot.h"
#include "button.h"
#include "stepmotor.h"

extern int get_button(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, int button_num);
extern void write_ds_75hc595(unsigned char data);
extern void latch_clock(void);
extern int current_floor_state;
extern volatile int TIM10_1ms_counter;  // ADD_PSJ_0930

extern int stepmotor_state;

void dotmatrix_main_test();
int dotmatrix_main(void);
int dotmatrix_main_func(void);
void arrow_display_up(void);
void init_arrow_up(void);
void init_arrow_down(void);
void arrow_display(void);
void arrow_display_stepmotor(void);


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

uint8_t two[] =
{
		0b01110000,	//2
		0b10001000,
		0b00001000,
		0b00010000,
		0b00100000,
		0b01000000,
		0b11111000,
		6
};

uint8_t three[] =
{
		0b11111000,	//3
		0b00010000,
		0b00100000,
		0b00010000,
		0b00001000,
		0b10001000,
		0b01110000,
		6
};

uint8_t four[] =
{

		0b00010000,	//4
		0b00110000,
		0b01010000,
		0b10010000,
		0b11111000,
		0b00010000,
		0b00010000,
		6
};


uint8_t col[4] = {0, 0, 0, 0};


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
	//static int i = 0;

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

//	if (TIM10_1ms_counter >= 1)
//	{
//		TIM10_1ms_counter = 0;
		col[0] = ~(1 << i);  // 00000001  --> 11111110
		col[1] = display_data_up[i];
		//HAL_SPI_Transmit(&hspi2, col, 2, 1);
		write_ds_75hc595(col[0]);
		write_ds_75hc595(col[1]);
		latch_clock();
//		i++;
//		i %= 8;
//	}
	//GPIOB->ODR &= ~GPIO_PIN_15;   // Pull latch pin down
	//GPIOB->ODR |= GPIO_PIN_15;    // Pull latch pin up

			HAL_Delay(1);
		}
}

void arrow_display_down(void)
{
	//static int i = 0;
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
//	if (TIM10_1ms_counter >= 1)
//	{
//		TIM10_1ms_counter = 0;
		col[0] = ~(1 << i);  // 00000001  --> 11111110
		col[1] = display_data_down[i];
		write_ds_75hc595(col[0]);
		write_ds_75hc595(col[1]);
		latch_clock();
//		i++;
//		i %= 8;
//	}
	//HAL_SPI_Transmit(&hspi2, col, 2, 1);
	//GPIOB->ODR &= ~GPIO_PIN_15;   // Pull latch pin down
	//GPIOB->ODR |= GPIO_PIN_15;    // Pull latch pin up
			HAL_Delay(1);
		}
}

void arrow_display_stepmotor(void)
{
	if (stepmotor_state == STEPMOTOR_FORWARD)
	{
		arrow_display_up();
	}
	else if (stepmotor_state == STEPMOTOR_BACKWARD)
	{
		arrow_display_down();
	}
}

void arrow_display(void)
{
	if (stepmotor_state == STEPMOTOR_STOP || stepmotor_state == GET_UP_BUTTON || stepmotor_state == GET_DOWN_BUTTON)
	{
		if (current_floor_state == FLOOR1)
		{
			for (int i=0; i < 8; i++)
			{
				col[0] = ~(1 << i);  // 00000001  --> 11111110
				col[1] = one[i];
				write_ds_75hc595(col[0]);
				write_ds_75hc595(col[1]);
				latch_clock();
				HAL_Delay(1);
			}
		}
		else if (current_floor_state == FLOOR2)
		{
			for (int i=0; i < 8; i++)
			{
				col[0] = ~(1 << i);  // 00000001  --> 11111110
				col[1] = two[i];
				write_ds_75hc595(col[0]);
				write_ds_75hc595(col[1]);
				latch_clock();
				HAL_Delay(1);
			}
		}
		else if (current_floor_state == FLOOR3)
		{
			for (int i=0; i < 8; i++)
			{
				col[0] = ~(1 << i);  // 00000001  --> 11111110
				col[1] = three[i];
				write_ds_75hc595(col[0]);
				write_ds_75hc595(col[1]);
				latch_clock();
				HAL_Delay(1);
			}
		}
		else if (current_floor_state == FLOOR4)
		{
			for (int i=0; i < 8; i++)
			{
				col[0] = ~(1 << i);  // 00000001  --> 11111110
				col[1] = four[i];
				write_ds_75hc595(col[0]);
				write_ds_75hc595(col[1]);
				latch_clock();
				HAL_Delay(1);
			}
		}
	}

	/*
	if(get_button(GPIOC, GPIO_PIN_0, BUTTON0) == BUTTON_PRESS)
	{
		arrow_clear = 0;
		arrow_state = !arrow_state;
	}*/
	else if (stepmotor_state == STEPMOTOR_FORWARD)
	{
		arrow_display_up();
	}
	else if (stepmotor_state == STEPMOTOR_BACKWARD)
	{
		arrow_display_down();
	}
}



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
		write_ds_75hc595(col[0]);
		write_ds_75hc595(col[1]);
		latch_clock();
		//HAL_SPI_Transmit(&hspi2, col, 2, 1);
		//GPIOB->ODR &= ~GPIO_PIN_15;   // Pull-down latch pin
		//GPIOB->ODR |= GPIO_PIN_15;   // Pull-up latch pin
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
			write_ds_75hc595(col[0]);
			write_ds_75hc595(col[1]);
			latch_clock();
			//HAL_SPI_Transmit(&hspi2, col, 2, 1);
			//GPIOB->ODR &= ~GPIO_PIN_15;   // Pull-down latch pin
			//GPIOB->ODR |= GPIO_PIN_15;   // Pull-up latch pin
			HAL_Delay(1);
		}
	}
	return 0;
}

