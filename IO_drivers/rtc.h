#ifndef __RTC_H__
#define __RTC_H__

#ifdef __cplusplus
extern "C" {
#endif

//Includes ------------------------------------------------------------------------//
#include "main.h"	

//Define --------------------------------------------------------------------------//

//typedef--------------------------------------------------------------------------//


//Prototypes ---------------------------------------------------------------------//
uint8_t GetTempDS3231 (uint8_t  , uint8_t ,  uint8_t * );
uint32_t GetUnixTime_from_DS3231 (void);
uint8_t PrepareSendTimedata_to_DS3231 (uint32_t );
char * return_HandleTimeBuff (void);

#ifdef __cplusplus
}
#endif

#endif /* RTC_H */
