/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32l1xx_hal_conf.h
  * @brief   HAL configuration file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
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
#ifndef __STM32L1xx_HAL_CONF_H
#define __STM32L1xx_HAL_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* ########################## Module Selection ############################## */
/**
  * @brief This is the list of modules to be used in the HAL driver
  */

#define HAL_MODULE_ENABLED
/*#define HAL_ADC_MODULE_ENABLED   */
/*#define HAL_CRYP_MODULE_ENABLED   */
/*#define HAL_COMP_MODULE_ENABLED   */
/*#define HAL_CRC_MODULE_ENABLED   */
/*#define HAL_CRYP_MODULE_ENABLED   */
/*#define HAL_DAC_MODULE_ENABLED   */
/*#define HAL_I2C_MODULE_ENABLED   */
/*#define HAL_I2S_MODULE_ENABLED   */
/*#define HAL_IRDA_MODULE_ENABLED   */
/*#define HAL_IWDG_MODULE_ENABLED   */
/*#define HAL_LCD_MODULE_ENABLED   */
/*#define HAL_NOR_MODULE_ENABLED   */
/*#define HAL_OPAMP_MODULE_ENABLED   */
/*#define HAL_PCD_MODULE_ENABLED   */
/*#define HAL_RTC_MODULE_ENABLED   */
/*#define HAL_SD_MODULE_ENABLED   */
/*#define HAL_SMARTCARD_MODULE_ENABLED   */
/*#define HAL_SPI_MODULE_ENABLED   */
/*#define HAL_SRAM_MODULE_ENABLED   */
#define HAL_TIM_MODULE_ENABLED
/*#define HAL_UART_MODULE_ENABLED   */
/*#define HAL_USART_MODULE_ENABLED   */
/*#define HAL_WWDG_MODULE_ENABLED   */
#define HAL_GPIO_MODULE_ENABLED
#define HAL_EXTI_MODULE_ENABLED
#define HAL_DMA_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED

/* ########################## Oscillator Values adaptation ####################*/
/**
  * @brief Adjust the value of External High Speed oscillator (HSE) used in your application.
  *        This value is used by the RCC HAL module to compute the system frequency
  *        (when HSE is used as system clock source, directly or through the PLL).
  */
#if !defined  (HSE_VALUE)
  #define HSE_VALUE    ((uint32_t)8000000) /*!< Value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (HSE_STARTUP_TIMEOUT)
  #define HSE_STARTUP_TIMEOUT    ((uint32_t)100)   /*!< Time out for HSE start up, in ms */
#endif /* HSE_STARTUP_TIMEOUT */

/**
  * @brief Internal Multiple Speed oscillator (MSI) default value.
  *        This value is the default MSI range value after Reset.
  */
#if !defined  (MSI_VALUE)
  #define MSI_VALUE    ((uint32_t)2097000) /*!< Value of the Internal oscillator in Hz*/
#endif /* MSI_VALUE */
/**
  * @brief Internal High Speed oscillator (HSI) value.
  *        This value is used by the RCC HAL module to compute the system frequency
  *        (when HSI is used as system clock source, directly or through the PLL).
  */
#if !defined  (HSI_VALUE)
  #define HSI_VALUE    ((uint32_t)16000000) /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */

/**
  * @brief Internal Low Speed oscillator (LSI) value.
  */
#if !defined  (LSI_VALUE)
  #define LSI_VALUE    (37000U) /*!< LSI Typical Value in Hz*/
#endif /* LSI_VALUE */          /*!< Value of the Internal Low Speed oscillator in Hz
                                     The real value may vary depending on the variations
                                     in voltage and temperature.*/

/**
  * @brief External Low Speed oscillator (LSE) value.
  *        This value is used by the UART, RTC HAL module to compute the system frequency
  */
#if !defined  (LSE_VALUE)
  #define LSE_VALUE    ((uint32_t)32768) /*!< Value of the External oscillator in Hz*/
#endif /* LSE_VALUE */

#if !defined  (LSE_STARTUP_TIMEOUT)
  #define LSE_STARTUP_TIMEOUT    ((uint32_t)5000)   /*!< Time out for LSE start up, in ms */
