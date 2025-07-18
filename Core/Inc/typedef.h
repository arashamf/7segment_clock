
//Includes -----------------------------------------------------------------------//
#include "main.h"
#include "ring_buffer.h"

//Define --------------------------------------------------------------------------//
#define     FIRST_REGISTR_TIME 	0x00
#define     FIRST_REGISTR_DATE 	0x04
#define     RTC_REGISTR_TEMP 	  0x11	
#define     RTC_ADDRESS 		    0xD0 //i2c-адрес мк RTC со сдвигом влево на один разряд 
//#define     NULL                0x00 

#define     BUFFER_SIZE         50 //размер буффера UART
#define     PDT_SIZE_MSG        8
#define     SIZE_CRC            2

//typedef--------------------------------------------------------------------------//
enum I2C_status
{
    I2C_OK          = 0,
    I2C_ERROR          ,
    I2C_TIMEOUT        ,
};

//--------------------------------------------------------------------------------//
typedef struct 
{
  char buffer_RX_UART [BUFFER_SIZE];
	uint8_t head_count; //счётчик полученных байтов
	uint8_t tail_count; //счётчик переданных байтов
} UART_Rx_data;

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
    uint8_t raw_bytes[18]; //массив 
  } PDT_struct;

  uint8_t msg_CRC;

  RING_buffer_t * ptr_RINGbuff; //структура с кольцевым буффером

} UART_msg_t;
