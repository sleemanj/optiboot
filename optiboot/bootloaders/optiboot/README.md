Optiboot - Small bootloader for Atmel AVR8 Microcontrollers
===============================================================================

This is a fork of Optiboot (https://github.com/Optiboot/optiboot) which has 
been modified in the following ways:

  * Build process cleaned up
  * ATMega48P, 328P and 128P Support Added/Fixed
  * ATMage88P/PA Support Added/Fixed
  * ATTiny25/45/85, 24/44/84 Support Added
  * ATTiny 87/167 Support Added
  * Disable blinkenlights by default (less memory, improved compatability) 
  * Tunable versions for internal oscillators - even for tiny25
  * Some patches from other contributors applied

Building A Single Bootloader
------------------------------------------------------------------------------

Building in this fork is only presently supported if you are on a Linux
based computer system, maybe you can do it on a Mac or Windows with
appropriate tools but those systems are not my thing so you're on your own with
them.

In short to compile a bootloader for a specific chip, do like this for example
to use `make` directly.

    AVR_FREQ=8000000L BAUD_RATE=9600 make attiny85
    
You can add some features if you want...
   
    FEATURES=OSCCAL_PROGMEM,SUPPORT_EEPROM AVR_FREQ=8000000L BAUD_RATE=9600 make attiny85

It is important that you should `make clean` between different sets of options
to ensure that make realises it should do something again...

    make clean
    FEATURES=LED=B2,LED_START_FLASHES=10,LED_OSCCAL_PROGMEM,SUPPORT_EEPROM AVR_FREQ=8000000L BAUD_RATE=9600 make attiny85

If you want to manually specify a reserved amount of flash (amount in bytes)
then you can do so like so...

    make clean
    BOOTLOADER_RESERVED=1024 AVR_FREQ=8000000L BAUD_RATE=9600 make attiny85

you might want to do that if the bootloader gets too large for the defaults
and errors out of the compile with "overlapping sections".

:warning: Do not attempt to use make to build multiple bootloaders at once, 
this will not work how you think it does...

    make clean
    AVR_FREQ=8000000L BAUD_RATE=9600 make attiny85 attiny84
    
Only one would actually get built due to how `make` works, but it won't error.

For building multiple bootloaders, read on...

Using The `makeall` Script For Better Betterness
------------------------------------------------------------------------------

There is an included script, `makeall` which makes compiling bootloaders a bit
easier and produces nicer output...
    
    AVR_FREQ=8000000L BAUD_RATE=9600 ./makeall attiny85

You can provide multiple chips to `makeall` and it will build them all...

    AVR_FREQ=8000000L BAUD_RATE=9600 ./makeall attiny85 attiny84
    
You can provide makefiles to `makeall` and it will build the chips in it...

    AVR_FREQ=8000000L BAUD_RATE=9600 ./makeall Makefile.attinyx5
    
You can provide multiple makefiles, and chips...

    AVR_FREQ=8000000L BAUD_RATE=9600 ./makeall Makefile.attinyx5 Makefile.attinyx4 atmega328
    
You can provide features, just like we did before...

    FEATURES=TUNABLE,SUPPORT_EEPROM AVR_FREQ=8000000L BAUD_RATE=9600 ./makeall Makefile.attinyx5

Available Features
------------------------------------------------------------------------------

All of these features can optionally be provided to make or makeall as shown 
above.

    LED                  : [PORTLETTER][NUM] of the LED, eg, A1, default none
    LED_DATA_FLASH       : Flash the led when programming, default off
    LED_START_FLASHES=nn : Flash the led this many times at start, default 0
    SUPPORT_EEPROM       : Support writing EEPROM through avrdude default off
    TUNABLE              : Enable tunable bootloader generation, default off
    OSCCAL_EEPROM_ADDR   : Read OSCCAL value from this EEPROM address on boot
                           default none
    OSCCAL_PROGMEM       : Read the 3rd-from-end byte of FLASH and use as 
                           OSCCAL value on boot.  Default off, TUNABLE enables
                           this automatically.

Building "Tunable" Bootloaders
------------------------------------------------------------------------------

Some chips (at least ATTiny24/44/84 and ATTIny25/45/85 support "baking in" a 
temporary "Tuning" sketch along with the bootloader.  This is useful (some say
essential) for using the internal oscillator with serial communications.

Simply add the feature "TUNABLE"

    make clean
    FEATURES=TUNABLE AVR_FREQ=8000000L BAUD_RATE=9600 make attiny85

and three output hex files will be produced

  * `optiboot_attiny85_8000000L_9600.hex`
  * `attiny85_8000000L.tuner.hex`
  * `tunable_optiboot_attiny85_8000000L_9600.hex`

The first is just the bootloader hex as would normally be produced.  The
second is a sketch for tuning the oscillator.  The third is both optiboot
and the tuner combined into one hex file which can be burnt to your chip as
a single upload.

The tuner, like any other sketch uploaded, will be over-written as soon as you
upload another sketch so it doesn't take any space (apart from 10 or 20 bytes
of code to support it).

Using The Tuner
------------------------------------------------------------------------------

After building, and burning, your `tunable_optiboot` bootloader, you can...

  * Just try uploading a sketch, if it works, you likely didn't need to tune.
  * Connect via serial at 9600 Baud (that is, open a serial monitor/terminal)
    and repeatedly send the single character "x" with say half a second 
    between them.  After about 12-20 "x" sent you should see a message saying
    that tuning is done.  Now you can upload your sketch as normal.
    
Once you have uploaded a sketch, the tuning program is deleted just like any
other sketch upload, you can of course upload the hex file again manually
or reburn the combined bootloader hex file completely, if you should ever
need to tune again.

Adding New Chips
------------------------------------------------------------------------------

See the `Makefile.attinyx4` or `Makefile.atmega8` files for examples of chip
definition structures for the bootloader parameters, you may also need to 
edit pin_defs.h which defines UART and LED pins as necessary.

Of course you may also need to edit optiboot.c to handle any special cases of
your new chip, but that's left as an exercise for the reader!