#include "keypad.h"

#if 1
GPIO_TypeDef* keypadRowPort[4] = {ROW1_GPIO_Port, ROW2_GPIO_Port, ROW3_GPIO_Port, ROW4_GPIO_Port}; //R1~R4
GPIO_TypeDef* keypadColPort[4] = {COL1_GPIO_Port, COL2_GPIO_Port, COL3_GPIO_Port, COL4_GPIO_Port}; //C1~C4
uint16_t keypadRowPin[4] = {ROW1_Pin, ROW2_Pin, ROW3_Pin, ROW4_Pin}; //R1~R4 GPIO Input & Pull-up으로 설정을 해야 한다.
uint16_t keypadColPin[4] = {COL1_Pin, COL2_Pin, COL3_Pin, COL4_Pin}; //C1~C4  GPIO Output으로만 설정 한다.
#else  // orginal 
GPIO_TypeDef* keypadRowPort[4] = {GPIOF, GPIOF, GPIOF, GPIOG}; //R1~R4
GPIO_TypeDef* keypadColPort[4] = {GPIOE, GPIOE, GPIOE, GPIOE}; //C1~C4
uint16_t keypadRowPin[4] = {GPIO_PIN_8, GPIO_PIN_7, GPIO_PIN_9, GPIO_PIN_1}; //R1~R4
uint16_t keypadColPin[4] = {GPIO_PIN_3, GPIO_PIN_6, GPIO_PIN_5, GPIO_PIN_4}; //C1~C4
#endif 
void keypadInit()
{
	for(uint8_t col = 0; col < 4; col++)
	{
		HAL_GPIO_WritePin(keypadColPort[col], keypadColPin[col], SET); //초기 값 1로 셋팅 // for not work button when Power ON.
	}
}

uint8_t getKeypadState(uint8_t col, uint8_t row)
{
#if 1
	uint8_t keypadChar[4][4] = { // mapping table
			{'/', '3', '2', '1'},
			{'*', '6', '5', '4'},
			{'-', '9', '8', '7'},
			{'+', '=', '0', ' '},
	};

#else
	uint8_t keypadChar[4][4] = {
			{'1', '4', '7', '*'},
			{'2', '5', '8', '0'},
			{'3', '6', '9', '#'},
			{'A', 'B', 'C', 'D'},
	};
#endif
	static uint8_t prevState[4][4] = { // all button are not pressed.
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
	};
	uint8_t curState = 1;

	HAL_GPIO_WritePin(keypadColPort[col], keypadColPin[col], SET);
	curState = HAL_GPIO_ReadPin(keypadRowPort[row], keypadRowPin[row]);

	HAL_GPIO_WritePin(keypadColPort[col], keypadColPin[col], RESET);

	if(curState == PUSHED && prevState[col][row] == RELEASED)
	{
		prevState[col][row] = curState;
		return 0;
	}
	else if (curState == RELEASED && prevState[col][row] == PUSHED)
	{
		prevState[col][row] = curState;
		printf("%c\n", keypadChar[col][row]);
		return keypadChar[col][row];
	}
	return 0;
}

uint8_t keypadScan()
{
	uint8_t data;

	for(uint8_t col=0; col<4; col++)
	{
		for(uint8_t row=0; row<4; row++)
		{
			data = getKeypadState(col, row);
			if(data != 0)
			{
				return data;
			}
		}
	}
	return 0;
}
