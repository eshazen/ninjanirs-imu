# avr-imu-readout

Readout the IMU in response to a serial command byte.
There are two compile-time options:

1.  Any byte recieved sends the return message
2.  'A' sends IMU data, 'T' sends 1111, 2222, 3333 etc test data

Normal data.  All 16-bit values sent LSB first.

| Value   | Size    | Notes            |
|---------|---------|------------------|
| count   | 8 bits  | = 14             |
| temp    | 16 bits | raw temperature  |
| gyro_x  | 16 bits | raw gyro reading |
| gyro_y  | 16 bits |                  |
| gyro_z  | 16 bits |                  |
| accel_x | 16 bits |                  |
| accel_y | 16 bits |                  |
| accel_z | 16 bits |                  |

Error response (may not be a complete list):

| Value | Size   | Notes |                                                       |
|-------|--------|-------|-------------------------------------------------------|
| count | 8 bits | = 1   |                                                       |
| error | 8 bits |       |                                                       |
|       |        | 0x20  | NAK received after sending device address for writing |
|       |        | 0x30  | NAK received after sending data                       |
|       |        | 0x38  | Arbitration lost with address or data                 |
|       |        | 0x48  | NAK received after sending device address for reading |
|       |        | 0xff  | unexpected data from IMU                              |

