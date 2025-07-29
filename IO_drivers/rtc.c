#ifdef __cplusplus
extern "C" {
#endif

// Includes---------------------------------------------------------------------------------//
#include "rtc.h"
#include "i2c.h"
#include "typedef.h"
#include "time64.h"
#include "usart.h"
#include <stdio.h>

//Defines------------------------------------------------------------------------------------//

//Typedefs-----------------------------------------------------------------------------------//

//Prototypes---------------------------------------------------------------------------------//
static uint8_t GetDataDS3231 (uint8_t , uint8_t , uint8_t , uint8_t * );
static uint8_t SetTimeDS3231 (uint8_t , uint8_t , uint8_t , uint8_t *);
static uint8_t RTC_ConvertToDec(uint8_t );
static uint8_t RTC_ConvertToBinDec(uint8_t );
static uint8_t GetTime_DS3231 (void);
static void convert_time (char * , unsigned char * , unsigned char );
static void CalcUNIXtime_from_DS3231 (uint32_t * UNIX_time, uint8_t * time_data);
static void CalcTimeStamp_from_UART (uint32_t );
static void copy_TMdata(uint8_t *);


//variables----------------------------------------------------------------------------------//
struct TM TimeStamp;
rtc_data data = {0}; //структура с настройками времени
I2C_status_t status = I2C_OK;

//массив с количеством дней в предыдущем месяце, [0][12] - не високосный, [1][12] - високосный 
const unsigned int pred_sum [2][12] =	
{ 
	{0,31,59,90,120,151,181,212,243,273,304,334}, 
	{0,31,60,91,121,152,182,213,244,274,305,335} 
};

//FUNCTIONS----------------------------------------------------------------------------------//
static uint8_t GetDataDS3231 (uint8_t RTC_adress, uint8_t registr_adress, uint8_t sizebuf, uint8_t * RTC_buffer)
{
	status = I2C_OK;

	if ((status = I2C_ReadBuffer (registr_adress,  (uint8_t*)RTC_buffer, (uint16_t) sizebuf)) == I2C_OK)
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
	return  status;
}

//--------------------------------------------------------------------------------------------//
uint8_t GetTempDS3231 (uint8_t  RTC_adress, uint8_t registr_adress,  uint8_t * temp_buffer)
{
	status = I2C_OK;
	if ((status = I2C_ReadByte (registr_adress,  temp_buffer)) != I2C_OK)
	{	*temp_buffer = 0;	}	
	return  status;
}

//-------------------------------------------------------------------------------------------//
static uint8_t SetTimeDS3231 (uint8_t RTC_adress, uint8_t registr_adr, uint8_t sizebuf, uint8_t *RTC_buffer)
{		
	uint8_t count = 0;
	status = I2C_OK;

	for (count = 0; count < sizebuf; count++) {     //формирование сообщения для RTC
		*(RTC_buffer+count)  = RTC_ConvertToBinDec(*(RTC_buffer+count));  //перевод числа в двоично-десятичное представление из  обычного бинарного
	}

	for (count = 0; count < sizebuf; count++)
	{	 
		status = I2C_WriteBuffer ((uint16_t)registr_adr, RTC_buffer, sizebuf);
	}	
	
	return  status;
}

//-------------ф-я перевода числа из двоично-десятичного представления  в обычное-------------//
static uint8_t RTC_ConvertToDec(uint8_t digit)
{
	uint8_t ch = ((digit>>4)*10 + (0x0F & digit));
	return ch;
}

//-------------ф-я перевода числа из обычного в двоично-десятичного представления-------------//
static uint8_t RTC_ConvertToBinDec(uint8_t digit)
{
	uint8_t ch = ((digit/10) << 4) + (digit%10);
	return ch;
}

//---------------------------------------------------------------------------------------------//
static uint8_t GetTime_DS3231 (void)
{
	status = I2C_OK;
	status = GetDataDS3231(RTC_ADDRESS, FIRST_REGISTR_TIME, TIME_NUMBER, data.DS3231_getdata); //получение данных времени сек/мин/чч
    status = GetDataDS3231(RTC_ADDRESS, FIRST_REGISTR_DATE, TIME_NUMBER, data.DS3231_getdata+3); //получение данных времени день/мес/год      
	return status;
}

//--------------------перевод данных времени из числовой формы в символьную--------------------//
static void convert_time (char * mod_time_data, uint8_t * time_data, uint8_t time_size)
{
		
	for (int8_t count = (time_size-1); count >= 0; count--)
	{
		*mod_time_data++ = (*(time_data + count) / 10) + 0x30; //часы/минуты/секунды
		*mod_time_data++ = (*(time_data + count) % 10) + 0x30;
	}
}

//---------------------------------------------------------------------------------------------//
static void CalcUNIXtime_from_DS3231 (uint32_t * UNIX_time, uint8_t * time_data)
{
	TimeStamp.tm_sec = *time_data;
	TimeStamp.tm_min = *(time_data+1);
	TimeStamp.tm_hour = *(time_data+2);
	TimeStamp.tm_mday = *(time_data+3)-1;
	TimeStamp.tm_mon = *(time_data+4);			
	TimeStamp.tm_year = *(time_data+5) + (2000 - 1900);
	*UNIX_time =  (uint32_t	) mktime64(&TimeStamp); //перевод полученной даты в формат UNIX (01.01.1970)
}

//------------------получение данных времени из DS3231 и расчёт UNIX времени------------------//
uint32_t GetUnixTime_from_DS3231 (void)
{
	status = I2C_OK;
    status = GetTime_DS3231 ();

	if (status == I2C_OK)
	{	
		convert_time (data.time, data.DS3231_getdata, NUMB_DIGITS_BOARD/2); //формирование символьного массива времени формата чч/мм/сс
		CalcUNIXtime_from_DS3231 (&data.DS3231_UNIXtime, data.DS3231_getdata);  //расчёт UNIX времени, полученного от DS3231
		return data.DS3231_UNIXtime;
	}
	else
	{ return 0;	}

}

//---------------------------------------------------------------------------------------------//
static void CalcTimeStamp_from_UART (uint32_t UART_UNIXtime)
{
	int64_t buf = (uint64_t)UART_UNIXtime;
	TimeStamp = *gmtime64_r (&buf, &TimeStamp);

}

//---------------------------------------------------------------------------------------------//
static void copy_TMdata(uint8_t *RTC_buffer)
{
	*RTC_buffer 		=  TimeStamp.tm_sec;
	*(RTC_buffer+1) 	=  TimeStamp.tm_min;
	*(RTC_buffer+2) 	=  TimeStamp.tm_hour;
	*(RTC_buffer+3) 	=  (TimeStamp.tm_mday);
	*(RTC_buffer+4) 	=  (TimeStamp.tm_mon) + 1; //в структуре TimeStamp месяца от 0 до 11; в DS3231 - от 1 до 12
	*(RTC_buffer+5) 	=  TimeStamp.tm_year - (2000 - 1900);
}

//------------перевод UNIX времени в структуру и запись полученных данных в DS3231------------//
uint8_t PrepareSendTimedata_to_DS3231 (uint32_t UART_UnixTime)
{
	status = I2C_OK;
	CalcTimeStamp_from_UART (UART_UnixTime);
	copy_TMdata(data.DS3231_putdata);
    status = SetTimeDS3231 (RTC_ADDRESS, FIRST_REGISTR_TIME, TIME_NUMBER, data.DS3231_putdata);
    status = SetTimeDS3231 (RTC_ADDRESS, FIRST_REGISTR_DATE, TIME_NUMBER, data.DS3231_putdata+3);
	return status;
}

//----------возврат указателя на символьный массив с данными времени формата чч/мм/сс----------//
char * return_HandleTimeBuff (void)
{
	return data.time; 
}

#ifdef __cplusplus
}
#endif
