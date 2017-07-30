/**
  ******************************************************************************
  * @file    USB_Host/HID_Standalone/Src/stm32f4xx_it.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    17-February-2017
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

#include "stm32f4xx_it.h"

extern HCD_HandleTypeDef hhcd;

void NMI_Handler(void) {}

void HardFault_Handler(void)
{
    while (1)
        ;
}

void MemManage_Handler(void)
{
    while (1)
        ;
}

void BusFault_Handler(void)
{
    while (1)
        ;
}

void UsageFault_Handler(void)
{
    while (1)
        ;
}

void SVC_Handler(void) {}

void DebugMon_Handler(void) {}

void PendSV_Handler(void) {}

void SysTick_Handler(void) {
    HAL_IncTick();
}

void OTG_FS_IRQHandler(void) {
    HAL_HCD_IRQHandler(&hhcd);
}
