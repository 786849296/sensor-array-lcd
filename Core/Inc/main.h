/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f1xx_hal.h"

#include "stm32f1xx_ll_adc.h"
#include "stm32f1xx_ll_dma.h"
#include "stm32f1xx_ll_i2c.h"
#include "stm32f1xx_ll_iwdg.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_spi.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "cmsis_os.h"
#include "color.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct
{
	GPIO_TypeDef* group;
	uint32_t pin;
}Gpio;
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
#define EN_B_Pin LL_GPIO_PIN_13
#define EN_B_GPIO_Port GPIOC
#define EN_C_Pin LL_GPIO_PIN_14
#define EN_C_GPIO_Port GPIOC
#define EN_D_Pin LL_GPIO_PIN_15
#define EN_D_GPIO_Port GPIOC
#define ROW0_Pin LL_GPIO_PIN_0
#define ROW0_GPIO_Port GPIOB
#define ROW1_Pin LL_GPIO_PIN_1
#define ROW1_GPIO_Port GPIOB
#define ROW2_Pin LL_GPIO_PIN_2
#define ROW2_GPIO_Port GPIOB
#define DC_Pin LL_GPIO_PIN_8
#define DC_GPIO_Port GPIOA
#define RST_T_Pin LL_GPIO_PIN_15
#define RST_T_GPIO_Port GPIOA
#define RST_S_Pin LL_GPIO_PIN_5
#define RST_S_GPIO_Port GPIOB
#define COL0_Pin LL_GPIO_PIN_6
#define COL0_GPIO_Port GPIOB
#define COL1_Pin LL_GPIO_PIN_7
#define COL1_GPIO_Port GPIOB
#define COL2_Pin LL_GPIO_PIN_8
#define COL2_GPIO_Port GPIOB
#define EN_A_Pin LL_GPIO_PIN_9
#define EN_A_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define MAX_ROW 32
#define MAX_COL 32
extern osSemaphoreId spi2DmaSemaphoreHandle;
extern osSemaphoreId lcdBusySemaphoreHandle;
extern QueueHandle_t adcMailBox;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
