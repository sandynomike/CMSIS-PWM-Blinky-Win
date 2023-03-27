//  Minimalist PWM Blinky with only CMSIS (no HAL)
//  Mike Shegedin, 03/2023
//
//  Use Timer 2, Channel 1, (Pin A0) to output 1 Hz signal using PWM.
//  Attach an anode (+ side) of LED to GPIO Pin A0. Then connect a 1k resister between the
//  and cathode (- side) of the LED and ground.

//  Steps Involved:
//  1. Enable TIMER 2 by setting the RCC_APB1ENR_TIM2EN bit in the RCC APB1ENR register.
//  2. Enable GPIO Port A by setting the RCC_APB2ENR_IOPAEN bit in the RCC APB2ENR register.
//  3. Set CNF and MODE bits for GPIO Port A, Pin 0 for output, push-pull, alternate function,
//     2 MHz speed, by setting the appropriate CNF0 and MODE0 bits in the GPIOA CRL register.
//  4. Set PWM period via the TIM2 ARR register.
//  5. Set PWM duty cycle via the TIM2 CCR1 register.
//  6. Set PWM mode 1 by setting the TIM_CCMR1_OC1Mx bits as 110 via the TIM2 CCMR1 register.
//  7. Enable output on Channel 1 of Timer 2 by setting the TIM_CCER_CC1E bit in the TIM2 CCER
//     register.
//  8. Set the prescaler via the TIM2 PSC register.
//  9. Enable the clock counter via the TIM_CR1_CEN bit in the TIM2 CR1 register.

#include "stm32f103xb.h"
int main( void ){

  // =========================================
  // Setup up PWM Output on GPIO A0 (TIM2/CH1)
  // =========================================

  // Enable TIM2
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

  // Enable GPIO Port A
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

  // Set the CNF and MODE bits for Pin A0 as follows:
  // Set CNF0[1:0] to 0b10 for Alternate function output, push-pull. Alternate function as
  // defined by the CNF bits is needed for PWM functionality.
  // Set MODE0[1:0] to 0b10 for output functionality with a max speed of 2 MHz.
  // Note that CNF[1] bit is set to 1 by default, and that bit must be cleared!
  GPIOA->CRL &= ~GPIO_CRL_CNF0_Msk;                       // Clear both CNF bits
  GPIOA->CRL |= ( GPIO_CRL_CNF0_1 | GPIO_CRL_MODE0_1 );   // Set CNF0[1] and MODE0[1] bits.

  // Set TIM2 Frequency to 8000 (8 MHz clock / 8000 = 1 kHz output when there is no prescaler)
  TIM2->ARR = 16000;

  // Set TIM2/CH3 Duty Cycle to 50% (50% of 8000 = 4000)
  TIM2->CCR1 = 8000;

  // Set TIM2/CH3 to PWM Mode 1 (OC1M[2:0] = 0b110)
  TIM2->CCMR1 |=  ( TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 );

  // Enable output on Timer 2 Channel 1 pin
  TIM2->CCER |= TIM_CCER_CC1E;

  // Set prescaler to divide clock by 1000, so 1 kHz becomes 1 Hz.
  // Note that the prescaler value should be reduced by 1 because a prescaler value
  // of zero is actually a prescaler of /1, so a prescaler of 1000 would be /1001.
  TIM2->PSC = 999;

  // Finally, enable the clock counter (turn on the clock!)
  TIM2->CR1 |= TIM_CR1_CEN;

  // Endless Loop
  while( 1 )
  {
  }
}
