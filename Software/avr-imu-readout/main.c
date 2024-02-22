/*
 * main.c - main program for AVR IMU readout
 * 
 * wait to receive a character (currently anything)
 * send a binary return:
 *   <count> <length>
 *
 *   <count> = 14   normal data, 7 16-bit words of Temp, GyroX/Y/Z, Accel X/Y/Z
 *   <count> = 1    error code or other info
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

void send_data( uint8_t nb, uint8_t* d);

static int16_t acc_test[] = { 1111, 2222, 3333, 4444, 5555, 6666, 7777 };

int main (void)
{
  uint8_t loop_cnt = 0;
  int16_t acc_res[7];
  uint8_t res;
  bool acc_connected = false;

  USART0Init();			/* initialize USART (no interrupts) */
  i2c_init( BDIV);		/* initialize I2C */

  // send a '*' to indicate we're alive
  USART0SendByte( '*', 0);

  while ( true) {

    while( !USART0CharacterAvailable())
      ;
    res = USART0ReceiveByte(0);

    if( res == 'T') {

      // send fake data
      send_data( 14, (uint8_t *)acc_test);

    } else if( res == 'A') {

      // Accelerometer init
      while (!acc_connected){
	res = init_acc();
	//      if (res==PICO_ERROR_GENERIC){
	if (res){      
	  send_data( 1, &res);
	  _delay_ms(250);
	  acc_connected = false;
	} else {
	  acc_connected = true;
	}
      }

      res = read_acc(acc_res);
      //    if (res==PICO_ERROR_GENERIC){
      if (res){    
	send_data( 1, &res);
	acc_connected = false;
      }

      // send the data
      send_data( sizeof(acc_res), (uint8_t *)acc_res);
    } else {
      res = 'X';
      send_data( 1, &res);
    }      


  }

}


void send_data( uint8_t nb, uint8_t* d) {
  USART0SendByte( nb, 0);
  for( uint8_t i=0; i<nb; i++)
    USART0SendByte( d[i], 0);
}
