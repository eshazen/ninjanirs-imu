/*
 * First attempt at code for the ninjaNIRS custom IMU
 * Read an ISM330DHCX on the hardware I2C pins
 */

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "i2c.h"

#include "imu.h"

// Arduino LED is on PB5

#define LED_DDR DDRB
#define LED_BIT 5
#define LED_PORT PORTB

// create a file pointer for read/write to USART0
FILE usart0_str = FDEV_SETUP_STREAM(USART0SendByte, USART0ReceiveByte, _FDEV_SETUP_RW);

static char buff[40];

int main (void)
{
  unsigned char c = ' ';

  USART0Init();
  stdout = &usart0_str;		/* connect UART to stdout */
  stdin = &usart0_str;		/* connect UART to stdin */

  i2c_init( BDIV);		/* initialize I2C */

  // set up the IMU

  puts("IMU test");

  while( 1) {
    puts("> ");
    fgets( buff, sizeof( buff), stdin);
    
  }
}


