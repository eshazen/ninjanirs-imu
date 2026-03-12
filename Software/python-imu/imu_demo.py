#!/usr/bin/python3
#
# imu_demo.py
#
# read and plot gyro and acceleration data from IMU
# expects IMU firmware which is triggered by a recieved byte
# and sends 6 comma-separated integers per trigger
#


import matplotlib.pyplot as plt
import serial
import sys
import time

# connect to device.  For windows use e.g. COM3 for device
ser = serial.Serial('/dev/ttyUSB0', 9600)
# ser = serial.Serial('COM3', 9600)

full_scale = 4.0                # IMU full-scale in G
factor = full_scale/32767       # scale factor for G

dfactor = 250.0/32767           # scale factor for dps

trig = b'\xff';

samp_interval = 250             # sample interval in ms

# arrays for acceleration on 3 axis
ax = []
ay = []
az = []

gx = []
gy = []
gz = []

X = []                          # X axis array
ns = 0                          # sample counter

ndata = 6                       # number of words sent

samples = 10                    # default sample count
if(len(sys.argv) > 1):
    samples = int(sys.argv[1])

# get starting time in ms
start_ms = round(time.time()*1000)

# loop over specified number of samples
for i in range(samples):
    time_now = round(time.time()*1000)-start_ms # get the time
    ser.write(trig)
    bdata = ser.readline()                      # read data from IMU
    tdata = bdata.decode('utf-8')               # convert to text
    acc = tdata.split(',')                      # split to 3 values
    if( len(acc) == ndata):                         # only valid lines
        gx.append(int(acc[0])*dfactor)
        gy.append(int(acc[1])*dfactor)
        gz.append(int(acc[2])*dfactor)
        ax.append(int(acc[3])*factor)           # convert to g
        ay.append(int(acc[4])*factor)
        az.append(int(acc[5])*factor)
        X.append(time_now)
        ns = ns + 1
        print(ns,"/",samples,end="\r")               # print sample# for progress
    # delay 500ms
    while True:
        our_time = round(time.time()*1000)-start_ms
        if our_time - time_now > samp_interval:
            break;

# make a plot with 3 data series
plt.figure(1)
plt.plot( X, ax, label="Accel X", marker='o')
plt.plot( X, ay, label="Accel Y", marker='*')
plt.plot( X, az, label="Accel Z", marker='s')
plt.title("Instantaneous Acceleration (g)")
plt.ylabel("Acceleration (g)")
plt.xlabel("Time (ms)")
plt.legend()

plt.figure(2)
plt.plot( X, gx, label="Gyron X", marker='o')
plt.plot( X, gy, label="Gyron Y", marker='*')
plt.plot( X, gz, label="Gyron Z", marker='s')
plt.title("Rotation Rate")
plt.ylabel("Degrees/s")
plt.xlabel("Time (ms)")


plt.show()

