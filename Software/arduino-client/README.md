# arduino-client

Test client to periodically read data from IMU unit and re-transmit
as text for testing.

Can run on an Arduino Uno with care:  need to disable the USB serial
by not connecting a USB cable, and powering the board externally.
Requires a separate serial USB adapter to read the data.

Connect as follows:

    D0 - serial data from IMU (nom. 250kB)
	D2 - serial data to computer (9600)
	
	
