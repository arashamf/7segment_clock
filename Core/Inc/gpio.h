/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
//---------------------------------------------------------------------------------//
#define Segment_A(x) ((x)? (LL_GPIO_SetOutputPin (SA_GPIO_Port, SA_Pin)) : (LL_GPIO_ResetOutputPin(SA_GPIO_Port, SA_Pin)))
#define Segment_B(x) ((x)? (LL_GPIO_SetOutputPin (SB_GPIO_Port, SB_Pin)) : (LL_GPIO_ResetOutputPin(SB_GPIO_Port, SB_Pin)))
#define Segment_C(x) ((x)? (LL_GPIO_SetOutputPin (SC_GPIO_Port, SC_Pin)) : (LL_GPIO_ResetOutputPin(SC_GPIO_Port, SC_Pin)))
#define Segment_D(x) ((x)? (LL_GPIO_SetOutputPin (SD_GPIO_Port, SD_Pin)) : (LL_GPIO_ResetOutputPin(SD_GPIO_Port, SD_Pin)))
#define Segment_E(x) ((x)? (LL_GPIO_SetOutputPin (SE_GPIO_Port, SE_Pin)) : (LL_GPIO_ResetOutputPin(SE_GPIO_Port, SE_Pin)))
#define Segment_F(x) ((x)? (LL_GPIO_SetOutputPin (SF_GPIO_Port, SF_Pin)) : (LL_GPIO_ResetOutputPin(SF_GPIO_Port, SF_Pin)))
#define Segment_G(x) ((x)? (LL_GPIO_SetOutputPin (SG_GPIO_Port, SG_Pin)) : (LL_GPIO_ResetOutputPin(SG_GPIO_Port, SG_Pin)))
#define Segment_H(x) ((x)? (LL_GPIO_SetOutputPin (SH_GPIO_Port, SH_Pin)) : (LL_GPIO_ResetOutputPin(SG_GPIO_Port, SH_Pin)))
#define Segment_J(x) ((x)? (LL_GPIO_SetOutputPin (SJ_GPIO_Port, SJ_Pin)) : (LL_GPIO_ResetOutputPin(SJ_GPIO_Port, SJ_Pin)))
#define Segment_DP(x) ((x)? (LL_GPIO_SetOutputPin (SDP_GPIO_Port, SDP_Pin)) : (LL_GPIO_ResetOutputPin(SDP_GPIO_Port, SDP_Pin)))

#define NUMBER_0(x) ((x)? (LL_GPIO_SetOutputPin (A0_GPIO_Port, A0_Pin)) : (LL_GPIO_ResetOutputPin (A0_GPIO_Port, A0_Pin)))
#define NUMBER_1(x) ((x)? (LL_GPIO_SetOutputPin (A1_GPIO_Port, A1_Pin)) : (LL_GPIO_ResetOutputPin (A1_GPIO_Port, A1_Pin)))
#define NUMBER_2(x) ((x)? (LL_GPIO_SetOutputPin (A2_GPIO_Port, A2_Pin)) : (LL_GPIO_ResetOutputPin (A2_GPIO_Port, A2_Pin)))
#define NUMBER_3(x) ((x)? (LL_GPIO_SetOutputPin (A3_GPIO_Port, A3_Pin)) : (LL_GPIO_ResetOutputPin (A3_GPIO_Port, A3_Pin)))
#define NUMBER_4(x) ((x)? (LL_GPIO_SetOutputPin (A4_GPIO_Port, A4_Pin)) : (LL_GPIO_ResetOutputPin (A4_GPIO_Port, A4_Pin)))
#define NUMBER_5(x) ((x)? (LL_GPIO_SetOutputPin (A5_GPIO_Port, A5_Pin)) : (LL_GPIO_ResetOutputPin (A5_GPIO_Port, A5_Pin)))

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

