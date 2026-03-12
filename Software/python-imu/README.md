# Python test script

`imu_250k_test.py` works for "standard" fNIRS firmware.  This firmware triggers
a readout on any Rx data on the UART and sends a binary frame as below.
The bit rate is 250k.  This works on Linux with an FTDI interface, YMMV.

    
    Expected binary output:
      <echo> <count> <length> <checksum>
    
    offset 0      <echo>     = xx   echo of byte received
    offset 1      <count>    = 14   normal data, 
    offset 2..15  <data>     =      (7) 16-bit words of Temp, GyroX/Y/Z, Accel X/Y/Z
    offset 16     <checksum> =      uint8_t sum of bytes at offset 1..15
    
    if any error occurs, <count> = 1 and the error code is somehow related
    to the I2C error.

