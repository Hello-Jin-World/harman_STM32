#include "servomotor.h"
#define servo0 0
#define servo180 1

extern volatile int TIM10_servomotor_counter;
extern TIM_HandleTypeDef htim3;

void servomotor_main(void);
void servo_motor_control(void);

/* system clock = 84Mhz
 * Timer3 enter 50khz : prescaler 1/1680 -> 50khz
 * T = 1/f = 1/50000 = 0.00002sec (20us)
 * 20ms : 0.00002 x 1000
 * 2ms(180') : 0.00002 x 100
 * 1.5ms(90') : 0.00002 x 75
 * 1ms(0') : 0.00002 x 50
*/
void servomotor_main(void)
{
	static uint8_t servo_state = 0;

	if (TIM10_servomotor_counter >= 1000)
	{
		TIM10_servomotor_counter = 0;
		switch(servo_state)
		{
		case 0: // 180'
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 100); // 2ms(180') : 0.00002 x 100
			break;
		case 1: // 90'
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 60);  // 1.5ms(90') : 0.00002 x 75
			break;
		case 2: // 0'
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 15);  // 1ms(0') : 0.00002 x 50
			break;
		}
		servo_state = (servo_state + 1) % 3;
		//servo_state++;
		//servo_state % 3;
	}
}

uint8_t servo_state = servo0;
// GPIO_PIN_1 : servo motor -> 0' ~> 180' 3ec
// GPIO_PIN_4 : servo motor -> 180' ~> 0'
void servo_motor_control(void)
{
	//static uint8_t servo_state = 0;

	if (servo_state)
	{
		if(TIM10_servomotor_counter < 3000)
		{
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 100); // 2ms(180') : 0.00002 x 100
		}
		if (TIM10_servomotor_counter >= 3000)
		{
			TIM10_servomotor_counter = 0;
			servo_state = servo0;
		}
	}
	else
	{
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 15);  // 1ms(0') : 0.00002 x 50
	}


}
