/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ROW4_Pin GPIO_PIN_3
#define ROW4_GPIO_Port GPIOE
#define ROW1_Pin GPIO_PIN_4
#define ROW1_GPIO_Port GPIOE
#define ROW2_Pin GPIO_PIN_5
#define ROW2_GPIO_Port GPIOE
#define ROW3_Pin GPIO_PIN_6
#define ROW3_GPIO_Port GPIOE
#define USER_Btn_Pin GPIO_PIN_13
#define USER_Btn_GPIO_Port GPIOC
#define COL2_Pin GPIO_PIN_7
#define COL2_GPIO_Port GPIOF
#define COL1_Pin GPIO_PIN_8
#define COL1_GPIO_Port GPIOF
#define COL3_Pin GPIO_PIN_9
#define COL3_GPIO_Port GPIOF
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOH
#define RMII_MDC_Pin GPIO_PIN_1
#define RMII_MDC_GPIO_Port GPIOC
#define RMII_REF_CLK_Pin GPIO_PIN_1
#define RMII_REF_CLK_GPIO_Port GPIOA
#define RMII_MDIO_Pin GPIO_PIN_2
#define RMII_MDIO_GPIO_Port GPIOA
#define RMII_CRS_DV_Pin GPIO_PIN_7
#define RMII_CRS_DV_GPIO_Port GPIOA
#define RMII_RXD0_Pin GPIO_PIN_4
#define RMII_RXD0_GPIO_Port GPIOC
#define RMII_RXD1_Pin GPIO_PIN_5
#define RMII_RXD1_GPIO_Port GPIOC
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#define COL4_Pin GPIO_PIN_1
#define COL4_GPIO_Port GPIOG
#define D0_LCD_Pin GPIO_PIN_8
#define D0_LCD_GPIO_Port GPIOE
#define D1_LCD_Pin GPIO_PIN_9
#define D1_LCD_GPIO_Port GPIOE
#define D2_LCD_Pin GPIO_PIN_10
#define D2_LCD_GPIO_Port GPIOE
#define D3_LCD_Pin GPIO_PIN_11
#define D3_LCD_GPIO_Port GPIOE
#define D4_LCD_Pin GPIO_PIN_12
#define D4_LCD_GPIO_Port GPIOE
#define D5_LCD_Pin GPIO_PIN_13
#define D5_LCD_GPIO_Port GPIOE
#define D6_LCD_Pin GPIO_PIN_14
#define D6_LCD_GPIO_Port GPIOE
#define D7_LCD_Pin GPIO_PIN_15
#define D7_LCD_GPIO_Port GPIOE
#define RW_LCD_Pin GPIO_PIN_10
#define RW_LCD_GPIO_Port GPIOB
#define RS_LCD_Pin GPIO_PIN_11
#define RS_LCD_GPIO_Port GPIOB
#define E_LCD_Pin GPIO_PIN_12
#define E_LCD_GPIO_Port GPIOB
#define RMII_TXD1_Pin GPIO_PIN_13
#define RMII_TXD1_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define IO_DS1302_Pin GPIO_PIN_15
#define IO_DS1302_GPIO_Port GPIOB
#define STLK_RX_Pin GPIO_PIN_8
#define STLK_RX_GPIO_Port GPIOD
#define STLK_TX_Pin GPIO_PIN_9
#define STLK_TX_GPIO_Port GPIOD
#define USB_PowerSwitchOn_Pin GPIO_PIN_6
#define USB_PowerSwitchOn_GPIO_Port GPIOG
#define USB_OverCurrent_Pin GPIO_PIN_7
#define USB_OverCurrent_GPIO_Port GPIOG
#define CE_DS1302_Pin GPIO_PIN_6
#define CE_DS1302_GPIO_Port GPIOC
#define BUTTON0_Pin GPIO_PIN_8
#define BUTTON0_GPIO_Port GPIOC
#define BUTTON1_Pin GPIO_PIN_9
#define BUTTON1_GPIO_Port GPIOC
#define USB_SOF_Pin GPIO_PIN_8
#define USB_SOF_GPIO_Port GPIOA
#define USB_VBUS_Pin GPIO_PIN_9
#define USB_VBUS_GPIO_Port GPIOA
#define USB_ID_Pin GPIO_PIN_10
#define USB_ID_GPIO_Port GPIOA
#define USB_DM_Pin GPIO_PIN_11
#define USB_DM_GPIO_Port GPIOA
#define USB_DP_Pin GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define CLK_DS1302_Pin GPIO_PIN_15
#define CLK_DS1302_GPIO_Port GPIOA
#define BUTTON2_Pin GPIO_PIN_10
#define BUTTON2_GPIO_Port GPIOC
#define BUTTON3_Pin GPIO_PIN_11
#define BUTTON3_GPIO_Port GPIOC
#define RMII_TX_EN_Pin GPIO_PIN_11
#define RMII_TX_EN_GPIO_Port GPIOG
#define RMII_TXD0_Pin GPIO_PIN_13
#define RMII_TXD0_GPIO_Port GPIOG
#define LD2_Pin GPIO_PIN_7
#define LD2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
typedef struct ds1302
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hour;
	uint8_t date;
	uint8_t month;
	uint8_t dayofweek; // 1: sun, 2: mon ...
	uint8_t year;
	uint8_t ampm; // 0 : am, 1: pm
	uint8_t hourmode; // 0: 24 format, 1: 12 format
	uint8_t writeprotected; // 0: 24 format, 1: 12 format
} t_ds1302;

typedef struct set_time
{
	uint32_t magic;  // check for factory reset state or non-reset state  // 4byte -> 1 word
	uint8_t Hours; // 1byte
	uint8_t Minutes; // 1byte
	uint8_t Seconds; // 1byte
	uint8_t dummy1; // for memory alignment -> 1 word (dummy data)
	uint8_t Year; // 1byte
	uint8_t Month; // 1byte
	uint8_t Date; // 1byte
//	uint8_t dummy2; // for memory alignment -> 1 word (dummy data)
} t_set_time;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
