// Includes----------------------------------------------------------------------------------------//
#include "rtc.h"
#include "i2c.h"
#include "typedef.h"
#include "time64.h"

#include "usart.h"
#include <stdio.h>
// DEFINES-----------------------------------------------------------------------------------------//

// TYPEDEFS----------------------------------------------------------------------------------------//

//STATIC PROTOTYPES--------------------------------------------------------------------------------//
#ifdef __cplusplus
extern "C" {
#endif
static uint8_t RTC_ConvertToDec(uint8_t );
static uint8_t RTC_ConvertToBinDec(uint8_t );
#ifdef __cplusplus
}
#endif

// STATIC VARIABLES--------------------------------------------------------------------------------//
I2C_TypeDef * RTC_I2C = I2C1;
struct TM TimeStamp;

//массив с количеством дней в предыдущем месяце, [0][12] - не високосный, [1][12] - високосный 
const unsigned int pred_sum [2] [12] =	
{ 
	{0,31,59,90,120,151,181,212,243,273,304,334}, 
	{0,31,60,91,121,152,182,213,244,274,305,335} 
};

//FUNCTIONS----------------------------------------------------------------------------------------//
uint8_t GetTime (uint8_t RTC_adress, uint8_t registr_adress, uint8_t sizebuf, uint8_t * RTC_buffer)
{
	uint8_t I2C_status = I2C_OK;

	if ((I2C_status = I2C_ReadBuffer (registr_adress,  (uint8_t*)RTC_buffer, (uint16_t) sizebuf)) == I2C_OK)
	{
		for (uint8_t count = 0; count < sizebuf; count++)
		{
			*(RTC_buffer+count) = RTC_ConvertToDec(*(RTC_buffer+count)); //перевод числа из двоично-десятичного представления  в обычное
		}
	}
	else
	{
		for (uint8_t count = 0; count < sizebuf; count++)
		{	*(RTC_buffer+count) = 0; }
	}
	return  I2C_status;
}

//-------------------------------------------------------------------------------------------------//
uint8_t GetTemp (uint8_t  RTC_adress, uint8_t registr_adress,  uint8_t * temp_buffer)
{
	uint8_t I2C_status = I2C_OK;
	if ((I2C_status = I2C_ReadByte (registr_adress,  temp_buffer)) != I2C_OK)
	{	*temp_buffer = 0;	}	
	return  I2C_status;
}

//-------------------------------------------------------------------------------------------------//
uint8_t SetTime (uint8_t RTC_adress, uint8_t registr_adr, uint8_t sizebuf, uint8_t *RTC_buffer)
{		
	uint8_t count = 0;
	//uint8_t buf[3];

	for (count = 0; count < sizebuf; count++) //формирование сообщения для RTC
	{
		//buf[count] = RTC_ConvertToBinDec(*(RTC_buffer+count)); //перевод числа в двоично-десятичное представление из  обычного бинарного
		*(RTC_buffer+count)  = RTC_ConvertToBinDec(*(RTC_buffer+count));
	}

	#ifdef __USE_DBG
	//snprintf (DBG_buffer,  BUFFER_SIZE, "dd, sec=%u, min=%u, h=%u\r\n", *RTC_buffer,*(RTC_buffer+1), *(RTC_buffer+2));		          
    //DBG_PutString(DBG_buffer);
	#endif	


	for (count = 0; count < sizebuf; count++)
	{	
		//I2C_WriteBuffer ((uint16_t)registr_adr, buf, sizebuf);	 
		I2C_WriteBuffer ((uint16_t)registr_adr, RTC_buffer, sizebuf);
	}	
	
	return  0;
}

//---------------ф-я перевода числа из двоично-десятичного представления  в обычное---------------//
static uint8_t RTC_ConvertToDec(uint8_t digit)
{
	uint8_t ch = ((digit>>4)*10 + (0x0F & digit));
	return ch;
}

//---------------ф-я перевода числа из обычного в двоично-десятичного представления---------------//
static uint8_t RTC_ConvertToBinDec(uint8_t digit)
{
	uint8_t ch = ((digit/10) << 4) + (digit%10);
	return ch;
}


//------------------------------------------------------------------------------------------------//
void edit_RTC_data (uint8_t adress,  char * time)
{	

}

//------------------------------------------------------------------------------------------------//
uint8_t get_file_title (void)
{
	uint8_t Date [3]; 
	uint8_t file_title = 0;
	
	GetTime (RTC_ADDRESS,  FIRST_REGISTR_DATE, 3, Date); //чтение регистров 0х4-0х6 (дата: дд/мм/гг)
	file_title = ((pred_sum [0][*(Date+1)-1] )+ *Date); //извлечение не високосного года
	
	return file_title;
}

//-------------------------перевод данных времени из числовой в символьной-------------------------//
void convert_time (char * mod_time_data, uint8_t * time_data, uint8_t time_size)
{
		
	for (int8_t count = (time_size-1); count >= 0; count--)
	{
		*mod_time_data++ = (*(time_data + count) / 10) + 0x30; //часы/минуты/секунды
		*mod_time_data++ = (*(time_data + count) % 10) + 0x30;
	}
}

//-----------------------------------------------------------------------------------------------//
void CalcUNIXtime_from_DS3231 (uint32_t * UNIX_time, uint8_t * time_data)
{
	TimeStamp.tm_sec = *time_data;
	TimeStamp.tm_min = *(time_data+1);
	TimeStamp.tm_hour = *(time_data+2);
	TimeStamp.tm_mday = *(time_data+3)-1;
	TimeStamp.tm_mon = *(time_data+4);			
	TimeStamp.tm_year = *(time_data+5) + (2000 - 1900);
	*UNIX_time =  (uint32_t	) mktime64(&TimeStamp); //перевод полученной даты в формат UNIX (01.01.1970)
	#ifdef __USE_DBG
		//snprintf (DBG_buffer,  BUFFER_SIZE, "daytoTimeStamp=%u, mon=%u\r\n", TimeStamp.tm_mday, TimeStamp.tm_mon );		          
        //DBG_PutString(DBG_buffer);
	#endif
}

//-----------------------------------------------------------------------------------------------//
void CalcTimeStamp_from_UART (uint32_t UNIX_time)
{
	int64_t buf = (uint64_t)UNIX_time;
	TimeStamp = *gmtime64_r (&buf, &TimeStamp);

}

//-----------------------------------------------------------------------------------------------//
void copy_TMdata(uint8_t *RTC_buffer)
{
	*RTC_buffer 		=  TimeStamp.tm_sec;
	*(RTC_buffer+1) 	=  TimeStamp.tm_min;
	*(RTC_buffer+2) 	=  TimeStamp.tm_hour;
	*(RTC_buffer+3) 	=  (TimeStamp.tm_mday);
	*(RTC_buffer+4) 	=  (TimeStamp.tm_mon) + 1; //в структуре TimeStamp месяца от 0 до 11; в DS3231 - от 1 до 12
	*(RTC_buffer+5) 	=  TimeStamp.tm_year - (2000 - 1900);

	#ifdef __USE_DBG
		//snprintf (DBG_buffer,  BUFFER_SIZE, "daytoDS3231=%u, mon=%u\r\n",*(RTC_buffer+3), *(RTC_buffer+4) );		          
       // DBG_PutString(DBG_buffer);
	#endif
}
