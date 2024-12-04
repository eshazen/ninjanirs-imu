#!/usr/bin/python3

import matplotlib.pyplot as plt
import math
import random

#---------- fake IMU data using sin ----------
imu_max = 8191
cycles = 2
steps = 8

AX = []
AY = []
AZ = []

X = range( cycles*steps)
for ix in X:
    fx = ((ix % steps)/steps)*2*math.pi
    AX.append( math.sin(fx)*imu_max+random.randint(-1000,1000))
    fy = ((ix+3 % steps)/steps)*2*math.pi
    AY.append( math.sin(fy)*imu_max+random.randint(-1000,1000))
    fz = ((ix+6 % steps)/steps)*2*math.pi
    AZ.append( math.sin(fz)*imu_max+random.randint(-1000,1000))

#---------- plot ----------    

plt.plot(X, AX, label="Accel X", marker='o')
plt.plot(X, AY, label="Accel Y", marker='*')
plt.plot(X, AZ, label="Accel Z", marker='s')
plt.legend()

plt.show()
