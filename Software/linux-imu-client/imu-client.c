//
// simple client to read and display binary IMU data
// 
// 
// | Offset | Value    | Size    | Notes                         |
// |--------|----------|---------|-------------------------------|
// | 0      | 0x91     | 8 bits  | sync byte 1                   |
// | 1      | 0x57     | 8 bits  | sync byte 2                   |
// | 2      | count    | 8 bits  | = 14                          |
// | 3, 4   | temp     | 16 bits | raw temperature               |
// | 5, 6   | gyro_x   | 16 bits | raw gyro reading              |
// | 7, 8   | gyro_y   | 16 bits |                               |
// | 9, 10  | gyro_z   | 16 bits |                               |
// | 11, 12 | accel_x  | 16 bits |                               |
// | 13, 14 | accel_y  | 16 bits |                               |
// | 15, 16 | accel_z  | 16 bits |                               |
// | 17     | checksum | 8 bits  | unsigned sum of offsets 1..15 |


#include "imu.h"
#include "imu_const.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include "sio_cmd.h"

static int fd;

// #define BAUD_RATE B115200
#define BAUD_RATE B9600

#define NWORD 7

int read_serial( void *bp, int nchar) {
  uint8_t* buf = (uint8_t *)bp;
  int res;
  uint8_t rch;
  int nc = 0;

  while( nc < nchar) {
    res = read( fd, &rch, 1);
    if( res == 1) {
      buf[nc++] = rch;
    }
  }
  return nc;
}

int main( int argc, char*argv[] )
{
  char cmd = 'T';
  int16_t buff[NWORD];
  uint8_t nw;
  uint8_t err;
  int rc;

  if( argc < 2) {
    printf("usage:  imi-client <port> <cmd>\n");
    exit(1);
  }

  char *port = argv[1];

  if( argc > 2)
    cmd = argv[2][0];

  if( (fd = sio_open( port, BAUD_RATE)) < 0) {
    printf("Error opening serial port %s\n", port);
    exit( 1);
  }

  // flush the buffer
  flush(fd);

  while( 1) {
    flush( fd);
    // send a character
    write( fd, &cmd, 1);

    // receive the command back (why?)
    read_serial( &nw, 1);
    //    printf("Got echo %d (0x%x)\n", nw, nw);

    // receive the count
    read_serial( &nw, 1);
    //    printf("Got count %d\n", nw);

    if( nw == 1) {
      read_serial( &err, 1);
      //      printf("Got error = 0x%x\n", err);
    } else if( nw == 14) {
      //      printf("Trying to read %d bytes\n", nw);
      rc = read_serial( &buff, nw);
      //      printf("Read %d bytes\n", rc);
      printf("Data: ");
      for( int i=0; i<NWORD; i++)
	printf( "%6d ", buff[i]);
      printf("\n");
    } else {
      printf("unknown count = %d\n", nw);
      flush( fd);
    }
      sleep( 1);
  }

}
