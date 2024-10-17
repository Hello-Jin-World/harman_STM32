#include "extint.h"
#include "stepmotor.h"

extern int stepmotor_state;


// move from Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
// external interupt call back function
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
	case GPIO_PIN_1: // PORTA1 // BUTTON0
		stepmotor_state = STEPMOTOR_FORWARD;
		break;
	case GPIO_PIN_4: // PORTA4 // BUTTON1
		stepmotor_state = STEPMOTOR_BACKWARD;
		break;
	}
}

// It is operating did not extern code.
