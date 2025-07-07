# Using the IMU with other systems

This page has some information for using the IMU with non- NinjaNIRS (fNIRS) systems.

The small IMU board comes in two versions.

V1:  ![](Images/v1-imu-front.jpg?raw=true)  ![](Images/v1-imu-back.jpg?raw=true)

V2:  ![](Images/v2-imu.jpg?raw=true)

Both can be used.  The V1 board requires either wires soldered
directly to the board or a separate adapter board which connects to an RJ-11 (modular) jack.

Adapter:  ![](Images/adapter.jpeg?raw=true)

## Connector Pinouts

| Pin No | Function | Notes                     |
|--------|----------|---------------------------|
| 1      | VCC      | 3.3V power                |
| 2      | TxD      | Data out from IMU         |
| 3      | RxD      | Data in or trigger to IMU |
| 4      | GND      | Ground                    |

V1 Pinout: ![](Images/v1-imu-pinout.jpg?raw=true)

V2 Pinout: ![](Images/v2-imu-pinout.jpg?raw=true)

Pin 2 provides the data out from the IMU.  The firmware supplied for
"other" (non-fNIRS) users can be compiled with various options, but
the usual one is that a pulse on the `RxD` pin causes the IMU data to
be read and transmitted on the `TxD` pin.

The default data format is three 16-bit integers separated by commas,
representing acceleration along the X, Y and Z axis.


