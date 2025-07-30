//Includes -------------------------------------------------------------------------//
#include "time_protocol.h"
#include "typedef.h"
#include "usart.h"
#include <stdio.h>
#include "rtc.h"

//variables-------------------------------------------------------------------------//
UART_msg_t UART_msg ; //структура необходимая для парсинга полученного по UART сообщения
uint8_t * ptr_PDTmsg = UART_msg.PDT_struct.raw_bytes; //указатель на буфер с "сырыми" байтами PDT сообщения
uint32_t TimeUART = 0; //UNIX-время полученное из PDT сообщения
RING_buffer_t RING_buffer; //структура с кольцевым буффером

//prototypes-----------------------------------------------------------------------//
static uint8_t calc_CRC8 (uint8_t * );
static uint32_t  calcUNIXtime_from_PDTmsg (uint8_t * );

//---------------------------------------------------------------------------------//
void PutCharRingBuf(char smb)
{
  RING_Put(&RING_buffer, smb); //отправка байта в кольцевой буффер
}

//---------------------------------------------------------------------------------//
void UARTbuf_init (void)
{
  UART_msg.machine_state = __START_BYTE;
  RING_Init (&RING_buffer, returnPtrUARTbuf(), BUFFER_SIZE); //инициализация кольцевого буффера
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
              UART_msg.ntp_UNIXtime =  calcUNIXtime_from_PDTmsg (&ptr_PDTmsg[7]);          
            }
            count = 0;
            UART_msg.machine_state = __START_BYTE;
            buffer_status =  GET_MSG;
            
            #ifdef __USE_DBG
		        //  snprintf (DBG_buffer,  BUFFER_SIZE, "msg=$PDT,1,%c%c%c%c%c%c%c%c*%c%c, UT=%lu\r\n", ptr_PDTmsg[7], ptr_PDTmsg[8],
            //  ptr_PDTmsg[9], ptr_PDTmsg[10],ptr_PDTmsg[11], ptr_PDTmsg[12],ptr_PDTmsg[13], 
            //  ptr_PDTmsg[14], ptr_PDTmsg[16], ptr_PDTmsg[17], UART_msg.ntp_UNIXtime);	          
           //   DBG_PutString(DBG_buffer);
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
static uint32_t calcUNIXtime_from_PDTmsg (uint8_t * ptr_msg)
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

//--------------------------возвращает время формата UNIX от ntp сервера--------------------------//
uint32_t return_UNIXtimeNTP (void)
{
  return UART_msg.ntp_UNIXtime;
}

