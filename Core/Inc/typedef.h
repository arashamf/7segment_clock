
//Includes -----------------------------------------------------------------------//
#include "main.h"
#include "ring_buffer.h"

//Define --------------------------------------------------------------------------//
#define     FIRST_REGISTR_TIME 	  0x00
#define     FIRST_REGISTR_DATE 	  0x04
#define     RTC_REGISTR_TEMP 	    0x11	
#define     RTC_ADDRESS 		      0xD0 //i2c-адрес мк RTC со сдвигом влево на один разряд 
#define     TIME_NUMBER           0x03

#define     I2C_DELAY             100
#define     BUFFER_SIZE           50 //размер буффера UART
#define     PDT_SIZE_MSG          8
#define     SIZE_DS3231_BUF       6
#define     NUMB_DIGITS_BOARD     6
#define     SIZE_CRC              2

#define     NO_MSG                0
#define     GET_MSG               1
#define     ON                    1
#define     OFF                   0

//typedef--------------------------------------------------------------------------//

//---------------------------------------------------------------------------------//
typedef struct
{
    uint8_t DS3231_getdata [SIZE_DS3231_BUF]; //данные времени в бинарной форме формата сек/мин/чч/день/мес/год
    uint8_t DS3231_putdata [SIZE_DS3231_BUF];
    char time [SIZE_DS3231_BUF]; //данные времени в символьной форме
    uint32_t DS3231_UNIXtime; //время UNIX-формата от мк DS3231
} rtc_data;

//--------------------------------------------------------------------------------//
typedef struct 
{
	enum 
	{
		__START_BYTE = 1    ,
		__SYNC_WORD1        ,
    __SYNC_WORD2        ,
    __SYNC_WORD3        ,
    __SYNC_COMMA1       ,
		__ID_MSG            ,
    __SYNC_COMMA2       ,
		__USEFUL_PART_MSG   ,
		__CRC_MSG,

	} machine_state;

  union 
  {
    uint16_t raw_words[9];
    uint8_t raw_bytes[18]; //буфер приёма сообщений PDT
  } PDT_struct;

  uint8_t msg_CRC; 
  uint32_t ntp_UNIXtime; //время UNIX-формата в двоичном виде от ntp сервера
  RING_buffer_t * ptr_RINGbuff; //структура с кольцевым буффером

} UART_msg_t;
