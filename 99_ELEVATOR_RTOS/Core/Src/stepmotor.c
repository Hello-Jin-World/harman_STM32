#include "stepmotor.h"
#include "button.h"


extern void delay_us(unsigned int us);
extern int get_button(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, int button_num);
extern void init_arrow_up(void);
extern void init_arrow_down(void);
extern volatile int TIM10_1ms_counter1;

void set_rmp(int rmp);
void stepmotor_main(void);
void stepmotor_drive(int step);
void stepmotor_forward(void);
void stepmotor_backward(void);
void stepmotor_stop(void);
void select_floor(void);

int j = 0;
int stepmotor_state = 0;
int floor_select_state = 0;
int current_floor_state = 0;

/*
 * RPM (Revolutions Per Minutes)
 * 1minute => 60seconds => 60 x 1,000,000us
 * 1sec => 1000ms
 * 1cycle needs 4096 steps
 * 4096 steps / 8 => 512 sequence
 * 1 sequence (8 step) : 0.70312'
 * 0.70312 x 512 sequence = 360'
 *
 * ----RPM modulation function----
 * 60,000,000us / 4096 / rpm(1~13)
 * 13 cycles : 60,000,000us / 4096 / 13 -> between step and step, need 1126us delay
 * 1126us x 4096(1 cycle) = 4,615,384us = 4,615ms = 4.6sec
 *
 * 60sec / 4.6sec (1cycle) = 13 cycle
 *
 */

void (*stepmotor_funcp[])() =
{
		stepmotor_stop,
		stepmotor_forward,
		stepmotor_backward
};

void set_rmp(int rmp)
{
	delay_us(60000000/4096/rmp);
	// Maximum speed (13) : delay_us(1126);
}

void select_floor(void)
{
	(*stepmotor_funcp[stepmotor_state])();

	if (get_button(GPIOC, GPIO_PIN_0, BUTTON0) == BUTTON_PRESS)
	{

		if (floor_select_state > current_floor_state)
		{
			stepmotor_state = STEPMOTOR_FORWARD;
		}
		else if (floor_select_state < current_floor_state)
		{
			stepmotor_state = STEPMOTOR_BACKWARD;
		}
		else
		{
			stepmotor_state = STEPMOTOR_STOP;
		}

	}
	if (get_button(GPIOC, GPIO_PIN_1, BUTTON1) == BUTTON_PRESS)
	{
		HAL_GPIO_WritePin(GPIOB, 0xff, 0);
		floor_select_state++;
		floor_select_state %= 4;
		HAL_GPIO_WritePin(GPIOB, 0b0000001 << floor_select_state, 1);
	}

}


#if 1
void stepmotor_main(void)
{
	(*stepmotor_funcp[stepmotor_state])();

	if (get_button(GPIOC, GPIO_PIN_0, BUTTON0) == BUTTON_PRESS)
	{
		if (stepmotor_state == STEPMOTOR_BACKWARD || stepmotor_state == STEPMOTOR_FORWARD)
		{
			stepmotor_state = STEPMOTOR_STOP;
		}
	}

	if (get_button(GPIOC, GPIO_PIN_1, BUTTON1) == BUTTON_PRESS)
	{
		if (stepmotor_state == STEPMOTOR_FORWARD)
		{
			stepmotor_state = STEPMOTOR_BACKWARD;
		}
		else
		{
			stepmotor_state = STEPMOTOR_FORWARD;
		}
	}
}
#else
void stepmotor_main(void)
{
	for (int i = 0; i < 512; i++) // clock rotation 1 cycle
	{
		for (int j = 0; j < 8; j++)
		{
			stepmotor_drive(j);
			set_rmp(13); // wait for 1126us
		}
	}

	for (int i = 0; i < 512; i++) // clock rotation 1 cycle
	{
		for (int j = 7; j >= 0; j--)
		{
			stepmotor_drive(j);
			set_rmp(13); // wait for 1126us
		}
	}
}
#endif

void stepmotor_stop(void)
{
	stepmotor_drive(j);
}

void stepmotor_forward(void)
{
//	if (TIM10_1ms_counter1 >= 16000/4096/13)
//	{
		stepmotor_drive(j);
		j++;
		j %= 8;
//		TIM10_1ms_counter1 = 0;
//	}
	if (floor_select_state == current_floor_state)
	{
		HAL_GPIO_WritePin(GPIOB, 0xff, 0);

		stepmotor_state = STEPMOTOR_STOP;
	}
	delay_us(126); // consider osDelay 1ms
//	set_rmp(13); // wait for 1126us

}

void stepmotor_backward(void)
{
	if (TIM10_1ms_counter1 >= 16000/4096/13)
	{
		stepmotor_drive(j);
		j--;
		if (j < 0)
		{
			j = 7;
		}
		TIM10_1ms_counter1 = 0;
	}
	if (floor_select_state == current_floor_state)
	{
		HAL_GPIO_WritePin(GPIOB, 0xff, 0);

		stepmotor_state = STEPMOTOR_STOP;
	}
	//set_rmp(13); // wait for 1126us

}

#if 1
void stepmotor_drive(int step)
{
	if (step == 0)
	{
		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 1);
		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0);
	}
	else if (step == 1)
	{
		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 1);
		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 1);
		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0);
	}
	else if (step == 2)
	{
		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 1);
		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0);
	}
	else if (step == 3)
	{
		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 1);
		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 1);
		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0);
	}
	else if (step == 4)
	{
		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 1);
		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0);
	}
	else if (step == 5)
	{
		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 1);
		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 1);
	}
	else if (step == 6)
	{
		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 1);
	}
	else if (step == 7)
	{
		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 1);
		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 1);
	}
}
#else
void stepmotor_drive(int step)
{
	switch(step)
	{
	case 0:
		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 1);
		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0);
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 1);
		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 1);
		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 1);
		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0);
		break;
	case 3:
		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 1);
		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 1);
		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0);
		break;
	case 4:
		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 1);
		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0);
		break;
	case 5:
		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 1);
		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 1);
		break;
	case 6:
		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 1);
		break;
	case 7:
		HAL_GPIO_WritePin(GPIOC, IN1_Pin, 1);
		HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0);
		HAL_GPIO_WritePin(GPIOC, IN4_Pin, 1);
		break;
	}
}
#endif
