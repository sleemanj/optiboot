# Developers Information

This information is for people who want to build their own distribution, or add new chip to the optiboot system.  If you just want to use the distribution, see [README.md](README.md)

## I want to build the bootloaders/distribution from source!

Assuming that you are on a Linux platform, and you have a reasonably modern avr-gcc and avrdude installed...

  ./makedist.sh
  
will build all the currently supported bootloaders (in the dists/*/avr/bootloaders folder(s)), create zip files under dists and update the package json file.

If you just want to build one bootloader, for example, atmega328p running at 16MHz with 57600 baud rate for the upload, then you can do...

    cd optiboot/bootloaders/optiboot
    AVR_FREQ=16000000L BAUD_RATE=57600 make atmega328p
    
and the hex file will be created.

If you want to make the bootloader and burn it to your chip right then...

    cd optiboot/bootloaders/optiboot
    AVR_FREQ=16000000L BAUD_RATE=57600 make atmega328p_isp
    
## I want to add a new chip to optiboot's capabilities

See `Makefile.atmega8` for a start, but in short

  * Use an AVR Fuse Calculator to work out the L, H and E fuse for your base setting - I have chosen to se 8MHz Internal Oscillator, you typically need 256 words (512 bytes) for the boot flash size, or larger if your chip doesn't go that small, and boot reset vector enabled (and of course SPI enabled, reset enabled etc as necessary). 
  * The `--section-start=.text=0x****` is the total flash memory of the chip minus the size of the boot section in bytes.
  * the `--section-start=.version=0x****` is the total flash memory of th echip minus 2 bytes
  * avrdude, and avr-libc sometimes don't know about a specific variant of a chip.  If it is avr-libc that doesn't know (error when compiling) then you need to change the `MCU_TARGET` in the main compile rules (not the isp rules).  If it's avrdude that doesn't know (error when burning), change the `MCU_TARGET` in the `_isp` rules.  In either case you can generally change to a close-enough target, eg the atmega328pa rules target the atmega328p instead because libc doesn't know about the pa.
  * you can determine what avr-libc knows about by searching inside /usr/lib/avr/include/avr/io.h, for example search that file for "mega48" and you will see it knos about the 48, 48A which are treated as the same chip and 48P which is treated differently.
  * you can determine what avrdude knows about by searching inside /etc/avrdude.conf, for example search that file for "mega48" and we can see it (at least mine) knows about the 48 and the 48P, but it doesn't directly know about the 48A so in our Makefile we have set the atmega48a_isp MCU_TARGET to atmega48 to keep avrdude happy



----

> Although it has evolved considerably, Optiboot builds on the original work of Jason P. Kyle (stk500boot.c), [Arduino group (bootloader)](http://arduino.cc), [Spiff (1K bootloader)](http://spiffie.org/know/arduino_1k_bootloader/bootloader.shtml), [AVR-Libc group](http://nongnu.org/avr-libc) and [Ladyada (Adaboot)](http://www.ladyada.net/library/arduino/bootloader.html).

> _Optiboot is the work of Peter Knight (aka Cathedrow). Despite some misattributions, it is not sponsored or supported by any organisation or company including Tinker London, Tinker.it! and Arduino._  
> Maintenance of optiboot was taken over by Bill Westfield (aka WestfW) in 2011.
