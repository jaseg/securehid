/**
  ******************************************************************************
  * @file    USB_Host/HID_Standalone/Src/main.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    17-February-2017
  * @brief   USB host HID Mouse and Keyboard demo main file
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

#include "main.h"

SCB_Type *scb = SCB;
GPIO_TypeDef *gpioa = GPIOA;

USBH_HandleTypeDef hUSBHost;
HID_ApplicationTypeDef Appli_state = APPLICATION_IDLE;

USART_HandleTypeDef console_uart;

static void SystemClock_Config(void);
static void USBH_UserProcess(USBH_HandleTypeDef* phost, uint8_t id);
static void HID_InitApplication(void);
static void Error_Handler(void);

void WWDG_IRQHandler()
{
    while (1);
}

void uart_print(char *s)
{
    /* Like, there is millions of lines of perfectly useless pseudo-comments in the HAL driver. But don't think they'd
     * say what unit that timeout parameter has in any obvious place. I'll just go with milliseconds here -.- */
    HAL_USART_Transmit(&console_uart, (uint8_t*)s, strlen(s), 1000);
}

void uart_putc(char c)
{
    /* See above. */
    HAL_USART_Transmit(&console_uart, (uint8_t*)&c, 1, 1000);
}

void HAL_USART_MspInit(USART_HandleTypeDef *narf)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    /* TX */
    GPIO_InitTypeDef pa9 = {
        .Pin = GPIO_PIN_9,
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_PULLUP,
        .Speed = GPIO_SPEED_FREQ_LOW,
        .Alternate = GPIO_AF7_USART1
    };
    HAL_GPIO_Init(GPIOA, &pa9);

    /* RX */
    GPIO_InitTypeDef pa10 = {
        .Pin = GPIO_PIN_10,
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_PULLUP,
        .Speed = GPIO_SPEED_FREQ_LOW,
        .Alternate = GPIO_AF7_USART1
    };
    HAL_GPIO_Init(GPIOA, &pa10);
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    /* FIXME
    HID_InitApplication();
    USBH_Init(&hUSBHost, USBH_UserProcess, 0);
    USBH_RegisterClass(&hUSBHost, USBH_HID_CLASS);
    USBH_Start(&hUSBHost);
    */

    /* LEDs */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();
    RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |=
        (1<<GPIO_MODER_MODER6_Pos)|
        (1<<GPIO_MODER_MODER7_Pos);

    GPIOA->OSPEEDR |=
        (2<<GPIO_OSPEEDR_OSPEED6_Pos)|
        (2<<GPIO_OSPEEDR_OSPEED7_Pos);

    USART_HandleTypeDef foo = {
        .Instance = USART1,
        .Init = {
            .BaudRate = 115200,
            .WordLength = USART_WORDLENGTH_8B,
            .StopBits = USART_STOPBITS_1,
            .Parity = USART_PARITY_NONE,
            .Mode = USART_MODE_TX_RX
        }
    };
    console_uart = foo;
    HAL_USART_Init(&console_uart);

    static int ticks = 0;
    while (1) {
        /* FIXME USBH_Process(&hUSBHost); */
        /* FIXME HID_MenuProcess(); */
        Toggle_Leds();
        if (ticks++ == 1000000) {
            uart_print("This is a test\r\n");
            ticks = 0;
        }
    }
}

static void HID_InitApplication(void)
{
    HID_MenuInit();
}

static void USBH_UserProcess(USBH_HandleTypeDef* phost, uint8_t id)
{
    switch (id) {
    case HOST_USER_SELECT_CONFIGURATION:
        break;

    case HOST_USER_DISCONNECTION:
        Appli_state = APPLICATION_DISCONNECT;
        break;

    case HOST_USER_CLASS_ACTIVE:
        Appli_state = APPLICATION_READY;
        break;

    case HOST_USER_CONNECTION:
        Appli_state = APPLICATION_START;
        break;

    default:
        break;
    }
}

void Toggle_Leds(void) {
    static uint32_t ticks;

    if (ticks++ == 1000000) {
        GPIOA->ODR ^= GPIO_ODR_OD6;
        GPIOA->ODR ^= GPIO_ODR_OD7;
        ticks = 0;
    }
}

static void SystemClock_Config(void)
{
    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* Enable HSE Oscillator and activate PLL with HSE as source */
    RCC_OscInitTypeDef foo = {
        .OscillatorType = RCC_OSCILLATORTYPE_HSE,
        .HSEState = RCC_HSE_ON,
        .PLL.PLLState = RCC_PLL_ON,
        .PLL.PLLSource = RCC_PLLSOURCE_HSE,
        /* HSE input: 8MHz */
        .PLL.PLLM = 8,             /* VCO in: 1MHz = 8MHz / 8 */
        .PLL.PLLN = 336,           /* VCO out: 336MHz = 1MHz * 336 */
        .PLL.PLLP = RCC_PLLP_DIV2, /* System: 168MHz = 336Mhz / 2 */
        .PLL.PLLQ = 7              /* USB: 48MHz = 336MHz / 7 */
    };
    if (HAL_RCC_OscConfig(&foo) != HAL_OK)
        Error_Handler();

    /* FIXME does this require configuration?
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CK48;
    PeriphClkInitStruct.Clk48ClockSelection = RCC_CK48CLKSOURCE_PLLSAIP;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
    */

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
    RCC_ClkInitTypeDef bar = {
        .ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2),
        .SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK,   /* See above */
        .AHBCLKDivider = RCC_SYSCLK_DIV1,          /* 168MHz = 168MHz / 1 */
        .APB1CLKDivider = RCC_HCLK_DIV4,           /* 42MHz = 168MHz / 4 */
        .APB2CLKDivider = RCC_HCLK_DIV2            /* 84Mhz = 168MHz / 2 */
    };
    if (HAL_RCC_ClockConfig(&bar, FLASH_LATENCY_5) != HAL_OK)
        Error_Handler();
}

void HAL_Delay(__IO uint32_t Delay)
{
    while (Delay) {
        if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
            Delay--;
    }
}

static void Error_Handler(void)
{
    while (1)
        ;
}

void _init(void) {}

