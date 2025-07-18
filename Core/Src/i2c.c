/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
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
#include "i2c.h"

/* USER CODE BEGIN 0 */
#include "tim.h"
#include "typedef.h"
#include "usart.h"
#include <stdio.h>

// DEFINES---------------------------------------------------------------------------//
#define   I2C_DELAY               100
#define   RTC_I2C                 I2C1
#define 	I2C_REQUEST_WRITE       0x00
#define 	I2C_REQUEST_READ        0x01
/* USER CODE END 0 */

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  LL_I2C_InitTypeDef I2C_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  /**I2C1 GPIO Configuration
  PB6   ------> I2C1_SCL
  PB7   ------> I2C1_SDA
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */

  /** I2C Initialization
  */
  LL_I2C_EnableClockStretching(I2C1);
  LL_I2C_SetOwnAddress2(I2C1, 0);
  I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
  I2C_InitStruct.ClockSpeed = 400000;
  I2C_InitStruct.DutyCycle = LL_I2C_DUTYCYCLE_2;
  I2C_InitStruct.OwnAddress1 = 0;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C1, &I2C_InitStruct);
  LL_I2C_DisableOwnAddress2(I2C1);
  LL_I2C_DisableGeneralCall(I2C1);
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/* USER CODE BEGIN 1 */
//------------------------------------------------------------------------------//
static uint8_t I2C_WriteAdress (uint16_t reg_addr, uint32_t delay)
{
  uint8_t I2C_status = I2C_OK;
  static uint64_t timeout_delay = 0;
	if ((I2C_status = LL_I2C_IsActiveFlag_BUSY(RTC_I2C)) != I2C_OK) 
  { return I2C_status; }

  LL_I2C_DisableBitPOS(RTC_I2C); //Disable Pos
  LL_I2C_AcknowledgeNextData(RTC_I2C, LL_I2C_ACK); //Prepare the generation of a ACKnowledge condition after the address receive match code or next received byte
  LL_I2C_GenerateStartCondition(RTC_I2C); //Generate a START condition
  
  timeout_delay = delay + Get_SysTick(); //инициализация переменной таймаута
	while(!LL_I2C_IsActiveFlag_SB(RTC_I2C))  //Indicate the status of Start Bit (master mode)
  {
    if (systick_delay (timeout_delay) == 1) //выход из ожидания в случае окончания таймаута
    {  return (I2C_status = I2C_TIMEOUT);  }
  }
  (void) RTC_I2C->SR1;   //считывание регистра SR1 для сброса флагов
		
  LL_I2C_TransmitData8(RTC_I2C, RTC_ADDRESS | I2C_REQUEST_WRITE); //передача адреса микросхемы и бита write 
  while(!LL_I2C_IsActiveFlag_ADDR(RTC_I2C))
  {
    if (systick_delay (timeout_delay) == 1) //выход из ожидания в случае окнчаеия таймаута
    {  return (I2C_status = I2C_TIMEOUT);  }
  }
  LL_I2C_ClearFlag_ADDR(RTC_I2C);
  (void) RTC_I2C->SR2;   //считывание регистра SR2 для сброса флагов

  LL_I2C_TransmitData8(RTC_I2C, (uint8_t) reg_addr); //адреса регистра 
	while(!LL_I2C_IsActiveFlag_TXE(RTC_I2C))
  {
    if (systick_delay (timeout_delay) == 1) //выход из ожидания в случае окнчаеия таймаута
    {  return (I2C_status = I2C_TIMEOUT);  }
  }
	return I2C_OK;
}

//------------------------------------------------------------------------------//
uint8_t I2C_WriteBuffer (uint16_t reg_addr, uint8_t *buf, uint16_t bytes_count)
{
  uint8_t I2C_status = I2C_OK;
  static uint64_t timeout_delay = 0;
	if ((I2C_status = I2C_WriteAdress (reg_addr, I2C_DELAY)) != I2C_OK)
	{	return I2C_status;	}

  timeout_delay = I2C_DELAY + Get_SysTick(); //инициализация переменной таймаута
  for(uint16_t i=0; i<bytes_count; i++)
  {
    LL_I2C_TransmitData8(RTC_I2C, buf[i]);
    while(!LL_I2C_IsActiveFlag_TXE(RTC_I2C))
    {
      if (systick_delay (timeout_delay) == 1) //выход из ожидания в случае окончания таймаута
      {  return (I2C_status = I2C_TIMEOUT);  }
    }
  }
  LL_I2C_GenerateStopCondition(RTC_I2C);

	return I2C_status;
}

