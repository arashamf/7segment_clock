/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */
#include <stdio.h>
#include "typedef.h"
#include "time_protocol.h"

// defines--------------------------------------------------------------------------//
#define 	DBG_UART 			USART1
#define   NTP_UART		  USART1

//variables-------------------------------------------------------------------------//
#ifdef __USE_DBG
  char DBG_buffer[BUFFER_SIZE];
#endif
uint8_t uart_buffer[BUFFER_SIZE]; //массив для кольцевого буффера

//prototypes-----------------------------------------------------------------------//
static void UART_PutByte(char );

/* USER CODE END 0 */

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**USART1 GPIO Configuration
  PA9   ------> USART1_TX
  PA10   ------> USART1_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USART1 interrupt Init */
  NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5, 0));
  NVIC_EnableIRQ(USART1_IRQn);

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_DisableIT_CTS(USART1);
  LL_USART_ConfigAsyncMode(USART1);
  LL_USART_Enable(USART1);
  /* USER CODE BEGIN USART1_Init 2 */
  LL_USART_Disable(USART1);
  LL_USART_ClearFlag_RXNE(USART1); // сброс флага прерывания по приёму
  LL_USART_EnableIT_RXNE(USART1);
  LL_USART_Enable(USART1);

  /* USER CODE END USART1_Init 2 */

}

/* USER CODE BEGIN 1 */
//------------------------------передача строки по UART1------------------------------//
void DBG_PutString(const char *str)
{
	char c;
	while((c = *str++))
	{
		while (LL_USART_IsActiveFlag_TC(DBG_UART) != SET) {}
		LL_USART_TransmitData8(DBG_UART, c);
	}
}

//-------------------------------передача символа по UART-----------------------------------//
static void UART_PutByte(char c)
{
  while(!(NTP_UART->SR & USART_SR_TC)) {}; 
  NTP_UART->DR = c; 
}

//-------------------------------передача строки по UART-----------------------------------//
void UART_PutString (const char *str)
{
	char c;
	while((c = *str++))
	{ UART_PutByte(c);  }
}

//-------------------------------получение символа по UART1-----------------------------------//
void UART_CharReception_Callback (void)
{
  auto uint8_t smb;
	smb = LL_USART_ReceiveData8(NTP_UART);
  PutCharRingBuf(smb);
}

//----------------------------возврат указателя на UART буффер--------------------------------//
uint8_t * returnPtrUARTbuf (void)
{
  return uart_buffer;
}
/* USER CODE END 1 */
