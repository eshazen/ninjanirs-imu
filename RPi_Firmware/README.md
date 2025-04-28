# Raspberry Pi RP2040 Firmware

This directory has firmware images from Bernhard.

`IMUAdapterTestFirmware.uf2` -- Initial version tested 4/2025.  May have J2 Tx/Rx swap.

`IMUAdapterTestFirmware_20250418.uf2` -- Updated version of 4/22/25.  <br>Changes:

* Well defined sample rate of 20 Hz
* Sampling of a 'Sync IN' digital input, and output of its value at the end of the IMU data line
* Corrected the RX/TX swap on J2 of the splitter

![](Sync_in.jpg?raw=true)
