
#include <stm32f407xx.h>
#include <stdint.h>
#include <system_stm32f4xx.h>
#include <stm32f4xx_ll_utils.h>
#include <stm32f4xx_ll_spi.h>
#include <string.h>
#include <unistd.h>
/* 
 * Part number: STM32F030F4C6
 */

void _init(void) {}

void tick(void) {
    for(int i=0; i<50; i++)
        __asm__("nop");
}

int main(void) {
    /*
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR&RCC_CR_HSERDY));
    RCC->CFGR &= ~RCC_CFGR_PLLMUL_Msk & ~RCC_CFGR_SW_Msk;
    RCC->CFGR |= (2<<RCC_CFGR_PLLMUL_Pos) | RCC_CFGR_PLLSRC_HSE_PREDIV;
    RCC->CFGR2 &= ~RCC_CFGR2_PREDIV_Msk;
    RCC->CFGR2 |= RCC_CFGR2_PREDIV_DIV2;
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR&RCC_CR_PLLRDY));
    RCC->CFGR |= (2<<RCC_CFGR_SW_Pos);
    SystemCoreClockUpdate();
    */

    LL_Init1msTick(SystemCoreClock);

    RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER |=
        (1<<GPIO_MODER_MODER5_Pos)|
        (1<<GPIO_MODER_MODER6_Pos)|
        (1<<GPIO_MODER_MODER7_Pos);

    /* Set shift register IO GPIO output speed */
    GPIOA->OSPEEDR |=
        (2<<GPIO_OSPEEDR_OSPEED5_Pos)|
        (2<<GPIO_OSPEEDR_OSPEED6_Pos)|
        (2<<GPIO_OSPEEDR_OSPEED7_Pos);

    GPIOA->BSRR = GPIO_BSRR_BR_5;
    GPIOA->BSRR = GPIO_BSRR_BR_6;
    GPIOA->BSRR = GPIO_BSRR_BR_7;
    while (42) {
        for (int i=0; i<10000; i++)
            tick();
        GPIOA->BSRR = GPIO_BSRR_BS_5;
        GPIOA->BSRR = GPIO_BSRR_BS_6;
        GPIOA->BSRR = GPIO_BSRR_BS_7;
        for (int i=0; i<10000; i++)
            tick();
        GPIOA->BSRR = GPIO_BSRR_BR_5;
        GPIOA->BSRR = GPIO_BSRR_BR_6;
        GPIOA->BSRR = GPIO_BSRR_BR_7;
    }
}

void NMI_Handler(void) {
}

void HardFault_Handler(void) {
    for(;;);
}

void SVC_Handler(void) {
}


void PendSV_Handler(void) {
}

void SysTick_Handler(void) {
}

