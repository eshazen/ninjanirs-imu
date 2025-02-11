/*
 * main.c - main program for AVR IMU readout
 * 
 * Triggered version:  use pin change on PD0 (aka UART Rx)
 * Send output.  Limit rate to 1/DEBOUNCE_MS
 *
 * ----------------------------------------
 * Binary output:
 *   <echo> <count> <length> <checksum>
 *
 * offset 0      <echo>     = xx   echo of byte received
 * offset 1      <count>    = 14   normal data, 
 * offset 2..15  <data>     =      (7) 16-bit words of Temp, GyroX/Y/Z, Accel X/Y/Z
 * offset 16     <checksum> =      uint8_t sum of bytes at offset 1..15
 *
 * if any error occurs, <count> = 1 and the error code is somehow related
 * to the I2C error.
 *
 * ----------------------------------------
 * Text output:
 * temp,gyroX,gyroY,gyroZ,accelX,accelY,accelZ   (all signed 16-bit decimal)
 *
 * only accel can be sent by setting FIRST_VAR to 4
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

//---- configuration which must (sometimes) happen before user #includes

#define DEBOUNCE_MS 1000

// send fake data (no IMU)
#define NO_IMU

// trigger on edge of PD0 (D0) to send data
// (this also disables UART Rx)
#define PD0_TRIGGER

// readout at fixed interval (no Rx data needed)
// don't use this with PD0_TRIGGER above
// #define AUTO_INTERVAL 100

// First variable to send:
//   temp = 0, gyro = 1..3, Accel = 4..6
#define FIRST_VAR 4

// send text data
#define TEXT_DATA

// send magic number first
#define MAGIC_NUMBER 0x5791

#define LED_BLINKS

#include "uart.h"
#include "i2c.h"

#include "imu.h"
#include "imu_const.h"
#include "timer.h"

#ifdef MAGIC_NUMBER
const uint8_t magic_lo = MAGIC_NUMBER & 0xff;
const uint8_t magic_hi = (MAGIC_NUMBER >> 8) & 0xff;
#endif

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

// last state of PD0 for trigger
#ifdef PD0_TRIGGER
static uint8_t pd0_last;
volatile uint8_t pd0_triggered;
volatile uint32_t last_time;
#endif

int main (void)
{

  USART0Init();			/* initialize USART (no interrupts) */
  timer_setup();		/* setup timer 0 for 1kHz interrupts */

  // enable pull-up on PD0 if GPIO
#ifdef PD0_TRIGGER
  PORTD |= _BV(PD0);
  // set up for pin change interrupt on PD0
  PCICR |= _BV(PCIE2);
  PCMSK2 |= _BV(PD0);		// Port D bit 0 only
#endif  

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
  
#ifdef PD0_TRIGGER
  sei();			// enable interrupts for pin change only
  last_time = get_millis();
#endif

  while ( true) {

// --- auto interval 
#ifdef AUTO_INTERVAL
    _delay_ms( AUTO_INTERVAL);
#else

// --- respond to input data
// --- or wait for trigger signal
#ifdef PD0_TRIGGER

    while( !pd0_triggered)
      ;
    pd0_triggered = 0;

//    // wait for state change to LOW on PD0
//    // <FIXME> this should be a pin-change interrupt, possibly with debounce
//    while(1) {
//      pd0 = PIND & 1;
//     break if( pd0 != pd0_last && pd0 == 0);
//      pd0_last = pd0;
//    }
#else    
    res = USART0ReceiveByte(0);
#endif

#endif    
    

#ifdef LED_BLINKS
      blink_led( 1, 100);
#endif

      //--- read out the IMU and send the data, either way

#ifdef NO_IMU
      // send fake data
      send_data( 14, (uint8_t *)acc_test);
#else
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
#endif
      
  }

}

//
// send nb bytes of data from d to serial port
// (blocking) followed by checksum
//
void send_data( uint8_t nb, uint8_t* d) {

  // ---------- text data
#ifdef TEXT_DATA
  if( nb == sizeof(acc_res)) {	/* normal 7-word data */
    for( int i=2*FIRST_VAR; i<sizeof(acc_res); i += 2) {
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

  // ---------- binary data
#ifdef MAGIC_NUMBER
  USART0SendByte( magic_lo, 0);
  USART0SendByte( magic_hi, 0);
#endif  

  uint8_t sum = nb;
  USART0SendByte( nb, 0);
  for( uint8_t i=2*FIRST_VAR; i<nb; i++) {
    USART0SendByte( d[i], 0);
    sum += d[i];
  }
  USART0SendByte( sum, 0);
#endif
}



//
// pin change interrupt on PD0
//
#ifdef PD0_TRIGGER
ISR( PCINT2_vect) {

#ifdef DEBOUNCE_MS
  if( (get_millis() - last_time) < DEBOUNCE_MS)
    return;
#endif

  last_time = get_millis();

  ++pd0_triggered;
}
#endif

