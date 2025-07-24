#ifndef __RTC_H__
#define __RTC_H__

#ifdef __cplusplus
extern "C" {
#endif

//Includes ------------------------------------------------------------------------//
#include "main.h"	

//Define --------------------------------------------------------------------------//

//Prototypes ---------------------------------------------------------------------//
uint8_t GetTime (uint8_t , uint8_t , uint8_t , uint8_t * );
uint8_t GetTemp (uint8_t  , uint8_t ,  uint8_t * );
uint8_t SetTime (uint8_t , uint8_t , uint8_t , uint8_t *);
void edit_RTC_data (uint8_t , char * );
uint8_t read_reg_RTC (uint8_t , uint8_t );
uint8_t get_file_title (void);	
void convert_time (char * , unsigned char * , unsigned char );
void CalcUNIXtime_from_DS3231 (uint32_t *, uint8_t * );
void CalcTimeStamp_from_UART (uint32_t );
void copy_TMdata(uint8_t *);

#ifdef __cplusplus
}
#endif

#endif /* RTC_H */
