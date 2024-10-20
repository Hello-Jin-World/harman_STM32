#include "stepmotor.h"
#include "button.h"
#include "servomotor.h"
#include <stdlib.h>  // malloc, free 사용

extern void delay_us(unsigned int us);
extern int get_button(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, int button_num);
extern void init_arrow_up(void);
extern void init_arrow_down(void);
extern void open_the_door(void);
extern void close_the_door(void);
extern volatile int TIM10_1ms_counter1;
extern volatile int TIM10_elevator_counter;
extern TIM_HandleTypeDef htim3;

void set_rmp(int rmp);
void stepmotor_drive(int step);
void stepmotor_forward(void);
void stepmotor_backward(void);
void stepmotor_stop(void);
void get_up_button(void);
void get_down_button(void);
void select_floor(void);
void init_floor_select(int size);
void reset_floor_select(void);

int j = 0; // for stepmotor speed
int stepmotor_state = STEPMOTOR_STOP;
int floor_select_state = FLOOR1;
int current_floor_state = 0;
int floor_select_list[4] = {NOT_SELECTED, NOT_SELECTED, NOT_SELECTED, NOT_SELECTED}; // Not Selected ==>  -1
// If all elements are -1, it means "No floor is selected"
int interrupt_floor = 0; // It returns 1 each time it reaches a floor
int now_up_down_state = ELEVATOR_CLOSED; // If it is rising, UP_STATE // If it is falling, DOWN_STATE
int door_state = CLOSE_THE_DOOR;

void (*stepmotor_funcp[])() =
{
		stepmotor_stop,
		stepmotor_forward,
		stepmotor_backward,
		get_up_button,
		get_down_button
};

void select_floor(void)
{
	(*stepmotor_funcp[stepmotor_state])();

	if (get_button(GPIOC, GPIO_PIN_0, BUTTON0) == BUTTON_PRESS)
	{

		if (now_up_down_state == NONE_STATE)
		{
			floor_select_list[floor_select_state] = floor_select_state;

			if (floor_select_state > current_floor_state)
			{
				stepmotor_state = STEPMOTOR_FORWARD;
			}
			else if (floor_select_state < current_floor_state)
			{
				stepmotor_state = STEPMOTOR_BACKWARD;
			}
		}

//		else if (now_up_down_state == PRESSED_UP_BUTTON)
//		{
//			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, door_state);
//			if (floor_select_state > current_floor_state)
//			{
//				floor_select_list[floor_select_state] = floor_select_state;
//				stepmotor_state = STEPMOTOR_FORWARD;
//			}
//			else if (floor_select_state < current_floor_state)
//			{
//				floor_select_list[floor_select_state] = floor_select_state;
//
//			}
//		}
//
//		else if (now_up_down_state == PRESSED_DOWN_BUTTON)
//		{
//			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, door_state);
//
//			if (floor_select_state < current_floor_state)
//			{
//				stepmotor_state = STEPMOTOR_BACKWARD;
//			}
//		}

		else if ((now_up_down_state == UP_STATE) && (floor_select_state > current_floor_state))
		{
			// When rising, it only saves the input floor value if the entered floor value is higher than the current floor.
			floor_select_list[floor_select_state] = floor_select_state;
		}
		else if ((now_up_down_state == DOWN_STATE) && (floor_select_state < current_floor_state))
		{
			// When falling, it only saves the input floor value if the entered floor value is lower than the current floor.
			floor_select_list[floor_select_state] = floor_select_state;
		}

		for (int i = 0; i < 4; i++) {
			printf("%d ", floor_select_list[i]);
		}
		printf("\n");

		//		if (floor_select_state > current_floor_state)
		//		{
		//			stepmotor_state = STEPMOTOR_FORWARD;
		//		}
		//		else if (floor_select_state < current_floor_state)
		//		{
		//			stepmotor_state = STEPMOTOR_BACKWARD;
		//		}

	}

	if (get_button(GPIOC, GPIO_PIN_1, BUTTON1) == BUTTON_PRESS)
	{
		HAL_GPIO_WritePin(GPIOB, 0xff, 0);
		floor_select_state++;
		floor_select_state %= 4;
		HAL_GPIO_WritePin(GPIOB, 0b0000001 << floor_select_state, 1);
	}
}

void reset_floor_select(void)
{
	now_up_down_state = ELEVATOR_CLOSED;
	memset(floor_select_list, NOT_SELECTED, sizeof(floor_select_list));

	printf("list reset!!!!\n");
}

