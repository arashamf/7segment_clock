/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
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
/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */
// Defines ---------------------------------------------------------------------//

#define 	TimerMsDelay 				        TIM10
#define 	TimerMsDelay_APB2_BIT 		  LL_APB2_GRP1_PERIPH_TIM10 
#define  	TimerMsDelay_IRQn           TIM10_IRQn
#define		TimerMsDelay_IRQHandler		  TIM10_IRQHandler

#define 	TIM_DELAY_us 				TIM7
#define 	PERIODIC 					  1
#define 	ONCE 						    0

// Variables --------------------------------------------------------------------//
uint8_t flag_end_delay = 0;

// Functions --------------------------------------------------------------------//

/* USER CODE END 0 */

/* TIM7 init function */
void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM7);

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  TIM_InitStruct.Prescaler = 31;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 1000;
  LL_TIM_Init(TIM7, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM7);
  LL_TIM_SetTriggerOutput(TIM7, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM7);
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/* USER CODE BEGIN 1 */
//------------------------------------------------------------------------------------//
void delay_us(uint16_t delay)
{
  LL_TIM_SetAutoReload(TIM_DELAY_us, delay); //установка задержки в мкс
	LL_TIM_ClearFlag_UPDATE(TIM_DELAY_us); //сброс флага обновления таймера
	LL_TIM_SetCounter(TIM_DELAY_us, 0); //сброс счётного регистра
	LL_TIM_EnableCounter(TIM_DELAY_us); //включение таймера
	while (LL_TIM_IsActiveFlag_UPDATE(TIM_DELAY_us) == 0) {} //ожидание установки флага обновления таймера 
	LL_TIM_DisableCounter(TIM_DELAY_us); //выключение таймера		
}

//------------------------------------------------------------------------------------//
void TimerMs_init (void)
{
	LL_TIM_InitTypeDef TIM_InitStruct = {0};
	
  	LL_APB2_GRP1_EnableClock(TimerMsDelay_APB2_BIT);
  	TIM_InitStruct.Prescaler = 32000-1;
  	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  	TIM_InitStruct.Autoreload = 1000-1;
  	LL_TIM_Init(TimerMsDelay, &TIM_InitStruct);
  	LL_TIM_DisableARRPreload(TimerMsDelay);
  	LL_TIM_SetTriggerOutput(TimerMsDelay, LL_TIM_TRGO_RESET);
  	LL_TIM_DisableMasterSlaveMode(TimerMsDelay);

	LL_TIM_ClearFlag_UPDATE(TimerMsDelay); //сброс флага обновления таймера
	LL_TIM_EnableIT_UPDATE(TimerMsDelay); //разрешений прерываний от таймера
	LL_TIM_SetCounter(TimerMsDelay, 0); //сброс счётного регистра

  	NVIC_SetPriority  (TimerMsDelay_IRQn , NVIC_EncodePriority(NVIC_GetPriorityGrouping(),2, 0));
  	NVIC_EnableIRQ(TimerMsDelay_IRQn ); //включение прерывания от таймера
  	LL_TIM_EnableCounter(TimerMsDelay); //включение таймера	
}

//------------------------------------------------------------------------------------//
void TimerMsDelay_IRQHandler (void)	
{
	if (LL_TIM_IsActiveFlag_UPDATE(TimerMsDelay) == SET)
	{	
		LL_TIM_ClearFlag_UPDATE (TimerMsDelay); 
		LL_TIM_DisableCounter(TimerMsDelay); 
		flag_end_delay = RESET;
	}
}

//------------------------------------------------------------------------------------//
void delay_ms (uint16_t delay)
{
	LL_TIM_SetAutoReload(TimerMsDelay, delay); 
	LL_TIM_SetCounter(TimerMsDelay, 0); 
	LL_TIM_ClearFlag_UPDATE(TimerMsDelay);
	LL_TIM_EnableCounter(TimerMsDelay); 
  	flag_end_delay = 1;
}

//-----------------------------------------------------------------------------------//
void delay_LED_digital (uint8_t numb)
{
    if (numb <4)
    {  delay_us(1000); }
    else
    {  delay_us(750); }
}

//-----------------------------------------------------------------------------------//

/* USER CODE END 1 */
