#!/usr/bin/python3
#
# imu_250k_test
# test "standard" fNIRS firmware
#

import serial
import sys
import time

# connect to device.  For windows use e.g. COM3 for device
ser = serial.Serial( port='/dev/ttyUSB0', baudrate=250000, timeout=0.1)

num_bytes_to_read = 14

trig = b'\xff';

while True:
#    ser.write( 'A'.encode('utf-8'))
    ser.write(trig);
    bd = ser.read(num_bytes_to_read)
    print(f"Read {len(bd)} bytes: {bd}")
    for i in range(7):
        val = bd[2*i] + 256 * bd[2*i+1]
        print(val)
    time.sleep(1);
