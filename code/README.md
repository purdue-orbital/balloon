How to Program
--------------
For programs with .ino extension, use Arduino ide.

For other programs which use Makefiles, connect a USBASP programmer (be sure udev rules have been added to /etc/udev/rules.d/) and type `make`

For attiny84 devices with attached 16MHz crystal, use `make 16mhz-external` to setup the fuses to use the oscillator before uploading code for the first time

Programs
-----

## 99-usbasp.rules
- udev rules for usbasp programmer.  the USBASP programmer is used for programmer devices which have an ISP connection, which includes arduinos, atmegas, and attinies.  This file aliases usbasp programmers to /dev/arduino
1. move to /etc/udev/rules.d/99-usbasp.rules
2. restart udev `sudo udevadm control --reload`
3. insert programmer and verify programmer is mounted at /dev/arduino

## master_nano
- code for an arduino connected to the master PC, in this case a Nano

## slave_84
- slave code for attiny84s

## program_dorji
- code for configuring dorji modules.  Sets TX/RX frequency and squelch.  Connect arduino serial TX to dorji pin 16 and connect ground, then power on arduino.

## modem
- modem code for attiny85 on slave/master devices

## test_receive_84
- code for verifying that the modem is working.  Sets radio to receive.  When a logic 1 or 0 is detected on PA4, the relay (LED) is turned on or off

## test_send_84
- code for verifying that the modem is working.  Sets radio to transmit.  Sends alternating logic 1 and 0

## multiplication_speedtest
- test how long a multiplication on an attiny85 takes

## pll_test_85
- proof of concept for clocking attiny at 16MHz from 8MHz crystal
