Arduino IDE 1.0.x Bootloader Burning Error
------------------------------------------

The 1.0.x series of the Arduino IDE includes an avrdude.conf which has
some issues with the ATtiny series when it comes to burning bootloaders.

In order to correct this issue, you need to copy the included 
`1_0_x_avrdude.conf` over the top of your existing 
`hardware/tools/avrdude.conf` in your Arduino installation directory.
