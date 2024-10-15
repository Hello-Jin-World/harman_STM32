#include "uart2.h"
#include "led.h"
#include <string.h>

#define MESSAGE_NUMBER 2

extern void (*fp[])();
extern void set_rtc(char *date_time);

extern int dotmatrix_command;
extern int dotmatrix_clear;

void pc_command_processing();

extern UART_HandleTypeDef huart2;
extern uint8_t rx_data;

uint8_t pdht11 = 1; // default -> print dht11
uint8_t pdatetime = 1; // default -> print datetime
uint8_t status = 1;

void printf_help(void);

char help_cmd[][50] =
{
		"pdht11 : print dht11 debug message.",
		"pdatetime : print date&time debug message."
};

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
	int func_index = 100;
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
		else if(strncmp(rx_buffer[front], "setrtc", strlen("setrtc")) == 0)
		{
			set_rtc(&rx_buffer[front][6]); // send from 6 room
		}
		else if(strncmp(rx_buffer[front], "pdatetime", strlen("pdatetime")) == 0)
		{
			pdatetime = !pdatetime;
		}
		else if(strncmp(rx_buffer[front], "pdht11", strlen("pdht11")) == 0)
		{
			pdht11 = !pdht11;
		}
		else if(strncmp(rx_buffer[front], "help", strlen("help")) == 0)
		{
			printf_help();
		}
		else if(strncmp(rx_buffer[front], "status", strlen("status")) == 0)
		{
			printf("pdht11 : ");
			status_cmd(pdht11);
			printf("pdatetime : ");
			status_cmd(pdatetime);
		}
		else if(strncmp(rx_buffer[front], "DOTDISON", strlen("DOTDISON")) == 0)
		{
			dotmatrix_command = 1;
			dotmatrix_clear = 0;
		}
		else if(strncmp(rx_buffer[front], "DOTDISSTOP", strlen("DOTDISSTOP")) == 0)
		{
			dotmatrix_command = 0;
			dotmatrix_clear = 0;
		}
		else if(strncmp(rx_buffer[front], "DOTDISOFF", strlen("DOTDISOFF")) == 0)
		{
			dotmatrix_command = 0;
			dotmatrix_clear = 1;
		}
		front++;
		front %= COMMAND_NUMBER;
		// add queue full check logic
	}

	if (func_index != 100)
	{
		(*fp[state])();
	}

}

void printf_help(void)
{
	for (int i = 0; i < MESSAGE_NUMBER; i++)
	{
		printf("%s\n", help_cmd[i]);  // help_cmd[i] -> &help_cmd[i][0]
	}
}

void status_cmd(int *status)
{
	if (status)
	{
		printf("on\n");
	}
	else
	{
		printf("off\n");
	}
}