#endif /* HSE_STARTUP_TIMEOUT */

/* Tip: To avoid modifying this file each time you need to use different HSE,
   ===  you can define the HSE value in your toolchain compiler preprocessor. */

/* ########################### System Configuration ######################### */
/**
  * @brief This is the HAL system configuration section
  */

#define  VDD_VALUE					  ((uint32_t)3300) /*!< Value of VDD in mv */
#define  TICK_INT_PRIORITY            ((uint32_t)15)    /*!< tick interrupt priority */
#define  USE_RTOS                     0
#define  PREFETCH_ENABLE              0
#define  INSTRUCTION_CACHE_ENABLE     1
#define  DATA_CACHE_ENABLE            1

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* ################## Register callback feature configuration ############### */
/**
  * @brief Set below the peripheral configuration  to "1U" to add the support
  *        of HAL callback registration/deregistration feature for the HAL
  *        driver(s). This allows user application to provide specific callback
  *        functions thanks to HAL_PPP_RegisterCallback() rather than overwriting
  *        the default weak callback functions (see each stm32l0xx_hal_ppp.h file
  *        for possible callback identifiers defined in HAL_PPP_CallbackIDTypeDef
  *        for each PPP peripheral).
  */
#define USE_HAL_ADC_REGISTER_CALLBACKS        0U
#define USE_HAL_COMP_REGISTER_CALLBACKS       0U
#define USE_HAL_DAC_REGISTER_CALLBACKS        0U
#define USE_HAL_I2C_REGISTER_CALLBACKS        0U
#define USE_HAL_I2S_REGISTER_CALLBACKS        0U
#define USE_HAL_IRDA_REGISTER_CALLBACKS       0U
#define USE_HAL_OPAMP_REGISTER_CALLBACKS      0U
#define USE_HAL_PCD_REGISTER_CALLBACKS        0U
#define USE_HAL_RTC_REGISTER_CALLBACKS        0U
#define USE_HAL_SDMMC_REGISTER_CALLBACKS      0U
#define USE_HAL_SMARTCARD_REGISTER_CALLBACKS  0U
#define USE_HAL_SPI_REGISTER_CALLBACKS        0U
#define USE_HAL_TIM_REGISTER_CALLBACKS        0U
#define USE_HAL_UART_REGISTER_CALLBACKS       0U
#define USE_HAL_USART_REGISTER_CALLBACKS      0U
#define USE_HAL_WWDG_REGISTER_CALLBACKS       0U

/* ################## SPI peripheral configuration ########################## */

/* CRC FEATURE: Use to activate CRC feature inside HAL SPI Driver
 * Activated: CRC code is present inside driver
 * Deactivated: CRC code cleaned from driver
 */

#define USE_SPI_CRC                   0U
/* Includes ------------------------------------------------------------------*/
/**
  * @brief Include module's header file
  */

#ifdef HAL_RCC_MODULE_ENABLED
 #include "stm32l1xx_hal_rcc.h"
#endif /* HAL_RCC_MODULE_ENABLED */

#ifdef HAL_GPIO_MODULE_ENABLED
 #include "stm32l1xx_hal_gpio.h"
#endif /* HAL_GPIO_MODULE_ENABLED */

#ifdef HAL_DMA_MODULE_ENABLED
  #include "stm32l1xx_hal_dma.h"
#endif /* HAL_DMA_MODULE_ENABLED */

#ifdef HAL_CORTEX_MODULE_ENABLED
 #include "stm32l1xx_hal_cortex.h"
#endif /* HAL_CORTEX_MODULE_ENABLED */

#ifdef HAL_ADC_MODULE_ENABLED
 #include "stm32l1xx_hal_adc.h"
#endif /* HAL_ADC_MODULE_ENABLED */

#ifdef HAL_COMP_MODULE_ENABLED
  #include "stm32l1xx_hal_comp.h"
#endif /* HAL_COMP_MODULE_ENABLED */

#ifdef HAL_CRC_MODULE_ENABLED
 #include "stm32l1xx_hal_crc.h"
#endif /* HAL_CRC_MODULE_ENABLED */

#ifdef HAL_CRYP_MODULE_ENABLED
 #include "stm32l1xx_hal_cryp.h"
