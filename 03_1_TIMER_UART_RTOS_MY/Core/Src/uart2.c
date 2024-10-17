#include "uart2.h"
#include "led.h"
#include <string.h>

extern void (*fp[])();

void pc_command_processing();

extern UART_HandleTypeDef huart2;
extern uint8_t rx_data;

//int toggle_full = 0;

// move from Drivers/STM32F4xx_HAL_Driver/src/stm32f4xx_hal_uart.c to here.
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	volatile static int i = 0;
	if (huart == &huart2)
	{
		uint8_t data;

		data = rx_data;
		//HAL_UART_Transmit(&huart2, &data, 1, 0xFFFF);
		if (data == '\n')
		{
			rx_buffer[rear][i++] = '\0'; // end message (insert NULL)
			i = 0;
			rear++;
			rear %= COMMAND_NUMBER;
			// add queue full check logic

//			if ((rear + 1) % 10 == front)
//			{
//				toggle_full = 1;
//			}
		}
		else
		{
			rx_buffer[rear][i++] = data;
		}
		HAL_UART_Receive_IT(&huart2, &rx_data, 1); // Must be contained.
	}
}

void pc_command_processing(void)
{
	static int state = 0;
	if (front != rear)
	{
		printf("%s\n", rx_buffer[front]); // &rx_buffer[front][0]
		if (strncmp(rx_buffer[front], "led_all_on", strlen("led_all_on")) == 0)
		{
			state = 1;
		}
		else if(strncmp(rx_buffer[front], "led_all_off", strlen("led_all_off")) == 0)
		{
			state = 0;
		}
		else if(strncmp(rx_buffer[front], "led_up_on", strlen("led_up_on")) == 0)
		{
			state = 2;
		}
		else if(strncmp(rx_buffer[front], "led_down_on", strlen("led_down_on")) == 0)
		{
			state = 3;
		}
		else if(strncmp(rx_buffer[front], "flower_on", strlen("flower_on")) == 0)
		{
			state = 4;
		}
		else if(strncmp(rx_buffer[front], "flower_off", strlen("flower_off")) == 0)
		{
			state = 5;
		}
		else if(strncmp(rx_buffer[front], "led_keep_on_up", strlen("led_keep_on_up")) == 0)
		{
			state = 6;
		}
		else if(strncmp(rx_buffer[front], "led_keep_on_down", strlen("led_keep_on_down")) == 0)
		{
			state = 7;
		}
		front++;
		front %= COMMAND_NUMBER;
		// add queue full check logic
	}

	(*fp[state])();

}

