# avr-imu-gpio-test

Readout the IMU in response to a trigger on the "UART Rx" pin (PD0)

Text data option:

Send a CSV record of up to 7 decimal values with comma separator.
<br>Can choose first variable to send with FIRST_VAR
<br>   temp = 0, gyro = 1..3, Accel = 4..6

Binary data option.  All 16-bit values sent LSB first.

| Offset | Value    | Size    | Notes                         |
|--------|----------|---------|-------------------------------|
| 0      | echo     | 8 bits  | received byte                 |
| 1      | count    | 8 bits  | = 14                          |
| 2, 3   | temp     | 16 bits | raw temperature               |
| 4, 5   | gyro_x   | 16 bits | raw gyro reading              |
| 6, 7   | gyro_y   | 16 bits |                               |
| 8, 9   | gyro_z   | 16 bits |                               |
| 10, 11 | accel_x  | 16 bits |                               |
| 12, 13 | accel_y  | 16 bits |                               |
| 14, 15 | accel_z  | 16 bits |                               |
| 16     | checksum | 8 bits  | unsigned sum of offsets 1..15 |

Error response (may not be a complete list):

| Offset | Value    | Size   | Notes         |                                                       |
|--------|----------|--------|---------------|-------------------------------------------------------|
| 0      | echo     | 8 bits | received byte |                                                       |
| 1      | count    | 8 bits | = 1           |                                                       |
| 2      | error    | 8 bits |               |                                                       |
|        |          |        | 0x20          | NAK received after sending device address for writing |
|        |          |        | 0x30          | NAK received after sending data                       |
|        |          |        | 0x38          | Arbitration lost with address or data                 |
|        |          |        | 0x48          | NAK received after sending device address for reading |
|        |          |        | 0xff          | unexpected data from IMU                              |
| 3      | checksum | 8 bits |               | unsigned sum of offsets 1, 2                          |

