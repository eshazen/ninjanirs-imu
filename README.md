# ninjanirs-imu

IMU and multiplexer designs for ninjaNIRS.  PCBs in this repo:

* `KiCAD/imu-adapter` - programming adapter for MCU programming (for V1 and V2)
* `KiCAD/imu-board` - the IMU board.  Now V2.  Accommodates an optional RJ-11 jack on the back
* `KiCAD/imu-rj-adapter` - RJ-11 adapter for V1 IMU boards (obsolete)
* `KiCAD/imu-splitter` - splitter for connecting IMU to NinjaNIRS readout via ribbon cable (for V1 and V2)

## IMU Board

Board designed in early 2024 along with various adapters and a splitter.
The IMU board has an ATMeg168 so plenty of resources.

### UART connector pinout

    1 - VCC (5V or 3.3V)
	2 - TxD (output from MCU)
	3 - RxD (input to MCU)
	4 - GND
	
### RJ Adapter board pinout

    GND     1
	RxD        2
	TxD     3
	VCC        4

### Using IMU with other systems

The IMU can be used with systems other than NinjaNIRS (fNIRS).  See [OTHERS.md](OTHERS.md) for details.
