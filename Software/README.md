# NN24 IMU board software

## Notes

MCU is an ATmega168 in a 28MLF package.

Pinout:

	22 PC3 - LED
    23 PC4 - SDA  (hardware TWI)
	24 PC5 - SCL  (hardware TWI)
	26 PD0 - RxD  (USART Rx)
	27 PD1 - TxD  (USART Tx)
	
Software should be pretty straightforward, since the built-in peripherals can be used.

## Software in this folder

| Folder              | Description                                            |
|---------------------|--------------------------------------------------------|
| `arduino-demo`      | C code (not Arduino) to test the IMU on an Arduino uno |
|                     | Prints lines of text with 7 values                     |
|                     |                                                        |
| `avr-imu-readout`   | **AVR software to readout IMU**                        |
|                     | Either wait for command byte or timer                  |
|                     | Send data in binary or CSV                             |
|                     |                                                        |
| `avr-imu-blinky`    | Blink the LED for quick hardware test                  |
|                     |                                                        |
| `avr-imu-led-test`  | Read the IMU and encode somehow on the LED             |
|                     | (for easy testing without a UART)                      |
|                     |                                                        |
| `avr-imu-gpio-test` | **AVR software to readout IMU**                        |
|                     | Version using UART Rx as edge trigger for readout      |
|                     | As used by senior design team 2025                     |
|                     |                                                        |
| `linux-imu-client`  | Linux client to read and display binary data from IMU  |
|                     |                                                        |
| `BZ_example_code`   | Yes                                                    |
|                     |                                                        |