#endif /* HAL_CRYP_MODULE_ENABLED */

#ifdef HAL_DAC_MODULE_ENABLED
 #include "stm32l1xx_hal_dac.h"
#endif /* HAL_DAC_MODULE_ENABLED */

#ifdef HAL_FLASH_MODULE_ENABLED
 #include "stm32l1xx_hal_flash.h"
#endif /* HAL_FLASH_MODULE_ENABLED */

#ifdef HAL_SRAM_MODULE_ENABLED
 #include "stm32l1xx_hal_sram.h"
#endif /* HAL_SRAM_MODULE_ENABLED */

#ifdef HAL_NOR_MODULE_ENABLED
 #include "stm32l1xx_hal_nor.h"
#endif /* HAL_NOR_MODULE_ENABLED */

#ifdef HAL_I2C_MODULE_ENABLED
 #include "stm32l1xx_hal_i2c.h"
#endif /* HAL_I2C_MODULE_ENABLED */

#ifdef HAL_I2S_MODULE_ENABLED
 #include "stm32l1xx_hal_i2s.h"
#endif /* HAL_I2S_MODULE_ENABLED */

#ifdef HAL_IWDG_MODULE_ENABLED
 #include "stm32l1xx_hal_iwdg.h"
#endif /* HAL_IWDG_MODULE_ENABLED */

#ifdef HAL_LCD_MODULE_ENABLED
 #include "stm32l1xx_hal_lcd.h"
#endif /* HAL_LCD_MODULE_ENABLED */

#ifdef HAL_OPAMP_MODULE_ENABLED
 #include "stm32l1xx_hal_opamp.h"
#endif /* HAL_OPAMP_MODULE_ENABLED */

#ifdef HAL_PWR_MODULE_ENABLED
 #include "stm32l1xx_hal_pwr.h"
#endif /* HAL_PWR_MODULE_ENABLED */

#ifdef HAL_RTC_MODULE_ENABLED
 #include "stm32l1xx_hal_rtc.h"
#endif /* HAL_RTC_MODULE_ENABLED */

#ifdef HAL_SD_MODULE_ENABLED
 #include "stm32l1xx_hal_sd.h"
#endif /* HAL_SD_MODULE_ENABLED */

#ifdef HAL_SPI_MODULE_ENABLED
 #include "stm32l1xx_hal_spi.h"
#endif /* HAL_SPI_MODULE_ENABLED */

#ifdef HAL_TIM_MODULE_ENABLED
 #include "stm32l1xx_hal_tim.h"
#endif /* HAL_TIM_MODULE_ENABLED */

#ifdef HAL_UART_MODULE_ENABLED
 #include "stm32l1xx_hal_uart.h"
#endif /* HAL_UART_MODULE_ENABLED */

#ifdef HAL_USART_MODULE_ENABLED
 #include "stm32l1xx_hal_usart.h"
#endif /* HAL_USART_MODULE_ENABLED */

#ifdef HAL_IRDA_MODULE_ENABLED
 #include "stm32l1xx_hal_irda.h"
#endif /* HAL_IRDA_MODULE_ENABLED */

#ifdef HAL_SMARTCARD_MODULE_ENABLED
 #include "stm32l1xx_hal_smartcard.h"
#endif /* HAL_SMARTCARD_MODULE_ENABLED */

#ifdef HAL_WWDG_MODULE_ENABLED
 #include "stm32l1xx_hal_wwdg.h"
#endif /* HAL_WWDG_MODULE_ENABLED */

#ifdef HAL_PCD_MODULE_ENABLED
 #include "stm32l1xx_hal_pcd.h"
#endif /* HAL_PCD_MODULE_ENABLED */

#ifdef HAL_EXTI_MODULE_ENABLED
 #include "stm32l1xx_hal_exti.h"
#endif /* HAL_EXTI_MODULE_ENABLED */

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT
/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function
  *         which reports the name of the source file and the source
  *         line number of the call that failed.
  *         If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0U)
#endif /* USE_FULL_ASSERT */

#ifdef __cplusplus
}
#endif

#endif /* __STM32L1xx_HAL_CONF_H */

