#include "extint.h"
#include "stepmotor.h"

extern int lets_stop2;
extern int stepmotor_state;
extern uint8_t servo_state;
extern volatile int TIM10_servomotor_counter;
extern int current_floor_state;
extern int interrupt_floor;
extern volatile int TIM10_elevator_counter;


// move from Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
// external interupt call back function
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
	case GPIO_PIN_1: // PORTA1 // BUTTON0
		current_floor_state = FLOOR1; // 1st floor
		break;
	case GPIO_PIN_4: // PORTA4 // BUTTON1
		current_floor_state = FLOOR2; // 2nd floor
		break;
	case GPIO_PIN_11: // PORTA4 // BUTTON1
		current_floor_state = FLOOR3; // 3rd floor
		break;
	case GPIO_PIN_12: // PORTA4 // BUTTON1
		current_floor_state = FLOOR4; // 4th floor
		break;
	}
	interrupt_floor = 1;
}

// It is operating did not extern code.
