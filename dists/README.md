<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [DIY AVR in the Arduino IDE](#diy-avr-in-the-arduino-ide)
  - [:star: Arduino IDE 1.8.5+ (or 1.6.13) Recommended](#star-arduino-ide-185-or-1613-recommended)
  - [:star: Make sure your "Arduino AVR Boards Built-In" is updated](#star-make-sure-your-arduino-avr-boards-built-in-is-updated)
  - [Supported Chips](#supported-chips)
  - [ATmega x8 Series (8,48,88,168,328)](#atmega-x8-series-84888168328)
  - [ATtiny](#attiny)
    - [ATtiny25, ATtiny45, ATtiny85](#attiny25-attiny45-attiny85)
      - [SPI Pin Inversions](#spi-pin-inversions)
    - [ATtiny24, ATtiny44, ATtiny84](#attiny24-attiny44-attiny84)
      - [SPI Pin Inversions](#spi-pin-inversions-1)
    - [ATtiny13](#attiny13)
      - [SPI / Wire (I2C, TWI) On Tiny13](#spi--wire-i2c-twi-on-tiny13)
    - [ATtiny4, ATtiny5, ATtiny9, ATtiny10](#attiny4-attiny5-attiny9-attiny10)
  - [Installation & Usage](#installation-&-usage)
    - [Automatic Installation through Board Manager (Recommended)](#automatic-installation-through-board-manager-recommended)
    - [Manual Installation (Not Recommended)](#manual-installation-not-recommended)
    - [Usage](#usage)
    - [Tunable Bootloaders](#tunable-bootloaders)
    - [Upload Using Programmer - EEPROM Auto Uploading](#upload-using-programmer---eeprom-auto-uploading)
    - [:boom: Upload Using Programmer - Important Note!](#boom-upload-using-programmer---important-note)
  - [Selectable Options (Tools Menus)](#selectable-options-tools-menus)
    - [Processor Version](#processor-version)
    - [Processor Speed](#processor-speed)
    - [Use Bootloader](#use-bootloader)
    - [Link Time Optimization](#link-time-optimization)
    - [Brown-out Detection Level](#brown-out-detection-level)
    - [Tiny Only Menus](#tiny-only-menus)
      - [Millis, Tone Support](#millis-tone-support)
      - [Millis Accuracy](#millis-accuracy)
      - [Print Support](#print-support)
      - [Serial Support](#serial-support)
    - [Overrides Menus](#overrides-menus)
      - [Override Clock Source](#override-clock-source)
      - [Override Frequency](#override-frequency)
      - [Override Upload Speed](#override-upload-speed)
      - [Override CKOPT (ATMega8 Only)](#override-ckopt-atmega8-only)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

DIY AVR in the Arduino IDE
================================================================================

Included here are a large set of convenient boards.txt and bootloaders to handle
many Atmel AVR Microcontrollers in the Arduino IDE for people making their own
DIY Arduino based stuff.

:warning: Arduino Version 1.0.x support has now ended in this package, 1.0.x 
  users should upgrade to 1.6.x+ without further delay, 
  [download for free here](https://www.arduino.cc/en/Main/Software).

:star: Arduino IDE 1.8.5+ (or 1.6.13) Recommended
--------------------------------------------------------------------------------

The current release was made using IDE version 1.8.5 so that's the recommended, but you should be ok with anything 1.6.13 or newer, and MAYBE older versions.

:boom: ** Do not use Arduino IDE 1.6.10 due to major bugs in that version of the IDE. :boom:  

:star: Make sure your "Arduino AVR Boards Built-In" is updated
--------------------------------------------------------------------------------

It is often a problem that people have updated their IDE version but it is using for some reason an old "Arduino AVR Boards", this can cause a problem (usually with avrdude configuration).

Go to "Tools > Board > Board Manager" and look at the (usually) first item which is called "Arduino AVR Boards", it should show version 1.6.15 or higher installed, note that this number is not the same as your IDE version (usually).

Especially if it is lower than 1.6.15, click the item and select/install the highest available version.

Supported Chips
--------------------------------------------------------------------------------

Presently the list of included chips is as follows:

## ATmega x8 Series (8,48,88,168,328)

[Board Manager JSON for ATmega 8/48/88/168/328](https://raw.githubusercontent.com/sleemanj/optiboot/master/dists/package_gogo_diy_atmega8_series_index.json)

:star: [See Arduino Pinout Diagram (DIP)](https://rawgit.com/sleemanj/ArduinoOrientedChipPinoutCreator/master/328-dip.png) 
:star: [See Arduino Pinout Diagram (SMD-32)](https://rawgit.com/sleemanj/ArduinoOrientedChipPinoutCreator/master/328.png) 
:star: [See Arduino Pinout Diagram (SMD-28)](https://rawgit.com/sleemanj/ArduinoOrientedChipPinoutCreator/master/328-mlf28.png) 

This series of chips are pin compatible in both 28 pin DIP (through hole) and
32 pin surface mount packages.  Consult the datasheets for the specific 
differences, but for most people the biggest difference is simply the amount
of memory.  

 * ATmega8,   ATmega8A,
 * ATmega48,  ATmega48A,  ATmega48P, ATmega48PA, ATmega48PB
 * ATmega88,  ATmega88A,  ATmega88P,  ATmega88PA, ATmega88PB 
 * ATmega168, ATmega168A, ATmega168P, ATmega168PA, ATmega168PB
 * ATmega328, ATmega328P, ATmega328PB
 
Note at current time only 8A, 48P and 328P have been tested, but the others 
should work too, open an issue if not.

Be sure to see the [Selectable Options (Tools Menus)](#selectable-options-tools-menus) which offers a number of ways to configure your chip beyond the defaults.

## ATtiny

[Board Manager JSON for ATtiny 25/45/85/24/44/84/13/10/5](https://raw.githubusercontent.com/sleemanj/optiboot/master/dists/package_gogo_diy_attiny_index.json)

The ATtiny series of chips use (installed automatically for you with the JSON above) a [fork of ATTinyCore](https://github.com/sleemanj/ATTinyCore) which has further optimisations for core code size reduction so you can fit (_considerably_) more useful code into your ATtiny and still maintain all the functions you are used to.

:boom: Because it doesn't use the "official" Arduino core files (which really don't support ATtiny at all properly) there may be some differences you will notice along the way, and some libraries may not be available.  The main issues will be when using the very small ATtiny13, 4, 5, 9 and 10 which have to sacrifice some functionality.

:warning: It is recommended that you only use the `A0`, `A1` ... `An` constants with `analogRead()`.  Doing something like `digitalRead(A0)`, may not do what you want (especially in releases of this core prior to October 2017, after October 2017 it's less of a problem).  Consult the pinout diagrams for each chip and it will show you clearly which numbers to use for each pin!


### ATtiny25, ATtiny45, ATtiny85

:star: [See Arduino Pinout Diagram](https://rawgit.com/sleemanj/ArduinoOrientedChipPinoutCreator/master/x5.png)

 * ATtiny25
 * ATtiny45
 * ATtiny85
 
Note only 85 has been tested, but the others should work too, open an issue if not.

#### SPI Pin Inversions

Note on the pinout diagram above that the MOSI and MISO pins have two "orientations" on the x5 Series, when Programming (Slave) use the Programming orientation, and when using SPI devices connected to your chip (Master) use the SPI orientation.  The reason for this inversion is that the SPI Master implementation uses the "USI" function of the x5, which does not have a hardware SPI system.

### ATtiny24, ATtiny44, ATtiny84

:star: [See Arduino Pinout Diagram](https://rawgit.com/sleemanj/ArduinoOrientedChipPinoutCreator/master/x4.png)

 * ATtiny24, ATtiny24A
 * ATtiny44, ATtiny44A
 * ATtiny84, ATtiny84A
 
Note only 84A has been tested, but the others should work too, open an issue if not.

#### SPI Pin Inversions

Note on the pinout diagram above that the MOSI and MISO pins have two "orientations" on the x4 Series, when Programming (Slave) use the Programming orientation, and when using SPI devices connected to your chip (Master) use the SPI orientation.  The reason for this inversion is that the SPI Master implementation uses the "USI" function of the x4, which does not have a hardware SPI system.

### ATtiny13

:star: [See Arduino Pinout Diagram](https://rawgit.com/sleemanj/ArduinoOrientedChipPinoutCreator/master/13.png)

This series of chips is pin compatible with the ATtinyx5 series, but have only 1K of flash and 64 Bytes of SRAM,  as a result they are a tight squeeze for the Arduino environment.  There is no room for bootloaders so you need an ISP programmer.

 * ATtiny13, ATtiny13A

Remember that although there is no bootloader, you still want to "Burn Bootloader" in order to set the fuses for your chosen clock speed etc.

A number of examples are available in `File > Examples > ATTinyCore > Tiny13` which will help you to get the hang of special considerations about writing code for the Tiny13.

#### SPI / Wire (I2C, TWI) On Tiny13

SPI is supported at least for writing to SPI devices, reading is not tested.  Writing has been tested with an SPI Digital Potentiometer, and there is enough space to use that, at least semi usefully.

I2C/TWI (Wire Library) is not presently supported on the Tiny13, at this stage I don't think there is sufficient space on a Tiny13 to offer a Wire library and actually have any flash left to use it in any way usefully.

### ATtiny4, ATtiny5, ATtiny9, ATtiny10

:star: [See Arduino Pinout Diagram](https://rawgit.com/sleemanj/ArduinoOrientedChipPinoutCreator/master/tiny5%2C10.png)

These chips are extremely small 6 pin chips and use TPI to upload, please read this [extended information about how to use the Tiny4/5/9/10](https://github.com/sleemanj/ATTinyCore/blob/master/avr/variants/tiny5_10/README.md) for more information and troubleshooting.

The 4 and 5 only have 512K of Flash, and all of them only have 32 bytes of SRAM, you can forget about anything to do with Print (that includes Serial) as it's not practically useful in such limitations. But all the basic functions, that is, digitalRead/Write, analogRead/Write and pinMode are good.

The 4 and 9 do not have an ADC, so analogRead() is not available on those chips.

You will find it useful to select `Tools > Millis, Tone Support > No Millis, No Tone` when ever possible on these chips, as that will save a lot of space - you can still use `delay()` even if you don't have `millis()` enabled so it's not entirely useless!

Due to these chips being what Atmel calls "Reduced Core TinyAVR" the compiler can not make the code as efficient on 4/5/9/10, so even code that fits for example in a Tiny13 may not fit in a Tiny10 even though they have the same amount (1024 bytes) of code space.  **Think small!**

Installation & Usage
--------------------------------------------------------------------------------

Note that this has been tested with 1.8.5, older versions your milage may vary.

### Automatic Installation through Board Manager (Recommended)

 * Start the Arduino IDE
 * Select File > Preferences
 * Locate the "Additional Boards Manager URLs" setting and click the button next to it.
 * Add the following URLs to the list (one or more as you desire support for)
 
    https://raw.githubusercontent.com/sleemanj/optiboot/master/dists/package_gogo_diy_atmega8_series_index.json
    https://raw.githubusercontent.com/sleemanj/optiboot/master/dists/package_gogo_diy_attiny_index.json
    
 * Hit OK
 * Select Tools > Board > Boards Manager
 * After a few seconds you will find that the new boards are available for 
   install, select the new Boards and click the Install button
 * Close the Boards Manager window
 
### Manual Installation (Not Recommended)

  * Download the following zip file(s) as you desire
    * [ATmega8 Series (8/48/88/168/328)](diy_atmega8_series_ARDUINO_1_6_x_MANUAL_INSTALL.zip?raw=true)
    * [ATtiny](diy_attiny_ARDUINO_1_6_x_MANUAL_INSTALL.zip?raw=true)
  * Unzip the file(s)
  * Move the folder you unzipped into a folder called "hardware" inside your Arduino Sketchbook (create one if it doesn't exist) - see the README.TXT in the file for full instructions on where to find your sketchbook folder
  * Restart your Arduino IDE
 
### Usage
 
 * Select Tools > Board > [the chip you want to program]
 * Select Tools > Processor Version > [the specific version of the chip]<sup>1</sup>
 * Select Tools > Processor Speed > [the speed to run the chip at]
 * Select Tools > Bootloader > [ if you want/have a bootloader or not ]
 * Select Tools > Link Time Optimisation > [ if you want LTO or not ]
 
If you need to burn a bootloader (if it hasn't already been done on your chip or you want to change the chip settings) or set fuses, you can then use the "Burn Bootloader" option to do so, if you selected to use a non-bootloaded setup, then Burn Bootloader will set the appropriate fuses for that too, so make sure you still do it!

Some chips provide more selectable options, you can read about all the Tools Menu options here...
[Selectable Options (Tools Menus)](#selectable-options-tools-menus)

<sup>1</sup> Not all "boards" have a Processor Version menu.

### Tunable Bootloaders

The 8MHz and 1MHz versions of the bootloader include an "oscillator tuning" function which is active until you first upload a sketch using the bootloader, this is useful because the internal oscillator of AVR chips is not that accurate and a reasonable degree of accuracy is required for reliable serial communication.

You can try uploading a sketch right after burning the bootloader, and if it works, it works, great, you **probably** didn't need to tune your oscillator.

If however you do need to tune it, typically evidenced by not being able to upload, or getting garbage when trying to print stuff to serial, then immediately after burning the bootloader, open a serial monitor at 9600 Baud, and repeatedly send the letter x to it with say half a second between them. `x<enter>x<enter>x<enter>x<enter>...` and so forth. 

After each x you send you should see some, possibly garbage, output from the serial, eventually (12-20 x's) it will start making sense as the oscillator "homes in", and finally it will tell you that it is complete and save the calibation.

Once you get the complete message, you can upload a sketch and work as normal, you don't need to do anything else the calibration data has been saved into the bootloader forever (or until you wipe out the bootloader).

### Upload Using Programmer - EEPROM Auto Uploading

Did you know that you can generate an EEPROM initialisation file right from your code?  Well you can, imagine you have this code...

    #include <avr/eeprom.h>    
    uint8_t MyVariable EEMEM = 42;    
    void setup()  
    {  
      Serial.begin(57600);
    }
    
    void loop()
    {      
      Serial.print("The EEPROM has the number: ");
      Serial.println( eeprom_read_byte(&MyVariable) );
      delay(5000);
    }

GCC will create for you as well as the normal code a file called `sketch_name.eep` in the compilation directory.

Normally however the Arduino IDE doesn't do anything with that file, and your EEPROM will be blanked so when you read it, it will be 255 instead of 42 like you wanted.

However after installing this system you will find that in the Tools > Programmers menu there is a new option for some of the programmers marked as **` + Upload EEPROM (Not For Burn Bootloader)`**, if you select this option and then **Upload Using Programmer** the *.eep file will also be uploaded automatically to initialize your EEPROM to those initial values.

It is important to note that you can not use a `+ Upload EEPROM` programmer selection when you are burning the bootloader, you would get an error, so just switch back to the normal programmer in the menu to burn your bootloaders.

Remember this only is for Upload Using Programmer, normal serial (bootloader/usb) uploading will not upload EEPROM!

This is a super cool feature!  Think about how you can be crafty and stash some stuff in your EEMEM like you would your PROGMEM, mmm, memory.

### :boom: Upload Using Programmer - Important Note!

Currently the below does not apply (because avrdude and gcc in the current Arduino IDE is modern and up to date instead of years out of date like it used to be).

~~Due to [a bug in Arduino](https://github.com/arduino/Arduino/issues/2886) if you Upload Using Programmer you may need to choose an alias of your Programmer from the `Tools > Programmer` menu, you will find there that aliases are present, for example if you normally use "USBAsp" as your programmer and it isn't working then you may need to choose instead one of the aliases, "DIY ATmega: USBAsp" or "DIY ATtiny: USBAsp" (depending if you are programming an ATmega or an ATtiny).~~


## Selectable Options (Tools Menus)

### Processor Version

Some chips have multiple similar versions, for example the ATMega328 and the ATMega328P, this menu allows you to select exactly which chip you are dealing with.

### Processor Speed

This is a simple selection for common speeds to run your chip at.  The options in this menu set the clock type, frequency, and the upload speed for bootloaders (upload baud rate, not the `Serial.begin()` rate which is whatever you choose as usual) so that they all work togethor.

Generally there are at least 3 options, 8MHz Internal Oscillator, choose this if you do not have a crystal.  1MHz Internal Oscillator, choose this if you don't have a crystal and want to save power by running slower.  16MHz Crystal/Resonator, choose this if you have an external crystal.

You must *Burn Bootloader* in order to set the fuses after changing the Processor Speed menu.

:boom: **Caution** if you choose a Crystal/Resonator option and you don't actually HAVE a Crystal/Resonator your chip will not function and can only be fixed by adding a Crystal/Resonator or injecting a clock signal.

### Use Bootloader

For chips where it is supported, decide if you will use the Bootloader (to upload over a serial/usb connection), or not (upload only using the ISP Programmer, eg, USBAsp).  **Even if you choose No Bootloader you must Burn Bootloader to set the fuses.**

### Link Time Optimization

Enables the LTO feature of GCC.  Only disable this if your Arduino IDE version does not support it (prior 1.6.11) or if you think it might be causing a problem, or if you want to disassemble the compiled output to investigate.  LTO provides significant reduction in flash consumption in most cases.

### Brown-out Detection Level

If the BOD is available and enabled, this sets the level at which it will cause the chip to reset.  You must *Burn Bootloader* in order to set the fuses after changing the Brown-out Detection Level menu.

### Tiny Only Menus

The following menus are only present on ATTiny chips.  Not all chips have all the menus, it depends on the functionality.

#### Millis, Tone Support

Allows you to disable `millis()` and `tone()` if you don't need those functions to save some space.

#### Millis Accuracy

The more accurate `millis()` is the more FLASH (Program Storage) memory it uses generally speaking, you can use this menu to sacrifice some accuracy of `millis()` to save memory.

#### Print Support

The more things that `print()` (eg `Serial.print()`) can support, the more memory it uses generally speaking, you can use this menu to sacrifice some functions to save memory.

#### Serial Support

Some chips offer different types of serial support, you can use this menu to select how much support, more support means more memory used.

### Overrides Menus

For **expert** users only some additional menus are provided to override fuses and options which normally would be set to defaults or by the previous menus.   Note that if you use the Overrides menus there is **no sanity checking**, for example there is nothing to stop you choosing "Internal Oscillator 8 MHz" and then picking 16MHz from the Override Frequency, your timing would be all wonked up, but that's your perogative if that's what you chose!

#### Override Clock Source

This gives you finer control over what clock source to use for the chip's main clock.  The most common reason to pick something from this menu would be if you are using a TCXO.  As with *Processor Speed* if you choose a Crystal/Resonator or External Clock option be sure you actually have one or you'll be stuck.

You must *Burn Bootloader* in order to set the fuses after changing the Override Clock Source menu.

#### Override Frequency

This gives you finer control over what frequency (F_CPU) to operate at. 

:boom: **Caution** due to the way that some functions of Arduino work, `millis`, `micros`, `pulseIn` and that kind of thing might or might not work (accurately or at all) if your frequency is less than 1 MHz, furthermore if possible you should ensure your frequency is an integer multiple of 1MHz.

#### Override Upload Speed

When using the Bootloader (not for Upload Using Programmer) the upload speed can be tweaked using this setting.  This does not affect "Serial" or "SoftwareSerial" etc, just uploads using the bootloader.

Generally you should stick with the default here if you are burning a bootloader, the exception would be if you already have a bootloader burned which is using a different rate to the default.

:boom: All frequencies above 1MHz should at least have a *Default* upload speed that has a bootloader file, but anything other than *Default* may or may not have a working bootloader and will produce an error when you Burn Bootloader.  In short, the only time you need to use this menu is if you already have a burned bootloader which has a different upload speed and you don't want to re-burn with *Default* (which will be the fastest recommended).

#### Override CKOPT (ATMega8 Only)

This option is only available on the ATMega8, it allows you to explicityly set or clear the CKOPT fuse.  It's behaviour depends on the clock source.  With Crystal/Resonator it will increase the drive strength, with Internal Oscillators it will do nothing, and with external Clocks or the Low Frequency Crystal option it will enable some internal capacitors so you don't need external ones.