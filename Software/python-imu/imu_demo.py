#!/usr/bin/python3
#
# imu_demo.py
#
# read IMU data for specified number of samples
# accumulate the data and plot
#


import matplotlib.pyplot as plt
import serial
import sys
import time

# connect to device.  For windows use e.g. COM3 for device
ser = serial.Serial('/dev/ttyUSB0', 9600)

full_scale = 4.0                # IMU full-scale in G
factor = full_scale/32767       # scale factor for G

# arrays for acceleration on 3 axis
ax = []
ay = []
az = []

X = []                          # X axis array
ns = 0                          # sample counter

samples = 10                    # default sample count
if(len(sys.argv)):
    samples = int(sys.argv[1])

# get starting time in ms
start_ms = round(time.time()*1000)

# loop over specified number of samples
for i in range(samples):
    time_now = round(time.time()*1000)-start_ms # get the time
    bdata = ser.readline()                      # read data from IMU
    tdata = bdata.decode('utf-8')               # convert to text
    acc = tdata.split(',')                      # split to 3 values
    if( len(acc) == 3):                         # only valid lines
        ax.append(int(acc[0])*factor)           # convert to g
        ay.append(int(acc[1])*factor)
        az.append(int(acc[2])*factor)
        X.append(time_now)
        ns = ns + 1
        print(ns,"/",samples,end="\r")               # print sample# for progress

# make a plot with 3 data series
plt.plot( X, ax, label="Accel X", marker='o')
plt.plot( X, ay, label="Accel Y", marker='*')
plt.plot( X, az, label="Accel Z", marker='s')
plt.title("Instantaneous Acceleration (g)")
plt.ylabel("Acceleration (g)")
plt.xlabel("Time (ms)")
plt.legend()
plt.show()

