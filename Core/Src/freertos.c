/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "rtc.h"
#include "typedef.h"
#include "usart.h"
#include "LED_display.h"
#include "time_protocol.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
osTimerId osProgTimerI2C_timeout;

osThreadId MessagingDS3231TaskHandle;
osThreadId SetupTimeonBoardTaskHandle;
osThreadId ParseUARTmsgTaskHandle;

osMutexId I2CmutexHandle; //мьютекс блокировки передачи команд ячейкам

uint8_t timeout_flag = OFF;
__IO uint32_t UNIXtimeDS3231 = 0;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void ProgTimerI2C_timeoutCallback (void const *argument);

void SetupTimeonBoard(void const * argument);
void LaunchMessagingDS3231 (void const * argument);
void ParseUARTmsg  (void const * argument);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
  *ppxTimerTaskStackBuffer = &xTimerStack[0];
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
  /* place for user code */
}
/* USER CODE END GET_TIMER_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  osMutexDef (I2CmutexHandle); 
	I2CmutexHandle = osMutexCreate(osMutex (I2CmutexHandle));
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  osTimerDef (TimerI2C_timeout, ProgTimerI2C_timeoutCallback);
	osProgTimerI2C_timeout = osTimerCreate(osTimer (TimerI2C_timeout), osTimerOnce, NULL);
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityIdle, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  
  osThreadDef(TaskSetupTimeonBoard, SetupTimeonBoard, osPriorityNormal , 0, 128);
  SetupTimeonBoardTaskHandle = osThreadCreate(osThread(TaskSetupTimeonBoard), NULL);

  osThreadDef(TaskMessagingDS3231, LaunchMessagingDS3231, osPriorityNormal , 0, 128);
  MessagingDS3231TaskHandle = osThreadCreate(osThread(TaskMessagingDS3231), NULL);

  osThreadDef(TaskParseUARTmsg, ParseUARTmsg, osPriorityNormal , 0, 128);
  ParseUARTmsgTaskHandle = osThreadCreate(osThread(TaskParseUARTmsg), NULL);
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(100);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
//---------------------------------------------------------------------------//
void LaunchMessagingDS3231 (void const * argument)
{
  uint32_t tickcount = osKernelSysTick();

  for(;;)
  {
    if (osMutexWait (I2CmutexHandle, 20) == osOK)
		{	
        timeout_flag = OFF;
        osTimerStart (osProgTimerI2C_timeout, I2C_DELAY);
        UNIXtimeDS3231 = GetUnixTime_from_DS3231();
        osMutexRelease (I2CmutexHandle);
        osDelayUntil (&tickcount, 1000);
    }
  }
}

//---------------------------------------------------------------------------//
void SetupTimeonBoard (void const * argument)
{
  for(;;)
  {
    if (UNIXtimeDS3231 > 0)
    { 
      LED_data_output ((char *)return_HandleTimeBuff(), NUMB_DIGITS_BOARD);
    }  
    osDelay (3);
  }  
}

//---------------------------------------------------------------------------//
void ParseUARTmsg (void const * argument)
{
  __IO uint32_t TimeUART = 0;
  static uint8_t flagGetMsg = NO_MSG;

  for(;;)
  {
    if (flagGetMsg == NO_MSG)
    {
      if (check_ring_buffer () == GET_MSG) //проверка кольцевого буффера
      {
        if ((TimeUART = return_UNIXtimeNTP()) > 0) //возвращает UNIX-время, полученное от ntp сервера
        {
          //если UNIX-время от ntp-сервера не равно времени от DS3231 и отличается больше, чем на единицу
          if ((UNIXtimeDS3231 != TimeUART) && ((TimeUART-UNIXtimeDS3231)>1)) 
          {

            #ifdef __USE_DBG
	 	        //  snprintf (DBG_buffer,  BUFFER_SIZE, "DS3231=%ld,NTP=%ld\r\n", UNIXtimeDS3231, TimeUART);	          
      	    //  DBG_PutString(DBG_buffer);
	          #endif
            flagGetMsg = GET_MSG; //установка флага получения сообщения с данными времени от ntp-сервера     
          }
        }
      }
      osDelay (3);
    }
    else
    {
      if (osMutexWait (I2CmutexHandle, 20) == osOK)
		  {	
        flagGetMsg = NO_MSG;
        timeout_flag = OFF; //сброс флага таймаута I2C
        osTimerStart (osProgTimerI2C_timeout, I2C_DELAY); //запуск таймера таймаута I2C
        PrepareSendTimedata_to_DS3231(TimeUART); //подготовка и передача данных времени в DS3231
        osMutexRelease (I2CmutexHandle); //освобождение мьютекса
        osDelay (1000);
      }
    }
  }
}

//---------------------------------------------------------------------------//
void ProgTimerI2C_timeoutCallback (void const *argument)
{
  timeout_flag = ON;
}

//---------------------------------------------------------------------------//
uint8_t return_flag_timeout (void)
{
  return timeout_flag;
}
/* USER CODE END Application */

