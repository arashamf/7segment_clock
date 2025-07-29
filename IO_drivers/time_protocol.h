#ifndef __TIME_PROTOCOL_H__
#define __TIME_PROTOCOL_H__

#ifdef __cplusplus
extern "C" {
#endif

//Includes ------------------------------------------------------------------------//
#include "main.h"	

//prototypes-----------------------------------------------------------------------//
void PutChar_toRingBuf(char );
void UARTbuf_init (void);
void RingBuffer_ini (void);
uint8_t check_ring_buffer (void);
uint32_t return_UNIXtimeNTP (void);

#ifdef __cplusplus
}
#endif

#endif 