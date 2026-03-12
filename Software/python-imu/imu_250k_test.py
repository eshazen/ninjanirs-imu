#!/usr/bin/python3
#
# imu_250k_test
# test "standard" fNIRS firmware
#
# Expected binary output:
#   <echo> <count> <length> <checksum>
#
# offset 0      <echo>     = xx   echo of byte received
# offset 1      <count>    = 14   normal data, 
# offset 2..15  <data>     =      (7) 16-bit words of Temp, GyroX/Y/Z, Accel X/Y/Z
# offset 16     <checksum> =      uint8_t sum of bytes at offset 1..15
#
# if any error occurs, <count> = 1 and the error code is somehow related
# to the I2C error.

import serial
import sys
import time

# connect to device.  For windows use e.g. COM3 for device
ser = serial.Serial( port='/dev/ttyUSB0', baudrate=250000, timeout=0.1)

# Firmware sends 17 bytes now
num_bytes_to_read = 20

trig = b'\xff';

def to_int16(v):
    v &= 0xffff
    if v & 0x8000:
        v -= 0x10000
    return v

imu_vals = ["Temp", "GyrX", "GyrY", "GyrZ", "AccX", "AccY", "AccZ"]

while True:
    ser.write(trig);
    bd = ser.read(num_bytes_to_read)
    print(f"Read {len(bd)} bytes: {bd}")
    if len(bd) != 17:
        print("Error, expected 17")
    for i in range(7):
        val = to_int16(bd[2*i+2] + 256 * bd[2*i+3])
        print( i, imu_vals[i], val)
    time.sleep(1);
