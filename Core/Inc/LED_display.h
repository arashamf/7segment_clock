#ifndef __LED_DISPLAY_H__
#define __LED_DISPLAY_H__

#ifdef __cplusplus
extern "C" {
#endif
//Private includes ----------------------------------------------------------------//
#include "main.h"

//Prototypes-----------------------------------------------------------------------//
void reset_number (uint8_t );
void reset_all_number (void);
void select_number (uint8_t );
void reset_all_digit (void);
void digit_0 (void);
void digit_1 (void);
void digit_2 (void);
void digit_3 (void);
void digit_4 (void);
void digit_5 (void);
void digit_6 (void);
void digit_7 (void);
void digit_8 (void);
void digit_9 (void);
void select_digit (uint8_t );

#ifdef __cplusplus
}
#endif

#endif /* LED_DISPLAY_H */

