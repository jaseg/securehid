/**
  ******************************************************************************
  * @file    USB_Host/HID_Standalone/Inc/main.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    17-February-2017
  * @brief   Header for main.c module
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
#ifndef __MAIN_H
#define __MAIN_H

#include "usbh_core.h"
#include "usbh_hid.h"
#include "usbh_hid_parser.h"

#include "stdio.h"

typedef enum {
    HID_DEMO_START = 0,
    HID_DEMO_KEYBOARD,
} HID_Demo_State;

typedef enum {
    HID_KEYBOARD_IDLE = 0,
    HID_KEYBOARD_WAIT,
    HID_KEYBOARD_START,
} HID_keyboard_State;

typedef struct _DemoStateMachine {
    __IO HID_Demo_State state;
    __IO HID_keyboard_State keyboard_state;
    __IO uint8_t select;
    __IO uint8_t lock;
} HID_DEMO_StateMachine;

typedef enum {
    APPLICATION_IDLE = 0,
    APPLICATION_DISCONNECT,
    APPLICATION_START,
    APPLICATION_READY,
    APPLICATION_RUNNING,
} HID_ApplicationTypeDef;

extern USBH_HandleTypeDef hUSBHost;
extern HID_ApplicationTypeDef Appli_state;
extern HID_DEMO_StateMachine hid_demo;
extern USART_HandleTypeDef console_uart;

void Toggle_Leds(void);
void HID_SelectItem(uint8_t** menu, uint8_t item);
void HID_MenuInit(void);
void HID_MenuProcess(void);
void HID_KeyboardMenuProcess(void);
void USR_KEYBRD_ProcessData(uint8_t data);

void uart_print(char *s);
void uart_putc(char c);

#endif
