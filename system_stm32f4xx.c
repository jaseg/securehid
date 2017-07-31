/**
  ******************************************************************************
  * @file    system_stm32f4xx.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    17-February-2017
  * @brief   CMSIS Cortex-M4 Device Peripheral Access Layer System Source File.
  *
  *   This file provides two functions and one global variable to be called from
  *   user application:
  *      - SystemInit(): This function is called at startup just after reset and
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32f4xx.s" file.
  *
  *      - SystemCoreClock variable: Contains the core clock (HCLK), it can be
  *used
  *                                  by the user application to setup the
  *SysTick
  *                                  timer or configure other parameters.
  *
  *      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and
  *must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.
  *
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 STMicroelectronics</center></h2>
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

#include "stm32f4xx.h"

#if !defined(HSE_VALUE)
#if defined(USE_STM32469I_DISCO_REVA)
#define HSE_VALUE \
    ((uint32_t)25000000) /*!< Default value of the External oscillator in Hz */
#else
#define HSE_VALUE \
    ((uint32_t)8000000) /*!< Default value of the External oscillator in Hz */
#endif /* USE_STM32469I_DISCO_REVA */
#endif /* HSE_VALUE */

#if !defined(HSI_VALUE)
#define HSI_VALUE \
    ((uint32_t)16000000) /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */

#define VECT_TAB_OFFSET 0x00 /* This value must be a multiple of 0x200. */

/* The following variable is updated in three ways:
    1) by calling CMSIS function SystemCoreClockUpdate()
    2) by calling HAL API function HAL_RCC_GetHCLKFreq()
    3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
   Note: If you use this function to configure the system clock; then there is no need to call the 2 first functions
   listed above, since the SystemCoreClock variable is updated automatically. */
uint32_t SystemCoreClock = 16000000;
const uint8_t AHBPrescTable[16] = { 0, 0, 0, 0, 0, 0, 0, 0,
    1, 2, 3, 4, 6, 7, 8, 9 };
const uint8_t APBPrescTable[8] = { 0, 0, 0, 0, 1, 2, 3, 4 };

/* Setup the microcontroller system: Initialize the FPU setting, vector table location and External memory configuration. */
void SystemInit(void)
{
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10 and CP11 Full Access */
#endif
    /* Reset the RCC clock configuration to the default reset state  */
    RCC->CR |= (uint32_t)0x00000001; /* Set HSION bit */
    RCC->CFGR = 0x00000000; /* Reset CFGR register */
    RCC->CR &= (uint32_t)0xFEF6FFFF; /* Reset HSEON, CSSON and PLLON bits */
    RCC->PLLCFGR = 0x24003010; /* Reset PLLCFGR register */
    RCC->CR &= (uint32_t)0xFFFBFFFF; /* Reset HSEBYP bit */
    RCC->CIR = 0x00000000; /* Disable all interrupts */

    SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
}

/* Update SystemCoreClock variable according to Clock Register Values.  The SystemCoreClock variable contains the core
 * clock (HCLK), it can be used by the user application to setup the SysTick timer or configure other parameters. */
void SystemCoreClockUpdate(void)
{
    switch (RCC->CFGR & RCC_CFGR_SWS) {
    case 0x00: /* HSI */
        SystemCoreClock = HSI_VALUE;
        break;
    case 0x04: /* HSE */
        SystemCoreClock = HSE_VALUE;
        break;
    case 0x08: {/* PLL */
        /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N
           SYSCLK  = PLL_VCO / PLL_P */
        uint32_t pllvco;
        uint32_t pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;
        uint32_t pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) >> 22;
        if (pllsource != 0) /* HSE */
            pllvco = (HSE_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
        else /* HSI */
            pllvco = (HSI_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);

        uint32_t pllp = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >> 16) + 1) * 2;
        SystemCoreClock = pllvco / pllp;
        break; }
    default:
        SystemCoreClock = HSI_VALUE;
        break;
    }
    /* Compute HCLK frequency */
    SystemCoreClock >>= AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
}

