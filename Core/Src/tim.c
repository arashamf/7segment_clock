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
#define 	SysTimer 					              TIM6
#define 	SysTimer_APB1_BIT 			        LL_APB1_GRP1_PERIPH_TIM6
#define  	SysTimer_IRQn                   TIM6_IRQn
#define		SysTimer_IRQHandler			        TIM6_IRQHandler
#define 	TIM_DELAY_us 					          TIM7
#define 	PERIODIC 										    1
#define 	ONCE 												    0

// Variables --------------------------------------------------------------------//
static portTickType xTimeNow; //указатель на ф-ю возврата системного времени вида uint32_t Get_SysTick(void)
static SysTick_CALLBACK SysTick_CallbackFunction = NULL; //инициализация указателя на ф-ю обработки (коллбэка) системного таймера
static __IO uint64_t TicksCounter = 0; //счётчик системных тиков
static xTIMER xTimerList[MAX_xTIMERS]; //массив структур программных таймеров
static xTimerHandle xSysTimer;

// Functions --------------------------------------------------------------------//
static void SysTimer_init (void);
static uint32_t Get_SysTick(void);
static void vSysTimerCallback(xTimerHandle xTimer); 

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
static void SysTimer_init (void)
{
	LL_TIM_InitTypeDef TIM_InitStruct = {0};
	
  LL_APB1_GRP1_EnableClock(SysTimer_APB1_BIT);
  TIM_InitStruct.Prescaler = 31;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 1000-1;
  LL_TIM_Init(SysTimer, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(SysTimer);
  LL_TIM_SetTriggerOutput(SysTimer, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(SysTimer);

	LL_TIM_ClearFlag_UPDATE(SysTimer); //сброс флага обновления таймера
	LL_TIM_EnableIT_UPDATE(SysTimer); //разрешений прерываний от таймера
	LL_TIM_SetCounter(SysTimer, 0); //сброс счётного регистра

  NVIC_SetPriority  (SysTimer_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(SysTimer_IRQn); //включение прерывания от таймера
  LL_TIM_EnableCounter(SysTimer); //включение таймера	

}

//------------------------------------------------------------------------------------//
void SysTimer_IRQHandler(void)
{
	if (LL_TIM_IsActiveFlag_UPDATE(SysTimer) == SET)
	{	
	  LL_TIM_ClearFlag_UPDATE (SysTimer); //сброс флага обновления таймера	
    TicksCounter++; 
    LL_TIM_EnableCounter(SysTimer); //включение таймера	
	  if ( SysTick_CallbackFunction != NULL ) //если указатель на коллбэк xTimer_Task проинициализирован
	  { SysTick_CallbackFunction(TicksCounter); } //вызов коллбэка
	}
}

//------------------------------------------------------------------------------------//
void timers_ini (void)
{
  SysTimer_init(); 		//инициализация TIM6 для привязки к xTimer
  SysTick_CallbackFunction = xTimer_Task; //присвоение указателю значения ф-и xTimer_Task
  xTimeNow = Get_SysTick; //инициализация указателя на ф-ю получения системного времени
  
  xSysTimer = xTimer_Create(1000, PERIODIC, &vSysTimerCallback, ENABLE);
}

//------------------------------------------------------------------------------------//
xTimerHandle xTimer_Create(uint32_t xTimerPeriodInTicks, FunctionalState AutoReload, 
tmrTIMER_CALLBACK CallbackFunction, FunctionalState NewState)
{
	xTimerHandle NewTimer = NULL; //инициализация указателя на структуру для программного таймера
	
	for (uint16_t count = 0; count < MAX_xTIMERS; count++) //поиск свободного элемента массива структур с таймерами
	{
		if (xTimerList[count].CallbackFunction == NULL) 		//если указатель на ф-ю коллбэка пуст
		{
			xTimerList[count].periodic = xTimerPeriodInTicks;		//время задержки в мс
			xTimerList[count].AutoReload = AutoReload;				//периодический запуск или одиночный
			xTimerList[count].CallbackFunction = CallbackFunction;	//указатель на ф-ю коллбэка
			
			if (NewState != DISABLE) //если новый таймер в состояние  ENABLE
			{
				xTimerList[count].expiry = xTimeNow() + xTimerPeriodInTicks; //вычисление времени следующего вызова коллбэка
				xTimerList[count].State = __ACTIVE; //состояние таймера активен
			} 
			else 
			{	xTimerList[count].State = __IDLE;	}	//если новый таймер в состояние  DISABLE	
			NewTimer = (xTimerHandle)(count+1); //возврат номера указателя на структуру таймера
			break;
    }
  }
	return NewTimer;
}

//------------------------------------------------------------------------------------//
void xTimer_SetPeriod(xTimerHandle xTimer, uint32_t xTimerPeriodInTicks) 
{
	if ( xTimer != NULL )  
	{	xTimerList[(uint32_t)xTimer-1].periodic = xTimerPeriodInTicks;	} //установка нового значения задержки таймера
}

//------------------------------------------------------------------------------------//
void xTimer_Reload(xTimerHandle xTimer) 
{
	if ( xTimer != NULL ) 
	{
		xTimerList[(uint32_t)xTimer-1].expiry = xTimeNow() + xTimerList[(uint32_t)xTimer-1].periodic; //установка уставки таймера
		xTimerList[(uint32_t)xTimer-1].State = __ACTIVE; //запуск таймера
	}
}

//------------------------------------------------------------------------------------//
void xTimer_Delete(xTimerHandle xTimer)
{
	if ( xTimer != NULL ) 
	{
		xTimerList[(uint32_t)xTimer-1].CallbackFunction = NULL;
		xTimerList[(uint32_t)xTimer-1].State = __IDLE;
		xTimer = NULL;
	}		
}

//--------------------------------ф-я диспетчера xTimer--------------------------------//
void xTimer_Task(uint32_t portTick)
{
	uint16_t i;
	
	for (i = 0; i < MAX_xTIMERS; i++) 
  {
		switch (xTimerList[i].State) 
		{
			case __ACTIVE: //если таймер активен
				if ( portTick >= xTimerList[i].expiry ) //если текущее системное время больше или равно установленному
				{				
					if ( xTimerList[i].AutoReload != DISABLE ) //если таймер периодический
					{	xTimerList[i].expiry = portTick + xTimerList[i].periodic;	} //установка нового времени срабатывания таймера
					else 
					{	xTimerList[i].State = __IDLE;	}	//статус таймера не активен		
					xTimerList[i].CallbackFunction((xTimerHandle)(i+1)); //вызов коллбэка
				}					
				break;
				
			default:
				break;
		}
	}	
}

//-----------------------------------------------------------------------------------//
static void vSysTimerCallback(xTimerHandle xTimer) 
{
  SysTimerCallback ();
}

//-----------------------------------------------------------------------------------//
void xSysTimer_Reload (uint16_t delay_ms)
{
	xTimer_SetPeriod(xSysTimer, delay_ms);
	xTimer_Reload(xSysTimer);	
}

// возвращает значение SysTick--------------------------------------------------------//
static uint32_t Get_SysTick(void)
{ 
  return TicksCounter;  
}

/* USER CODE END 1 */
