/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32l1xx_hal.h"

#include "stm32l1xx_ll_i2c.h"
#include "stm32l1xx_ll_rcc.h"
#include "stm32l1xx_ll_bus.h"
#include "stm32l1xx_ll_system.h"
#include "stm32l1xx_ll_exti.h"
#include "stm32l1xx_ll_cortex.h"
#include "stm32l1xx_ll_utils.h"
#include "stm32l1xx_ll_pwr.h"
#include "stm32l1xx_ll_dma.h"
#include "stm32l1xx_ll_tim.h"
#include "stm32l1xx_ll_usart.h"
#include "stm32l1xx_ll_gpio.h"

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
#define A0_Pin LL_GPIO_PIN_0
#define A0_GPIO_Port GPIOA
#define A1_Pin LL_GPIO_PIN_1
#define A1_GPIO_Port GPIOA
#define A2_Pin LL_GPIO_PIN_2
#define A2_GPIO_Port GPIOA
#define A3_Pin LL_GPIO_PIN_3
#define A3_GPIO_Port GPIOA
#define A4_Pin LL_GPIO_PIN_4
#define A4_GPIO_Port GPIOA
#define A5_Pin LL_GPIO_PIN_5
#define A5_GPIO_Port GPIOA
#define SB_Pin LL_GPIO_PIN_0
#define SB_GPIO_Port GPIOB
#define SA_Pin LL_GPIO_PIN_1
#define SA_GPIO_Port GPIOB
#define SJ_Pin LL_GPIO_PIN_10
#define SJ_GPIO_Port GPIOB
#define SH_Pin LL_GPIO_PIN_12
#define SH_GPIO_Port GPIOB
#define SDP_Pin LL_GPIO_PIN_13
#define SDP_GPIO_Port GPIOB
#define CLOCK_INT_Pin LL_GPIO_PIN_14
#define CLOCK_INT_GPIO_Port GPIOB
#define SM_Pin LL_GPIO_PIN_15
#define SM_GPIO_Port GPIOB
#define SC_Pin LL_GPIO_PIN_3
#define SC_GPIO_Port GPIOB
#define SD_Pin LL_GPIO_PIN_4
#define SD_GPIO_Port GPIOB
#define SE_Pin LL_GPIO_PIN_5
#define SE_GPIO_Port GPIOB
#define SF_Pin LL_GPIO_PIN_8
#define SF_GPIO_Port GPIOB
#define SG_Pin LL_GPIO_PIN_9
#define SG_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
//Define --------------------------------------------------------------------------//
#define __USE_DBG

//Prototypes-----------------------------------------------------------------------//
uint8_t return_flag_timeout (void);

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
