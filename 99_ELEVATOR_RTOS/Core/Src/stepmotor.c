#include "stepmotor.h"
#include "button.h"
#include <stdlib.h>  // malloc, free 사용


extern void delay_us(unsigned int us);
extern int get_button(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, int button_num);
extern void init_arrow_up(void);
extern void init_arrow_down(void);
extern volatile int TIM10_1ms_counter1;
extern volatile int TIM10_DHT11_counter;

void set_rmp(int rmp);
void stepmotor_main(void);
void stepmotor_drive(int step);
void stepmotor_forward(void);
void stepmotor_backward(void);
void stepmotor_stop(void);
void select_floor(void);
void init_floor_select(int size);
void reset_floor_select(void);

int j = 0;
int stepmotor_state = 0;
int floor_select_state = 0;
int current_floor_state = 0;
uint8_t *floor_select_list = NULL;  // 동적 할당을 위한 포인터
int floor_select_size = 0;      // 동적 할당할 배열 크기
int wow = 0;
int lets_stop = 0;
int wow2 = 0;

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

void stepmotor_main(void)
{
	(*stepmotor_funcp[stepmotor_state])();

	if (get_button(GPIOC, GPIO_PIN_2, BUTTON2) == BUTTON_PRESS)
	{
		if (stepmotor_state == STEPMOTOR_BACKWARD || stepmotor_state == STEPMOTOR_FORWARD)
		{
			stepmotor_state = STEPMOTOR_STOP;
		}
	}

	if (get_button(GPIOC, GPIO_PIN_3, BUTTON3) == BUTTON_PRESS)
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

void select_floor(void)
{

	int temp;
	static int is_duplicate = 0;

	(*stepmotor_funcp[stepmotor_state])();


	if (get_button(GPIOC, GPIO_PIN_0, BUTTON0) == BUTTON_PRESS)
	{
		lets_stop = 1;
#if 1
		// 현재 선택된 층이 이미 리스트에 있는지 확인
		for (int i = 0; i < wow; i++)
		{
			if (floor_select_list[i] == floor_select_state)
			{
				is_duplicate = 1;
				break;
			}
		}

		// 중복이 아니면 리스트에 추가하고 정렬
		if (!is_duplicate && wow < floor_select_size)
		{
			floor_select_list[wow] = floor_select_state;
			wow++;

			// 오름차순으로 정렬
			for (int i = 0; i < wow - 1; i++)
			{
				for (int j = i + 1; j < wow; j++)
				{
					if (floor_select_list[i] > floor_select_list[j])
					{
						temp = floor_select_list[i];
						floor_select_list[i] = floor_select_list[j];
						floor_select_list[j] = temp;
					}
				}
			}
		}
#else
		for (int i = 0; i < 4; i++)
		{
			for (int j = i + 1; j < 5; j++)
			{
				if (floor_select_list[i] > floor_select_list[j])
				{
					temp = floor_select_list[i];
					floor_select_list[i] = floor_select_list[j];
					floor_select_list[j] = temp;

				}
			}
		}
#endif
		printf("Current List Size: %d\n", wow);
		    for (int i = 0; i < wow; i++) {
		        printf("%d ", floor_select_list[i]);
		    }
		    printf("\n");
		wow++;

		if (floor_select_state > current_floor_state)
		{
			stepmotor_state = STEPMOTOR_FORWARD;
		}
		else if (floor_select_state < current_floor_state)
		{
			stepmotor_state = STEPMOTOR_BACKWARD;
		}
		//		else
		//		{
		//			stepmotor_state = STEPMOTOR_STOP;
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

void init_floor_select(int size)
{
    // 리스트의 크기 설정 및 메모리 할당
    floor_select_size = size;
    floor_select_list = (int*)malloc(floor_select_size * sizeof(int));

    if (floor_select_list == NULL)
    {
        printf("메모리 할당 실패\n");
        return;
    }

    // 리스트 초기화
    reset_floor_select();
}

void reset_floor_select(void)
{
	 memset(floor_select_list, 255, floor_select_size * sizeof(int));

	// wow 초기화
	wow = 0;

	printf("list reset!!!!\n");
}

void free_floor_select(void)
{
    // 메모리 해제
    if (floor_select_list != NULL)
    {
        free(floor_select_list);
        floor_select_list = NULL;
    }
}

void stepmotor_stop(void)
{
	//stepmotor_drive(j);
#if 1
	if (lets_stop)
	{
		if (TIM10_DHT11_counter <= 5000)
		{
			stepmotor_drive(j);

		}
		if (TIM10_DHT11_counter > 5000)
		{
			if (floor_select_list[wow2+1] > 10 || floor_select_list[wow2+1] < 0)
			{
				reset_floor_select();
				lets_stop = 0;
				printf("255 255 255 255 255 255 255\n");
			}

			if (floor_select_list[wow2+1] > current_floor_state)// && (floor_select_list[wow2+1] != 255))
			{
				printf("%d\n",floor_select_list[wow2+1]);

				stepmotor_state = STEPMOTOR_FORWARD;
				wow2++;
			}

			else
			{
				stepmotor_drive(j);
			}

			lets_stop = 0;
			TIM10_DHT11_counter = 0;
		}
	}
	else
	{
		stepmotor_drive(j);
	}
#endif
#if 0
	else
	{
		if (floor_select_list[0] > current_floor_state)
		{
			lets_stop = 1;
			stepmotor_state = STEPMOTOR_FORWARD;
		}
		else if (floor_select_list[1] > current_floor_state)
		{
			lets_stop = 1;
			stepmotor_state = STEPMOTOR_FORWARD;
		}
		else if (floor_select_list[2] > current_floor_state)
		{
			lets_stop = 1;
			stepmotor_state = STEPMOTOR_FORWARD;
		}
		else if (floor_select_list[3] > current_floor_state)
		{
			lets_stop = 1;
			stepmotor_state = STEPMOTOR_FORWARD;
		}
	}
#endif
}



void stepmotor_forward(void)
{

	if (TIM10_1ms_counter1 >= 16000/4096/13)
	{
		stepmotor_drive(j);
		j++;
		j %= 8;
		TIM10_1ms_counter1 = 0;
	}
#if 1
	if (current_floor_state == floor_select_list[wow2])//current_floor_state)//floor_select_list[wow])
	{
		HAL_GPIO_WritePin(GPIOB, 0xff, 0);
		TIM10_DHT11_counter = 0;
		stepmotor_state = STEPMOTOR_STOP;
	}
#else
	for (int i = 0; i < 4; i++)
	{
		if (floor_select_list[wow] == current_floor_state)
		{
			HAL_GPIO_WritePin(GPIOB, 0xff, 0);
			TIM10_DHT11_counter = 0;
			stepmotor_state = STEPMOTOR_STOP;
		}
		else
		{
			wow++;
			wow %= 4;
		}
	}


#endif
	//delay_us(126); // consider osDelay 1ms
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
		TIM10_DHT11_counter = 0;
		stepmotor_state = STEPMOTOR_STOP;
	}
	//set_rmp(13); // wait for 1126us

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
