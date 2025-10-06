// Sadhvi Narayanan, sanarayanan@g.hmc.edu, 10/4/2025
/*

File    : TIMER.h
Purpose : Set up struct for TIM15 so we can directly access the memory registers

*/


// TIMER_H
// Header for RCC functions

#ifndef STM32L4_TIMER_H
#define STM32L4_TIMER_H

#include <stdint.h>
#include <math.h>
#include <stm32l432xx.h>


///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////

#define __IO volatile


// PLL
#define PSC_VAL_TIMER 3000


///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

void init_delay(void);
void delay(int song_dur);

#endif