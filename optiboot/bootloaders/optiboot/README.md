<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**  *generated with [DocToc](https://github.com/thlorenz/doctoc)*

- [Optiboot - Small bootloader for Atmel AVR8 Microcontrollers](#optiboot---small-bootloader-for-atmel-avr8-microcontrollers)
  - [Building A Single Bootloader](#building-a-single-bootloader)
  - [Using The `makeall` Script For Better Betterness](#using-the-makeall-script-for-better-betterness)
  - [Available Features](#available-features)
  - [Building "Tunable" Bootloaders](#building-tunable-bootloaders)
  - [Using The Tuner](#using-the-tuner)
  - [Adding New Chips](#adding-new-chips)
  - [Burning Bootloaders (Picking Fuses)](#burning-bootloaders-picking-fuses)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

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

In general, the structure of a `Makefile.[family]` file is this...

    _family_: [extra common family stuff 1...]
    _family_: ...
    _family_: [extra common family stuff n...]
    _family_: _optiboot_
    
    specific: TARGET     = specific
    specific: FLASH_SIZE = 1234
    specific: FEATURES   += [...] 
    specific: [extra specific stuff 1...]
    (...)
    specific: [extra specific stuff n...]
    specific: _family_

Where `FLASH_SIZE` is the total size of the chip in bytes, decimal, and 
`TARGET` is just a repeat of the target name.  Generally speaking "family"
stuff will override "specific" stuff because of how make works, except for 
+= in which case it will combine, so in other words, only put things that 
absolutely apply to everything in the family in the family sections.

Note that `_family_` and `specific` are not literally that, substitute
with appropriate names, eg `_atmega8_` for `_family_` and `atmega328p` for 
`specific`.  The `_optiboot_` however is literal, this is the Make target
in `Makefile` which actually starts the build process.
    
The "stuff" in question is a variety of configurations which can be set...

    MCU_TARGET	     = target_for_gcc
      
      The `TARGET` (name of the chip, eg attiny24a) is typically what you pass
      to avrdude, however avr-gcc sometimes doesn't know about that name, and 
      you have to pretend it's something else for avr-gcc 
      (for example `attiny24`).  Defaults to same as TARGET.
      
    FEATURES        += [ list of features ]
      
      The features to enable, see the available features above, ones specified 
      here will ALWAYS be set for the chip, so generally you will only use the 
      following features here...
      
      SOFT_UART              : Use software serial (for optiboot)
      VIRTUAL_BOOT_PARTITION : Use a "virtual" bootloader partition
      
      Defaults to no features.
      
    RESET_VECTOR     = nn
    
      When using a VIRTUAL_BOOT_PARTITION the bootloader "hijacks" this decimal
      0-based index interrupt vector to use for virtual boot, this vector will
      no longer be usable for uploaded sketches.  Good candidates are SPM_READY 
      or EE_READY (sometimes SPM_RDY or EE_RDY), not all chips have the 
      SPM_R(EA)DY interrupt so use EE_R(EA)DY in that case.  Note that the 
      datasheet lists vectors starting from 1, we index them starting from ZERO
      so subtract one from the datasheet index.
      
      No default, this is required if you specify VIRTUAL_BOOT_PARTITION feature.
      
    BOOTLOADER_RESERVED = nnnn
      
      The space to reserve (at top of memory) for the bootloader, including 4
      extra bytes (3, but due to word addressing...), in bytes, decimal.  For 
      chips with dedicated boot sections of flash (eg ATMega328) this should 
      match that dedicated area size (depending on Fuse settings).  For 
      VIRTUAL_BOOT_PARTITION this can be an arbitrary size but should always be
      even (because word addressing).
      
      Without VIRTUAL_BOOT_PARTITION this option is REQUIRED
      
      With VIRTUAL_BOOT_PARTITION this option defaults to either 700 or 600 bytes
      depending on if the given features look like they would make a  "big" 
      bootloader or not.

In addition to those there are some "stuffs" which you should never need to 
set, but for completeness I will describe...

    BOOTLOADER_ADDRESS = nnnn
    
      This is the byte address of the first byte of the bootloader code.
      By default this is calculated from the FLASH_SIZE and BOOTLOADER_RESERVED
      
    VERSION_ADDRESS = nnnn
    
      This is the byte address of the first byte of the version bytes (2 of).
      By default this is calculated from the FLASH_SIZE
      
    CAL_ADDRESS = nnnn
    
      This is the byte address of the OSCCAL Calibration Byte when using 
      TUNABLE or OSCCAL_PROGMEM options. By default this is calculated from the
      FLASH_SIZE

Burning Bootloaders (Picking Fuses)
------------------------------------------------------------------------------
Burning a bootloader is typically done with avrdude, a command line such as...

    avrdude -c usbasp -p atmega328 \
    -U flash:w:tunable_optiboot_atmega328_8000000L_38400.hex:i  \
    -U lfuse:w:0x??:m \
    -U hfuse:w:0x??:m \
    -U efuse:w:0x??:m

Where `??` is a value for each fo the low, high and extended fuses (some 
chips don't have high or extended fuses at all in which case you would omit
those parts of the command entirely of course).

You must be very careful when setting fuse values, if you get it wrong you 
brick your chip which can be recovered in the best case with injecting a 
square wave, and in the worst case with using an appropriate high voltage 
programmer.

To calculate fuses, use an AVR Fuse Calculator, I tpically use 

    http://www.engbedded.com/fusecalc/

the options look daunting, but it's not too complicated really, here are
general tips, not all chips have all options so if you can't find it skip 
that, the particularly dangerous ones I have highlighted below.

  1. First select your chip in the AVR part name drop down.
  2. Find **"Reset Disabled"** make sure it is **not ticked**.
  3. Find **"Serial Program downloading"** make sure that it **is ticked**
  4. Find **"Debug Wire"** make sure it is **not ticked**.
  5. Find "Boot Reset vector", make sure it is ticked.
  6. Find "Boot Flash" (dropdown), choose the section size in words which 
  is appropriate (1 Word = 2 Bytes) for the bootloader generated, typically 
  this is 256 Words (512 Bytes) or 512 Words (1024 Bytes).
  7. Find "Watch-dog Timer always on" make sure it is not ticked.
  Unless you know what you are doing.
  8. Find "Preserve EEPROM memory", tick it or not as you desire.
  9. Find "Brown-out detection" (dropdown), choose as you desire.
  Brown-outs are when the voltage drops below the selected value, it will
  cause the chip to RESET when it does (rather than potentially start to
  misbehave).
  10. Find "Clock output", make sure it is not ticked.
  Unless you know what you are doing.
  
Now you need to choose the oscillator frequency from the drop-down which is
appropriate for your chip, it is important to get this right or it's bricked.
Here are some suggestions...

  * Higher than 8MHz - look for an "External Crystal" or "External Crystal Osc."
  option, usually there are multiple speed ranges, the highest likely 
  covers everything above 8MHz The startup time  is not that important, making 
  it longer to start is done to allow your power to stabilise.  Ensure that 
  "Divide click by 8" is not ticked.  :warning: If you select an external ("Ext.")
  option, you MUST provide that timing, you will not be able to program the 
  chip again without it, not even to change it back to the internal oscillator.
  
  * 8Mhz - look for an "Int RC Osc 8MHz" option.  The startup time 
  is not that important, making it longer to start is done to allow your 
  power to stabilise.  Ensure that "Divide click by 8" is not ticked.
  
  * 1Mhz - look for an "Int RC Osc 8MHz" option.  The startup time 
  is not that important, making it longer to start is done to allow your 
  power to stabilise.  Ensure that "Divide click by 8" is ticked.
   


