/*
 * main.c - main program for AVR IMU readout
 * 
 * wait to receive any character
 *
 * sends a binary return record
 *   <echo> <count> <length> <checksum>
 *
 * offset 0      <echo>     = xx   echo of byte received
 * offset 1      <count>    = 14   normal data, 
 * offset 2..15  <data>     =      (7) 16-bit words of Temp, GyroX/Y/Z, Accel X/Y/Z
 * offset 16     <checksum> =      uint8_t sum of bytes at offset 1..15
 *
 * if any error occurs, <count> = 1 and the error code is somehow related
 * to the I2C error.
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "i2c.h"

#include "imu.h"
#include "imu_const.h"

// send text data
#define TEXT_DATA

// readout at fixed interval (no Rx data needed)
#define AUTO_INTERVAL 500

// send magic number first
#define MAGIC_NUMBER 0x5791

#ifdef MAGIC_NUMBER
const uint8_t magic_lo = MAGIC_NUMBER & 0xff;
const uint8_t magic_hi = (MAGIC_NUMBER >> 8) & 0xff;
#endif

#define LED_BLINKS
// #define DEBUG_COMMANDS

#include "leds.h"

#ifdef AVR_TARGET
// bits for TWI interface for pull-up enable
#define TWI_PORT PORTC
#define TWI_BITS_MASK ((1 << 4) | (1 << 5))
#endif


// these work for both (jumpers on target, D3 and D4 on Arduino)
#define JUMPER_DDR DDRD
#define JUMPER_PORT PORTD
#define JUMPER_1_BIT 4
#define JUMPER_2_BIT 3

void send_data( uint8_t nb, uint8_t* d);

// dummy data for testing
static int16_t acc_test[] = { 1111, 2222, 3333, 4444, 5555, 6666, 7777 };

// buffer for text data
#ifdef TEXT_DATA
static char btext[10];
static int16_t tval;
#endif

static uint8_t loop_cnt = 0;
static int16_t acc_res[7];
static uint8_t res;
static uint8_t chr;
static bool acc_connected = false;

int main (void)
{

  USART0Init();			/* initialize USART (no interrupts) */

#ifdef AVR_TARGET
  // activate internal pull-ups on I2C pins
  TWI_PORT |= TWI_BITS_MASK;
#endif

  // activate pull-ups on jumpers
  JUMPER_PORT |= ((1 << JUMPER_1_BIT) | (1 << JUMPER_2_BIT));

  // setup I/O bits
  LED_DDR |= (1 << LED_BIT);

  i2c_init( BDIV);		/* initialize I2C */

#ifdef LED_BLINKS
  // blink the LED
  blink_led( 3, 250);		/* blink 3 times at power-up */
#endif
  
#ifdef DEBUG_COMMANDS
  // send a '*' to indicate we're alive
  USART0SendByte( '*', 0);
#endif

  while ( true) {

// --- auto interval 
#ifdef AUTO_INTERVAL
    _delay_ms( AUTO_INTERVAL);
#else

// --- respond to input data
    res = USART0ReceiveByte(0);

#ifdef DEBUG_COMMANDS
    if( res == 'T') {

#ifdef LED_BLINKS
      blink_led( 2, 100);
#endif
      // send fake data
      send_data( 14, (uint8_t *)acc_test);

    } else if( res == 'A') {

#endif

#endif      

#ifdef LED_BLINKS
      blink_led( 1, 100);
#endif

      //--- read out the IMU and send the data, either way

      // Accelerometer init
      while (!acc_connected){
	res = init_acc();
	if (res){      		/* error return */
	  send_data( 1, &res);
	  _delay_ms(250);
	  acc_connected = false;
	} else {
	  //	  chr = 'O';
	  //	  send_data( 1, &chr);
	  acc_connected = true;
	}
      }

      // simple mode, just send the data or an error packet
      res = read_acc(acc_res);
      if (res){    		/* error return */
	send_data( 1, &res);
	acc_connected = false;
      }

      // send the data
      send_data( sizeof(acc_res), (uint8_t *)acc_res);

#ifdef DEBUG_COMMANDS      

    } else {
      res = 'X';
      send_data( 1, &res);
    }      
#endif

  }

}

//
// send nb bytes of data from d to serial port
// (blocking) followed by checksum
//
void send_data( uint8_t nb, uint8_t* d) {

  // text data
#ifdef TEXT_DATA
  if( nb == sizeof(acc_res)) {	/* normal 7-word data */
    for( int i=0; i<sizeof(acc_res); i += 2) {
      tval = d[i] | (d[i+1] << 8);
      snprintf( btext, sizeof(btext), "%d", tval);
      USART0SendString( btext, 0);
      if( i == sizeof(acc_res)-2) {
	USART0SendByte( '\r', 0);
	USART0SendByte( '\n', 0);
      } else {
	USART0SendByte( ',', 0);
      }	
    }
  }
#else

  // binary data
#ifdef MAGIC_NUMBER
  USART0SendByte( magic_lo, 0);
  USART0SendByte( magic_hi, 0);
#endif  

  uint8_t sum = nb;
  USART0SendByte( nb, 0);
  for( uint8_t i=0; i<nb; i++) {
    USART0SendByte( d[i], 0);
    sum += d[i];
  }
  USART0SendByte( sum, 0);
#endif
}




