/**
  ******************************************************************************
  * @file    USB_Host/HID_Standalone/Inc/stm32f4xx_hal_conf.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    17-February-2017
  * @brief   HAL configuration file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without
  *modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright
  *notice,
  *      this list of conditions and the following disclaimer in the
  *documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its
  *contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  *ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  *LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
  *USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

#ifndef __STM32F4xx_HAL_CONF_H
#define __STM32F4xx_HAL_CONF_H

#define HAL_MODULE_ENABLED

#define HAL_DMA_MODULE_ENABLED
#include "stm32f4xx_hal_dma.h"

#define HAL_DMA2D_MODULE_ENABLED
#include "stm32f4xx_hal_dma2d.h"

#define HAL_FLASH_MODULE_ENABLED
#include "stm32f4xx_hal_flash.h"

#define HAL_SRAM_MODULE_ENABLED
#include "stm32f4xx_hal_sram.h"

#define HAL_SDRAM_MODULE_ENABLED
#include "stm32f4xx_hal_sdram.h"

#define HAL_GPIO_MODULE_ENABLED
#include "stm32f4xx_hal_gpio.h"

#define HAL_I2C_MODULE_ENABLED
#include "stm32f4xx_hal_i2c.h"

#define HAL_LTDC_MODULE_ENABLED
#include "stm32f4xx_hal_ltdc.h"

#define HAL_DSI_MODULE_ENABLED
#include "stm32f4xx_hal_dsi.h"

#define HAL_PWR_MODULE_ENABLED
#include "stm32f4xx_hal_pwr.h"

#define HAL_RCC_MODULE_ENABLED
#include "stm32f4xx_hal_rcc.h"

#define HAL_UART_MODULE_ENABLED
#include "stm32f4xx_hal_uart.h"

#define HAL_CORTEX_MODULE_ENABLED
#include "stm32f4xx_hal_cortex.h"

#define HAL_HCD_MODULE_ENABLED
#include "stm32f4xx_hal_hcd.h"

#define HAL_FMPI2C_MODULE_ENABLED
#include "stm32f4xx_hal_fmpi2c.h"

#if !defined(HSE_VALUE)
#if defined(USE_STM32469I_DISCO_REVA)
#define HSE_VALUE \
    25000000U /*!< Default value of the External oscillator in Hz */
#else
#define HSE_VALUE \
    8000000U /*!< Default value of the External oscillator in Hz */
#endif /* USE_STM32469I_DISCO_REVA */
#endif /* HSE_VALUE */

#if !defined(HSE_STARTUP_TIMEOUT)
#define HSE_STARTUP_TIMEOUT 100U /*!< Time out for HSE start up, in ms */
#endif

/* Internal High Speed oscillator (HSI) value. This value is used by the RCC HAL module to compute the system frequency
 * (when HSI is used as system clock source, directly or through the PLL). */
#if !defined(HSI_VALUE)
#define HSI_VALUE 16000000U
#endif

/* Internal Low Speed oscillator (LSI) value. */
#if !defined(LSI_VALUE)
#define LSI_VALUE 32000U
#endif

/* External Low Speed oscillator (LSE) value.  */
#if !defined(LSE_VALUE)
#define LSE_VALUE 32768U
#endif

#if !defined(LSE_STARTUP_TIMEOUT)
#define LSE_STARTUP_TIMEOUT 5000U
#endif

/* External clock source for I2S peripheral This value is used by the I2S HAL module to compute the I2S clock source
 * frequency, this source is inserted directly through I2S_CKIN pad. */
#if !defined(EXTERNAL_CLOCK_VALUE)
#define EXTERNAL_CLOCK_VALUE 12288000U
#endif

/* For the HAL */
#define VDD_VALUE 3300U
#define TICK_INT_PRIORITY 0x00U
#define USE_RTOS 0
#define PREFETCH_ENABLE 1
#define INSTRUCTION_CACHE_ENABLE 1
#define DATA_CACHE_ENABLE 1

#define USE_SPI_CRC 1U

#define assert_param(expr) ((void)0U)

#endif
