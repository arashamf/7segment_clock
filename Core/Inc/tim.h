/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
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
// Defines ---------------------------------------------------------------------//
#define 	 MAX_xTIMERS 								     2

//Exported types -----------------------------------------------------------------//
typedef void * xTimerHandle; //указатель типа void на структуру таймера 
typedef void (*SysTick_CALLBACK)(uint32_t); //указатель типа void на ф-ю обработки (коллбэка) системного таймера
typedef void (*tmrTIMER_CALLBACK)(xTimerHandle xTimer);	//указатель на коллбэк программного таймера (вызывается из коллбэка системного таймера)
typedef uint64_t (*portTickType)(void); //указатель типа void на ф-ю возврата системного времени

//--------------------------------------------------------------------------------//
typedef enum 
{
	__IDLE      =   0 , 
	__ACTIVE          , 
	__DONE
} tmrTIMER_STATE;

//--------------------------------------------------------------------------------//
typedef struct
{
	uint32_t expiry;
  uint32_t periodic;
	tmrTIMER_STATE State;
	FunctionalState	AutoReload;
	tmrTIMER_CALLBACK	CallbackFunction;
} xTIMER;
/* USER CODE END Private defines */

void MX_TIM7_Init(void);

/* USER CODE BEGIN Prototypes */
//Prototypes-----------------------------------------------------------------------//
void delay_us(uint16_t );
void delay_ms (uint16_t );
void timers_ini (void);

xTimerHandle xTimer_Create(uint32_t xTimerPeriodInTicks, FunctionalState AutoReload, 
tmrTIMER_CALLBACK CallbackFunction, FunctionalState NewState);
void xTimer_SetPeriod(xTimerHandle , uint32_t );
void xTimer_Reload(xTimerHandle );
void xTimer_Delete(xTimerHandle );
void xTimer_Task(uint32_t portTick);
void xSysTimer_Reload (uint16_t );

uint64_t Get_SysTick(void);
uint8_t systick_delay (uint64_t );
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