void stepmotor_stop(void)
{

	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, door_state);

	if (floor_select_list[FLOOR1] == NOT_SELECTED && floor_select_list[FLOOR2] == NOT_SELECTED && floor_select_list[FLOOR3] == NOT_SELECTED && floor_select_list[FLOOR4] == NOT_SELECTED)
	{
		// If all the selected floors are reached, it resets the array to prepare for receiving new floors.
		stepmotor_drive(j);
		reset_floor_select();

		if (get_button(GPIOC, GPIO_PIN_3, BUTTON3) == BUTTON_PRESS)
		{
			TIM10_elevator_counter = 0;
			now_up_down_state = NONE_STATE;
			door_state = OPEN_THE_DOOR;
			stepmotor_state = GET_UP_BUTTON;
		}

		else if (get_button(GPIOC, GPIO_PIN_2, BUTTON2) == BUTTON_PRESS)
		{
			TIM10_elevator_counter = 0;
			now_up_down_state = NONE_STATE;
			door_state = OPEN_THE_DOOR;
			stepmotor_state = GET_DOWN_BUTTON;
		}
	}

	else
	{
		if (TIM10_elevator_counter <= 5000) // Stop for 5 seconds.
		{
			if (TIM10_elevator_counter > 1500 && TIM10_elevator_counter < 3500)
			{
				// open the door
				door_state = OPEN_THE_DOOR;
			}
			else
			{
				// close the door
				door_state = CLOSE_THE_DOOR;
			}
			stepmotor_drive(j);
		}

		if (TIM10_elevator_counter > 5000)
		{
			floor_select_list[current_floor_state] = NOT_SELECTED;

			TIM10_elevator_counter = 0;

			if (now_up_down_state == UP_STATE) // If it is rising and floor_select_list is none of the elements is -1, it will continue to rise.
			{
				if (floor_select_list[FLOOR1] != NOT_SELECTED || floor_select_list[FLOOR2] != NOT_SELECTED || floor_select_list[FLOOR3] != NOT_SELECTED || floor_select_list[FLOOR4] != NOT_SELECTED)
				{
					stepmotor_state = STEPMOTOR_FORWARD;
				}
				//				if (floor_select_list[(current_floor_state + 1)%4] != -1 || floor_select_list[(current_floor_state + 2)%4] != -1
				//						|| floor_select_list[(current_floor_state + 3)%4] != -1)
				//				{
				//					stepmotor_state = STEPMOTOR_FORWARD;
				//				}
			}
			else if (now_up_down_state == DOWN_STATE) // If it is falling and floor_select_list is none of the elements is -1, it will continue to fall.
			{
				if (floor_select_list[FLOOR1] != NOT_SELECTED || floor_select_list[FLOOR2] != NOT_SELECTED || floor_select_list[FLOOR3] != NOT_SELECTED || floor_select_list[FLOOR4] != NOT_SELECTED)
				{
					stepmotor_state = STEPMOTOR_BACKWARD;
				}
				//				if (floor_select_list[(current_floor_state - 1)%4] != -1 || floor_select_list[(current_floor_state - 2)%4] != -1
				//						|| floor_select_list[(current_floor_state - 3)%4] != -1)
				//				{
				//					stepmotor_state = STEPMOTOR_BACKWARD;
				//				}
			}

			for (int i = 0; i < 4; i++) {
				printf("%d ", floor_select_list[i]);
			}
			printf("\n");
		}
	}
}

void stepmotor_forward(void)
{
	now_up_down_state = UP_STATE;
	if (TIM10_1ms_counter1 >= 1)
	{
		stepmotor_drive(j);
		j++;
		j %= 8;
		TIM10_1ms_counter1 = 0;
	}
	// If it reaches a floor and there is a value in the array(floor_select_list) for that floor, it stops.
	if (floor_select_list[current_floor_state] == current_floor_state && interrupt_floor)
	{
		HAL_GPIO_WritePin(GPIOB, 0xff, 0);
		TIM10_elevator_counter = 0;
		stepmotor_state = STEPMOTOR_STOP;
	}

	//delay_us(126); // consider osDelay 1ms
	//	set_rmp(13); // wait for 1126us

}

void stepmotor_backward(void)
{
	now_up_down_state = DOWN_STATE;
	if (TIM10_1ms_counter1 >= 1)
	{
		stepmotor_drive(j);
		j--;
		if (j < 0)
		{
			j = 7;
		}
		TIM10_1ms_counter1 = 0;
	}
	// If it reaches a floor and there is a value in the array(floor_select_list) for that floor, it stops.
	if (floor_select_list[current_floor_state] == current_floor_state && interrupt_floor)
	{
		HAL_GPIO_WritePin(GPIOB, 0xff, 0);
		TIM10_elevator_counter = 0;
		stepmotor_state = STEPMOTOR_STOP;
	}
	//set_rmp(13); // wait for 1126us
}

void get_up_button(void)
{
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, door_state);

	if (TIM10_elevator_counter > 2500)
	{
		door_state = CLOSE_THE_DOOR;
	}


}

void get_down_button(void)
{
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, door_state);

	if (TIM10_elevator_counter > 2500)
	{
		door_state = CLOSE_THE_DOOR;
	}

}

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


void set_rmp(int rmp)
{
	delay_us(60000000/4096/rmp);
	// Maximum speed (13) : delay_us(1126);
}
