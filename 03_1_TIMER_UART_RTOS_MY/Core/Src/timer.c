#include "timer.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

// delay_us(10); When use 10us delay function.
void delay_us(unsigned int us);
void delay_ms(unsigned int ms);

// Prescaled frequency is entered TIM2 from 82MHz to 1MHz.
// T = 1/f
void delay_us(unsigned int us)
{
	// TCNT = 0; at AVR.
	__HAL_TIM_SET_COUNTER(&htim2, 0); // 2nd Timer/Counter clear.
	// stay on user set us.
	while(__HAL_TIM_GET_COUNTER(&htim2) < us) // Waiting to meet user set pulse
		; // NOP
}

void delay_ms(unsigned int ms)
{
	// TCNT = 0; at AVR.
	__HAL_TIM_SET_COUNTER(&htim3, 0); // 2nd Timer/Counter clear.
	// stay on user set us.
	while(__HAL_TIM_GET_COUNTER(&htim3) * 20 < ms) // Waiting to meet user set pulse
		; // NOP
}
