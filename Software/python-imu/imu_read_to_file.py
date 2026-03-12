#!/usr/bin/python3
#
# imu_read_to_file.py
#
# read and plot gyro and acceleration data from IMU
# expects IMU firmware which is triggered by a recieved byte
# and sends 6 comma-separated integers per trigger
#
# write specified number of samples to file
#
# usage: imu_read_to_file.py <samples> <interval_ms> <file_name>
#     <samples> is number of samples
#     <interval_ms> is interval between samples in ms
#     <file_name> is (text) file to record data in
#
# current date/time is included at start of each record
#


import serial
import sys
import time
import datetime

# connect to device.  For windows use e.g. COM3 for device
ser = serial.Serial('/dev/ttyUSB0', 9600)
# ser = serial.Serial('COM3', 9600)

# trigger byte to send to IMU
trig = b'\xff';

# parse arguments
if(len(sys.argv) > 3):
    samples = int(sys.argv[1])
    samp_interval = int(sys.argv[2])
    fname = sys.argv[3]
else:
    print("Usage: ", sys.argv[0], " <samples> <interval_ms> <file_name>")
    sys.exit()

# get starting time in ms
start_ms = round(time.time()*1000)
ns = 0

try:
    f = open(fname, "w", encoding="utf-8")
except Exception as e:
    print("Error opening file")
    sys.exit()

# loop over specified number of samples
for i in range(samples):
    now = datetime.datetime.now()
    time_now = round(time.time()*1000)-start_ms # get the time
    ser.write(trig)
    bdata = ser.readline()                      # read data from IMU
    tdata = bdata.decode('utf-8')               # convert to text
    # write date/time stamp in quotes
    f.write( "\"")
    f.write( str(now))
    f.write( "\",")
    # write the IMU data
    f.write( tdata)
    print(ns,"/",samples,end="\r")               # print sample# for progress
    ns = ns + 1
    # delay until interval has elapsed
    while True:
        our_time = round(time.time()*1000)-start_ms
        if our_time - time_now > samp_interval:
            break;

f.close()
