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


