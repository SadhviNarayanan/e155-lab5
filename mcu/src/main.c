// Sadhvi Narayanan, sanarayanan@g.hmc.edu, 10/4/2025
/*

File    : main.c
Purpose : Initialize clocks, interrupts, and create the interrupt handler functions to calculate the direction, angular velocity, and display to console

*/


#include <stdio.h>
#include <stdlib.h>
#include <stm32l432xx.h>

#include "TIMER.h"
#include "STM32L432KC_FLASH.h"
#include "STM32L432KC_RCC.h"
#include "STM32L432KC_GPIO.h"
#include "INTERRUPT.h"


volatile int count = 0;
volatile int dir = 1;
volatile float ang_velocity = 0;
volatile int update = 0;

#define PULSE_PER_REV 408

#define A ((GPIOA->IDR >> 9) & 0x1)
#define B ((GPIOA->IDR >> 10) & 0x1)

int main(void) {
    configureFlash();
    configureClock();

    RCC->AHB2ENR |= (1 << 0); // GPIOA clock 
    RCC->APB2ENR |=  (1 << 17); // TIM16 clock
    RCC->APB2ENR |=  (1 << 16); // TIM15 clock
    RCC->APB2ENR |= (1 << 0); // SYSCFG clock enable


    // GPIO->MODER |=  (2 << (6 * 2));   // set pin A6 mode to 10 (AF) -> AF for pwm
    pinMode(9, GPIO_INPUT); // set pin A9 as an input
    pinMode(10, GPIO_INPUT); // set pin A10 as an input

    GPIOA->PUPDR &= ~(0b11 << 18); // reset pull up config for pin 9
    GPIOA->PUPDR |= (0b01 << 18); // turn on internal pull-ups for pin 9

    GPIOA->PUPDR &= ~(0b11 << 20); // reset pull up config for pin 10
    GPIOA->PUPDR |= (0b01 << 20); // turn on internal pull-ups for pin 10


    // initialize our system
    init_delay();
    enable_interrupt();	

    delay(1000); // 1 second delay

    while (1) {
      if (update) {
        // push to terminal
        update = 0;
        volatile int print_dir = dir;
        if (ang_velocity < 0) {
          print_dir = -1;
        } else if (ang_velocity > 0) {
          print_dir = 1;
        }
        printf("Angular Velocity: %.1f rev/s\nDirection: %d\n", ang_velocity, print_dir);
      }
    }
}

void EXTI9_5_IRQHandler() {
  // check for the interrupt pending register for input line 9 (pin 9)
  if (((EXTI->PR1 >> 9) & 1) == 1) {
    EXTI->PR1 |= (1 << 9); // clear interrupt bit for pin 9
    // if sampled on pos/neg edge of A, and B is different --> moving clockwise
    if (A != B) { // clockwise
      dir = 1;
      count += 1;
    } else { // counter-clockwise
      dir = -1;
      count -= 1;
    }
  }
}


void EXTI15_10_IRQHandler() {
// check for the interrupt pending register for input line 10 (pin 10)
  if (((EXTI->PR1 >> 10) & 1) == 1) {
    EXTI->PR1 |= (1 << 10); // clear interrupt bit for pin 10
    if (A == B) { // clockwise
      dir = 1;
      count += 1;
    } else { // counter-clockwise
      dir = -1;
      count -= 1;
    }
  }
}

void TIM1_UP_TIM16_IRQHandler() {
  // timer interrupt every 1 second to calculate pulses for angular velocity and to display to terminal
  if ((TIM16->SR & 1) == 1) {
    TIM16->SR &= ~(1 << 0); // clear interrupt bit
    ang_velocity = (float) count / 4 / (float)PULSE_PER_REV; // triggering on every rising/falling edge and doubling it --> so need to divide by 4
    // fixes direction to 0 if not spinning
    if (ang_velocity == 0.0) {
      dir = 0;
    }
    update = 1; // flag to display to terminal so its not handled in the interrupt
    count = 0;
    TIM16->CNT = 0; // reset it here so it will auto count at a resetted value
  }
}

int _write(int file, char *ptr, int len) {
 int i = 0;
 for (i = 0; i < len; i++) {
   ITM_SendChar((*ptr++));
 }
 return len;
}

