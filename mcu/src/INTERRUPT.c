// Sadhvi Narayanan, sanarayanan@g.hmc.edu, 10/4/2025
/*

File    : INTERRUPT.c
Purpose : Initialize all the bits for interrupts on both the input pins 9, 10 and tim16. Also use NVIC for the interrupt enable

*/

#include "INTERRUPT.h"

void enable_interrupt() {
    SYSCFG->EXTICR[2] &= ~(1 << 8 | 1 << 9 | 1 << 10); // enable external interrupt for pin 10
    SYSCFG->EXTICR[2] &= ~(1 << 4 | 1 << 5 | 1 << 6); // enable external interrupt for pin 9
    
    EXTI->IMR1 |= (1 << 9 | 1 << 10); // turn on interrupt mask for pins 9 and 10
    EXTI->RTSR1 |= (1 << 9 | 1 << 10); // enable interrupt on rising edge
    EXTI->FTSR1 |= (1 << 9 | 1 << 10); // enable interrupt on falling edge

    TIM16->DIER |= (1 << 0); // turn on interrupt enable register

    __enable_irq(); // enable global interrupts

    __NVIC_EnableIRQ(EXTI9_5_IRQn); // enable interrupts for pin/line 9
    __NVIC_EnableIRQ(EXTI15_10_IRQn); // enable interrupts for pin/line 10
    __NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn); // enable interrupts for tim16

    // set priority on interrupt lines
    __NVIC_SetPriority(EXTI9_5_IRQn, 2);
    __NVIC_SetPriority(EXTI15_10_IRQn,2);
    __NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 1);

}