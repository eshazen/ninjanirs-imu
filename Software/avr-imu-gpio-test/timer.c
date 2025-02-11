//
// single timer interrupt to count milliseconds
//
#include "timer.h"

volatile uint32_t millis_value = 0;

//------------------------------------------------------------------------------
void timer_setup(void){

  TCCR0A = (1 << WGM01);	/* set CTC mode so stop at OCR0A */
  
  // enable Timer0+1 overflow interrupt:
  TIMSK0 = (1 << OCIE0A);

  // Set CS10 + 11 bits so timer runs at clock speed/64:
  TCCR0B |= (1 << CS01) | (1 << CS00);
#ifdef AVR_TARGET
  OCR0A = 125;			/* 8MHz / 64 / 125 = 1kHz */
#else
  OCR0A = 250;			/* 16MHz / 64 / 250 = 1kHz */
#endif
}


//------------------------------------------------------------------------------
uint32_t get_millis(void){
  return millis_value;
}

//------------------------------------------------------------------------------
ISR(TIMER0_COMPA_vect)
{
  millis_value++;
}

