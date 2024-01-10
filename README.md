# ninjanirs-imu

IMU and multiplexer designs for ninjaNIRS

## IMU Board

This board would house an IMU (nom. the ST ISM330DHCX, since it has a
claimed product life of 10y) with a serial interface of some sort.

We propose asynchronous serial (aka UART) protocol as it is robust
against multiple edge transitions and other channel noise/distortion.
We might also even consider RS-422 differential output.

The board can in principle be "output only" though some sort of
bidirectional communications would be ideal.

A very early trial design using an ATTiny10 was created.
This device has only 32 bytes RAM and 512 words flash and no I2C or
UART hardware.  So it would require carefully crafted assembly code.
Probably a non-starter.

Much more realistic would be something like an ATTiny4313 in a 3x3
VQFN20 package.  This guy has both a USART and a USI so the
interfacing would be relatively trivial.  And, with 4K flash and 256
bytes RAM it should be fine to program in C.

