#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void timer_setup(void);
uint32_t get_millis(void);

#endif