//------------------------------------------------------------------------------//
uint8_t I2C_WriteByte (uint8_t byte, uint16_t reg_addr)
{
  uint8_t I2C_status = I2C_OK;
  static uint64_t timeout_delay = 0;
  if ((I2C_status = I2C_WriteAdress (reg_addr, I2C_DELAY)) != I2C_OK)
	{	return I2C_status;	}

  timeout_delay = I2C_DELAY + Get_SysTick(); //инициализация переменной таймаута
  LL_I2C_TransmitData8(RTC_I2C, byte);
  while(!LL_I2C_IsActiveFlag_TXE(RTC_I2C))
  {
    if (systick_delay (timeout_delay) == 1) //выход из ожидания в случае окончания таймаута
    {  return (I2C_status = I2C_TIMEOUT);  }
  };
  LL_I2C_GenerateStopCondition(RTC_I2C);
		
	return I2C_status;
}

//------------------------------------------------------------------------------//
uint8_t I2C_ReadBuffer (uint16_t reg_addr, uint8_t *buf, uint16_t bytes_count)
{
  uint8_t I2C_status = I2C_OK;
  static uint64_t timeout_delay = 0;
  if ((I2C_status = I2C_WriteAdress (reg_addr, I2C_DELAY)) != I2C_OK)
	{	return I2C_status;	}
	
  LL_I2C_GenerateStartCondition(RTC_I2C); //условие Start
  timeout_delay = I2C_DELAY + Get_SysTick(); //инициализация переменной таймаута
  while(!LL_I2C_IsActiveFlag_SB(RTC_I2C))
  {
    if (systick_delay (timeout_delay) == 1) //выход из ожидания в случае окончания таймаута
    {  return (I2C_status = I2C_TIMEOUT);  }
  }
  (void) RTC_I2C->SR1;

  LL_I2C_TransmitData8(RTC_I2C, RTC_ADDRESS | I2C_REQUEST_READ); //передача адреса микросхемы и бита read 
  while (!LL_I2C_IsActiveFlag_ADDR(RTC_I2C))
  {
    if (systick_delay (timeout_delay) == 1) //выход из ожидания в случае окончания таймаута
    {  return (I2C_status = I2C_TIMEOUT);  }
  }
  LL_I2C_ClearFlag_ADDR(RTC_I2C);
		
  for(uint16_t i=0; i<bytes_count; i++)
  {
    if(i<(bytes_count-1))
    {
      while(!LL_I2C_IsActiveFlag_RXNE(RTC_I2C))
      {
        if (systick_delay (timeout_delay) == 1) //выход из ожидания в случае окончания таймаута
        { return (I2C_status = I2C_TIMEOUT);  }
      }
      buf[i] = LL_I2C_ReceiveData8(RTC_I2C);
    }
    else //если последний байт буффера
    {
      LL_I2C_AcknowledgeNextData(RTC_I2C, LL_I2C_NACK);
      LL_I2C_GenerateStopCondition(RTC_I2C); //условие Stop после получения текущего байта
      while(!LL_I2C_IsActiveFlag_RXNE(RTC_I2C))
      {
        if (systick_delay (timeout_delay) == 1) //выход из ожидания в случае окончания таймаута
        { return (I2C_status = I2C_TIMEOUT);  }
      };
      buf[i] = LL_I2C_ReceiveData8(RTC_I2C);
    }
  }	
	return I2C_status;
}

//------------------------------------------------------------------------------//
uint8_t I2C_ReadByte (uint16_t reg_addr, uint8_t * byte)
{
	uint8_t I2C_status = I2C_OK;
  static uint64_t timeout_delay = 0;
  if ((I2C_status = I2C_WriteAdress (reg_addr, I2C_DELAY)) != I2C_OK)
	{	return I2C_status;	}
	
  LL_I2C_GenerateStartCondition(RTC_I2C); //условие Start
  while(!LL_I2C_IsActiveFlag_SB(RTC_I2C))
  {
    if (systick_delay (timeout_delay) == 1) //выход из ожидания в случае окончания таймаута
    { return (I2C_status = I2C_TIMEOUT);  }
  }
  (void) RTC_I2C->SR1;
  LL_I2C_TransmitData8(RTC_I2C, RTC_ADDRESS | I2C_REQUEST_READ); //передача адреса микросхемы и бита read 
  while (!LL_I2C_IsActiveFlag_ADDR(RTC_I2C))
  {
    if (systick_delay (timeout_delay) == 1) //выход из ожидания в случае окончания таймаута
    { return (I2C_status = I2C_TIMEOUT);  }
  }
	LL_I2C_ClearFlag_ADDR(RTC_I2C);
		
	LL_I2C_AcknowledgeNextData(RTC_I2C, LL_I2C_NACK);
	LL_I2C_GenerateStopCondition(RTC_I2C); //условие Stop после получения текущего байта
	while(!LL_I2C_IsActiveFlag_RXNE(RTC_I2C))
  {
    if (systick_delay (timeout_delay) == 1) //выход из ожидания в случае окончания таймаута
    { return (I2C_status = I2C_TIMEOUT);  }
  }
	*byte = LL_I2C_ReceiveData8(RTC_I2C);	
		
	return I2C_status;
}
/* USER CODE END 1 */
