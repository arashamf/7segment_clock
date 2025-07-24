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

// defines--------------------------------------------------------------------------//
#define 	DBG_UART 			USART1
#define   NTP_UART		  USART1

//variables-------------------------------------------------------------------------//
#ifdef __USE_DBG
  char DBG_buffer[BUFFER_SIZE];
#endif

RING_buffer_t RING_buffer; //структура с кольцевым буффером
UART_msg_t UART_msg ; //структура необходимая для парсинга полученного по UART сообщения
uint8_t uart_buffer[BUFFER_SIZE]; //массив для кольцевого буффера
uint8_t * ptr_PDTmsg = UART_msg.PDT_struct.raw_bytes;

//static prototypes-----------------------------------------------------------------//
#ifdef __cplusplus
extern "C" {
#endif
static void UART_PutByte(char );
static uint8_t calc_CRC8 (uint8_t * );
static uint32_t calc_UNIXtime (uint8_t * );
#ifdef __cplusplus
}
#endif

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
  NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
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
  RING_Init (&RING_buffer, uart_buffer, sizeof (uart_buffer)); //инициализация кольцевого буффера
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
  RING_Put(&RING_buffer, smb); //отправка байта в кольцевой буффер
}

//-------------------------------------------------------------------------------------------------//
void UART_msg_ini (void)
{
  UART_msg.machine_state = __START_BYTE;
  UART_msg.ptr_RINGbuff = &RING_buffer;
  ptr_PDTmsg[0]=  '$';
  ptr_PDTmsg[1]=  'P';
  ptr_PDTmsg[2]=  'D';
  ptr_PDTmsg[3]=  'T';
  ptr_PDTmsg[4]=  ',';
  ptr_PDTmsg[5]=  '1';
  ptr_PDTmsg[6]=  ',';
  ptr_PDTmsg[15]= '*';
}
//------------------------------проверка состояние буффера UARTа------------------------------//
uint8_t check_ring_buffer (void)
{
    uint8_t buffer_status =  NO_MSG;
    uint8_t CRC8 = 0; //переменная для хранения CRC
    static uint8_t count  = 0; //счётчик принятых байтов тела сообщения
    static uint8_t divider = 0x10; //делитель для перевода в 16чный формат
    uint8_t byte; //полученный из кольцевого буффера байт

    if (RING_GetCount(UART_msg.ptr_RINGbuff) > 0)
    {
      byte = RING_Pop(UART_msg.ptr_RINGbuff);
      switch (UART_msg.machine_state) //проверка стадии получения сообщения
		  { 
        case __START_BYTE: 
          if ( byte == ptr_PDTmsg[0]) //если полученный символ равен '$' 
					{ UART_msg.machine_state = __SYNC_WORD1;  }
				break;

        case __SYNC_WORD1: 
          if ( byte == ptr_PDTmsg[1]) //если полученный символ равен 'P' 
					{ UART_msg.machine_state = __SYNC_WORD2;  }
          else
          { UART_msg.machine_state = __START_BYTE;  }
				break;

        case __SYNC_WORD2: 
          if ( byte == ptr_PDTmsg[2]) //если полученный символ равен 'D' 
					{ UART_msg.machine_state = __SYNC_WORD3;  }
          else
          { UART_msg.machine_state = __START_BYTE;  }
				break;

        case __SYNC_WORD3: 
          if ( byte == ptr_PDTmsg[3]) //если полученный символ равен 'T' 
					{ UART_msg.machine_state = __SYNC_COMMA1;  }
          else
          { UART_msg.machine_state = __START_BYTE;  }
				break;

        case __SYNC_COMMA1: 
          if ( byte == ptr_PDTmsg[4]) //если полученный символ равен ',' 
					{ UART_msg.machine_state = __ID_MSG;  }
          else
          { UART_msg.machine_state = __START_BYTE;  }
				break;

        case __ID_MSG: 
          if ( byte == ptr_PDTmsg[5]) //если полученный символ равен '1' 
					{ UART_msg.machine_state = __SYNC_COMMA2;  }
          else
          { UART_msg.machine_state = __START_BYTE;  }
				break;

        case __SYNC_COMMA2: 
          if ( byte == ptr_PDTmsg[6]) //если полученный символ равен ',' 
					{ UART_msg.machine_state = __USEFUL_PART_MSG;  }
          else
          { UART_msg.machine_state = __START_BYTE;  }
				break;

        case  __USEFUL_PART_MSG: 
          if ( count < PDT_SIZE_MSG ) //если счётчик принятых байтов меньше полученной длины сообщения
				  {
					  ptr_PDTmsg[count+7] = byte; //сохранения принятого байта					
					  count++;
				  } 
				  else //если полезная часть сообщения получена полностью
				  {
					  count = 0; //сброс счётчика принятых символов

            if (( byte == ptr_PDTmsg[15])) //если полученный символ равен '*' 
					  { 
              UART_msg.machine_state = __CRC_MSG; 
              UART_msg.msg_CRC = 0;
              divider = 0x10;
            } 	
				  } 
				break;	

        case  __CRC_MSG: 
          if (count < SIZE_CRC)
          {
            ptr_PDTmsg[16 + count] = byte;
            if ((byte >= '0') && (byte <= '9')) //проверка диапазона принятого символа
            { UART_msg.msg_CRC |= (byte-0x30)*divider; }
            else
            {     
              if ((byte >= 'A') && (byte <= 'F'))
              { UART_msg.msg_CRC |= (byte-0x37)*divider; }
            }
            count++;
            divider = (divider >> 4); //деление на 0x10
          }
          else 
          {
            if ((CRC8 = calc_CRC8 ( &ptr_PDTmsg[1])) == UART_msg.msg_CRC)
            {
              UART_msg.ntp_UNIXtime = calc_UNIXtime (&ptr_PDTmsg[7]);          
            }
            count = 0;
            UART_msg.machine_state = __START_BYTE;
            buffer_status =  GET_MSG;
            #ifdef __USE_DBG
		          snprintf (DBG_buffer,  BUFFER_SIZE, "msg=$PDT,1,%c%c%c%c%c%c%c%c*%c%c, UT=%lu\r\n", ptr_PDTmsg[7], ptr_PDTmsg[8],
              ptr_PDTmsg[9], ptr_PDTmsg[10],ptr_PDTmsg[11], ptr_PDTmsg[12],ptr_PDTmsg[13], 
              ptr_PDTmsg[14], ptr_PDTmsg[16], ptr_PDTmsg[17], UART_msg.ntp_UNIXtime);	          
              DBG_PutString(DBG_buffer);
	          #endif
          }
          
        break;
      }
    }
  return buffer_status;
}

//----------------------------------------------------------------------------//
static uint8_t calc_CRC8 (uint8_t * ptr_msg)
{
    uint8_t CRC8 = *ptr_msg;

    while (*(++ptr_msg) != '*') 
    { CRC8 = CRC8 ^ (*ptr_msg);  }

    return CRC8;
}

//----------------------------------------------------------------------------//
static uint32_t calc_UNIXtime (uint8_t * ptr_msg)
{
  uint8_t byte = 0;
  uint32_t UNIXtime = 0;
  uint32_t divider = 1;
  for (uint8_t count = 0; count < 9; count++)
  {
    byte = *(ptr_msg + (7 - count)) ;
    if ((byte >= '0') && (byte <= '9')) //проверка диапазона принятого символа
    {
      UNIXtime |= (byte-0x30)*divider;
    }
    else
    {
      if ((byte >= 'A') && (byte <= 'F'))
      { UNIXtime |= (byte-0x37)*divider;  }
    }
    divider *= 0x10;
  }
 return UNIXtime;
}

//-----------------------------------------------------------------------------------------------//
uint32_t return_UNIXtimeNTP (void)
{
  return UART_msg.ntp_UNIXtime;
}
/* USER CODE END 1 */
