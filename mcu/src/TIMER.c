// Sadhvi Narayanan, sanarayanan@g.hmc.edu, 9/21/2025
/*

File    : TIMER.c
Purpose : Initialize timer on TIM15, so we can use it for the delay in our music

*/


#include "TIMER.h"

void init_delay() {
    // setting the pre-scalar capture value (timer clock)
    TIM16->PSC = PSC_VAL_TIMER;

    // disabling sl*** mode
    TIM16->SMCR &= ~(1 << 16 | 1 << 2 | 1 << 1 | 1 << 0); // SMS

    // setting the UG (update generation) bit to 1, to allow for shadow register update
    TIM16->EGR |= (1 << 0); // UG

    // turning on the counter enable
    TIM16->CR1 |= (1 << 0); // CEN
}

void delay(int song_dur){
    // calculating the length of the notes array
    // calculating the ARR value (max_counter value)
    uint32_t timer_clk = 80000000 / (PSC_VAL_TIMER + 1);
    uint32_t arr_val = ((timer_clk / 1000) * song_dur) - 1;

    // set the counter max value to the calculated amount
    TIM16->ARR = arr_val; // ARR

    // set the UG (update generation) bit to 1 to update registers
    TIM16->EGR |= (1 << 0); // UG

    // reset the counter for safety
    TIM16->CNT = 0; // CNT

    // make sure the UIF bit is 0 for safety
    TIM16->SR &= ~(1 << 0); // UIF

    // remove busy wait in this function and make it interrupt triggered
    // wait until the UIF bit turns to 1 --> this means we reached our max counter value in ARR
    // while ((TIM15->SR & 1) == 0); // UIF
}


