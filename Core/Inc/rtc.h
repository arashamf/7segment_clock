#ifndef __RTC_H__
#define __RTC_H__

#ifdef __cplusplus
extern "C" {
#endif

//Private includes ----------------------------------------------------------------//
#include "main.h"	

//Private define ------------------------------------------------------------------//
#define FIRST_REGISTR_TIME 		0x00
#define FIRST_REGISTR_DATE 		0x04
#define RTC_REGISTR_TEMP 		0x11	
#define RTC_ADDRESS 			0xD0 //i2c-адрес мк RTC со сдвигом влево на один разряд 
#define TIME_NUMBER             3

//--------------------------------------------------------------------------------//
typedef struct
{
    uint8_t DS3231_data [6];
    char time [10] ;
    uint8_t digital;
} rtc_data;

//Private prototypes ---------------------------------------------------------------//
uint8_t GetTime (uint8_t , uint8_t , uint8_t , uint8_t * );
uint8_t GetTemp (uint8_t  , uint8_t ,  uint8_t * );
uint8_t SetTime (uint8_t , uint8_t , char *);
void edit_RTC_data (uint8_t , char * );
uint8_t read_reg_RTC (uint8_t , uint8_t );
uint8_t get_file_title (void);	
void convert_time (char * , unsigned char * , unsigned char );

#ifdef __cplusplus
}
#endif

#endif /* RTC_H */
