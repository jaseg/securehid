/**
  ******************************************************************************
  * @file    USB_Host/HID_Standalone/Src/menu.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    17-February-2017
  * @brief   This file implements Menu Functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

HID_DEMO_StateMachine hid_demo;

static void USBH_KeybdDemo(USBH_HandleTypeDef* phost);

void HID_MenuInit(void)
{
    uart_print("Starting HID Demo");
    hid_demo.state = HID_DEMO_START;
    HID_MenuProcess();
}

void HID_MenuProcess(void)
{
    switch (hid_demo.state) {
    case HID_DEMO_START:
        if (Appli_state == APPLICATION_READY) {
            if (USBH_HID_GetDeviceType(&hUSBHost) == HID_KEYBOARD) {
                hid_demo.keyboard_state = HID_KEYBOARD_IDLE;
                hid_demo.state = HID_DEMO_KEYBOARD;

                uart_print("HID initialized");

                HID_KeyboardMenuProcess();
            } else {
                uart_print("Unsupported HID device!\n");
                hid_demo.state = HID_DEMO_START;
            }
        }
        break;

    case HID_DEMO_KEYBOARD:
        if (Appli_state == APPLICATION_READY) {
            USBH_KeybdDemo(&hUSBHost);
        }
        break;

    default:
        break;
    }

    if (Appli_state == APPLICATION_DISCONNECT) {
        Appli_state = APPLICATION_IDLE;
        uart_print("HID device disconnected\n");
        hid_demo.state = HID_DEMO_START;
        hid_demo.select = 0;
    }
}

static void USBH_KeybdDemo(USBH_HandleTypeDef* phost)
{
    HID_KEYBD_Info_TypeDef* k_pinfo;
    char c;
    k_pinfo = USBH_HID_GetKeybdInfo(phost);

    if (k_pinfo != NULL) {
        c = USBH_HID_GetASCIICode(k_pinfo);
        if (c != 0) {
            USR_KEYBRD_ProcessData(c);
        }
    }
}
