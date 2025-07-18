#ifndef __RTC_H__
#define __RTC_H__

#ifdef __cplusplus
extern "C" {
#endif

//Includes ------------------------------------------------------------------------//
#include "main.h"	

//Define --------------------------------------------------------------------------//
#define TIME_NUMBER             3

//Typedefs-------------------------------------------------------------------------//
typedef struct
{
    uint8_t DS3231_data [6];
    char time [10] ;
    uint8_t digital;
} rtc_data;

//Prototypes ---------------------------------------------------------------------//
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
