//
// read and display IMU data for "SCOS" version
// expects record as:
//   0x91
//   0x57
//   0x0e
//   ... seven two-byte signed values
//   ... 
// 

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

#define MAGIC1 0x91
#define MAGIC2 0x57

#define BUFCHR 20

int main( int argc, char*argv[] )
{
  int res;
  uint8_t rch;
  uint8_t rch0, rch1;
  uint8_t bufr[BUFCHR];
  uint8_t count;
  uint8_t i;
  int16_t val;

  if( argc < 2) {
    printf("usage:  imi-test <port>\n");
    exit(1);
  }

  char *port = argv[1];

  if( (fd = sio_open( port, BAUD_RATE)) < 0) {
    printf("Error opening serial port %s\n", port);
    exit( 1);
  }

  // flush the buffer
  flush(fd);

  // read until magic seen
  printf("Waiting for synch chars...\n");
  do {
    res = read( fd, &rch, 1);
    if( res == 1) {
      rch0 = rch1;
      rch1 = rch;
    }
  } while( rch1 != MAGIC2 || rch0 != MAGIC1);

  while( 1) {

    while( read( fd, &count, 1) != 1)
      ;
    if( count != 0xe)
      printf("Error!  count = %d (expecting 14)\n", count);
    else {
      for( i=0; i<count; i++) {
	while( read( fd, &rch, 1) != 1)
	  ;
	bufr[i] = rch;
      }
      // read and dump checksum
      while( read( fd, &rch, 1) != 1)
	;
      // read and check sync
      while( read( fd, &rch, 1) != 1)
	;
      if( rch != MAGIC1)
	printf("sync error!\n");
      while( read( fd, &rch, 1) != 1)
	;
      if( rch != MAGIC2)
	printf("sync error!\n");
    }

    // dump the buffer as 16-byte integers
    for( int i=0; i<count; i += 2) {
      val = bufr[i] | (bufr[i+1] << 8);
      printf("%d ", val);
    }
    printf("\n");
    

  }
    
}
