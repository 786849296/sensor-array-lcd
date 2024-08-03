/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
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
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_TIM3_Init(void);
void MX_TIM4_Init(void);

/* USER CODE BEGIN Prototypes */
__STATIC_INLINE void TIM3CH1_setDuty(uint32_t duty)
{
    uint32_t period = LL_TIM_GetAutoReload(TIM3);
    uint32_t time = duty * period / 100;
    LL_TIM_OC_SetCompareCH1(TIM3, time);
}
__STATIC_INLINE void TIM3CH2_setDuty(uint32_t duty)
{
    uint32_t period = LL_TIM_GetAutoReload(TIM3);
    uint32_t time = duty * period / 100;
    LL_TIM_OC_SetCompareCH2(TIM3, time);
}
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

