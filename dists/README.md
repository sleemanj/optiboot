<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [DIY AVR in the Arduino IDE](#diy-avr-in-the-arduino-ide)
  - [:star: Arduino IDE 1.6.13+ (Probably) Required](#star-arduino-ide-1613-probably-required)
  - [Supported Chips](#supported-chips)
  - [ATmega x8 Series (8,48,88,168,328)](#atmega-x8-series-84888168328)
  - [ATtiny](#attiny)
    - [ATtiny25, ATtiny45, ATtiny85](#attiny25-attiny45-attiny85)
    - [ATtiny24, ATtiny44, ATtiny84](#attiny24-attiny44-attiny84)
    - [ATtiny13](#attiny13)
    - [ATtiny4, ATtiny5, ATtiny9, ATtiny10](#attiny4-attiny5-attiny9-attiny10)
  - [Installation & Usage](#installation-&-usage)
    - [Automatic Installation through Board Manager (Recommended)](#automatic-installation-through-board-manager-recommended)
    - [Manual Installation (Not Recommended)](#manual-installation-not-recommended)
    - [Usage](#usage)
    - [Upload Using Programmer - EEPROM Auto Uploading](#upload-using-programmer---eeprom-auto-uploading)
    - [:boom: Upload Using Programmer - Important Note!](#boom-upload-using-programmer---important-note)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

DIY AVR in the Arduino IDE
================================================================================

Included here are a large set of convenient boards.txt and bootloaders to handle
many Atmel AVR Microcontrollers in the Arduino IDE for people making their own
DIY Arduino based stuff.

:warning: Arduino Version 1.0.x support has now ended in this package, 1.0.x 
  users should upgrade to 1.6.x without further delay, 
  [download for free here](https://www.arduino.cc/en/Main/Software).

:star: Arduino IDE 1.6.13+ Recommended
--------------------------------------------------------------------------------

This distribution now recommends at least Arduino IDE 1.6.13, this is because of the following great things...

  1. IDE 1.6.13+ IDE version now includes a long awaited updated GCC (compiler) and AVRDUDE (uploader)
  2. IDE 1.6.13+ IDE version now has LTO available (Link Time Optimisation) which can radically reduce your code size
 
due to these main advantages, it's just not worth putting a lot of work into keeping backward compatiability with older Arduino IDE versions, since upgrading is something almost everybody is going to want to do!

It MIGHT still work in older IDE versions, I have tested very briefly in 1.6.9 and it seems OK and probably should be in 1.6.11 and 1.6.12 also, but seriously, just upgrade already :+1:

:boom: ** Do not use Arduino IDE 1.6.10 due to major bugs in that version of the IDE. :boom:  

Supported Chips
--------------------------------------------------------------------------------

Presently the list of included chips is as follows:

## ATmega x8 Series (8,48,88,168,328)

[IDE 1.6.x Board Manager JSON for ATmega 8/48/88/168/328](https://raw.githubusercontent.com/sleemanj/optiboot/master/dists/package_gogo_diy_atmega8_series_index.json)

:star: [See Arduino Pinout Diagram (DIP)](https://raw.githubusercontent.com/sleemanj/ArduinoOrientedChipPinoutCreator/master/328-dip.jpg) 
:star: [See Arduino Pinout Diagram (SMD-32)](https://raw.githubusercontent.com/sleemanj/ArduinoOrientedChipPinoutCreator/master/328.jpg) 
:star: [See Arduino Pinout Diagram (SMD-28)](https://raw.githubusercontent.com/sleemanj/ArduinoOrientedChipPinoutCreator/master/328-mlf28.jpg) 

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

## ATtiny

[IDE 1.6.x Board Manager JSON for ATtiny 25/45/85/24/44/84/13/10/5](https://raw.githubusercontent.com/sleemanj/optiboot/master/dists/package_gogo_diy_attiny_index.json)

The ATtiny series of chips use (installed automatically for you with the JSON above) a [fork of ATTinyCore](https://github.com/sleemanj/ATTinyCore) which has further optimisations for core code size reduction so you can fit (_considerably_) more useful code into your ATtiny and still maintain all the functions you are used to.

:boom: Because it doesn't use the "official" Arduino core files (which really don't support ATtiny at all properly) there may be some differences you will notice along the way, and some libraries may not be available.  The main issues will be when using the very small ATtiny13, 4, 5, 9 and 10 which have to sacrifice some functionality.

:warning: It is important that you only use the `A0`, `A1` ... `An` constants with `analogRead()` especially when using ATtiny chips, do **not** do something like `digitalRead(A0)`, that will not do what you want.  Consult the pinout diagrams for each chip and it will show you clearly which numbers to use for each pin!


### ATtiny25, ATtiny45, ATtiny85

:star: [See Arduino Pinout Diagram](https://rawgit.com/sleemanj/ArduinoOrientedChipPinoutCreator/master/x5.jpg)

 * ATtiny25
 * ATtiny45
 * ATtiny85
 
Note only 85 has been tested, but the others should work too, open an issue if not.

### ATtiny24, ATtiny44, ATtiny84

:star: [See Arduino Pinout Diagram](https://rawgit.com/sleemanj/ArduinoOrientedChipPinoutCreator/master/x4.jpg)

 * ATtiny24, ATtiny24A
 * ATtiny44, ATtiny44A
 * ATtiny84, ATtiny84A
 
Note only 84A has been tested, but the others should work too, open an issue if not.

### ATtiny13

:star: [See Arduino Pinout Diagram](https://rawgit.com/sleemanj/ArduinoOrientedChipPinoutCreator/master/13.jpg)

This series of chips is pin compatible with the ATtinyx5 series, but have only 1K of flash and 64 Bytes of SRAM,  as a result they are a tight squeeze for the Arduino environment.  There is no room for bootloaders so you need an ISP programmer.

 * ATtiny13, ATtiny13A

Remember that although there is no bootloader, you still want to "Burn Bootloader" in order to set the fuses for your chosen clock speed etc.

A number of examples are available in `File > Examples > ATTinyCore > Tiny13` which will help you to get the hang of special considerations about writing code for the Tiny13.

### ATtiny4, ATtiny5, ATtiny9, ATtiny10

:star: [See Arduino Pinout Diagram](https://raw.githubusercontent.com/sleemanj/ArduinoOrientedChipPinoutCreator/master/tiny5%2C10.jpg)

These chips are extremely small 6 pin chips and work a bit differently, please read this [extended information about how to use the Tiny4/5/9/10](https://github.com/sleemanj/ATTinyCore/blob/master/avr/variants/tiny5_10/README.md) as it does require some extra setup.

The 4 and 5 only have 512K of Flash, and all of them only have 32 bytes of SRAM, you can forget about anything to do with Print (that includes Serial) as it's not practically useful in such limitations. But all the basic functions, that is, digitalRead/Write, analogRead/Write and pinMode are good.

The 4 and 9 do not have an ADC, so analogRead() is not available on those chips.

You will find it useful to select `Tools > Millis, Tone Support > No Millis, No Tone` when ever possible on these chips, as that will save a lot of space - you can still use `delay()` even if you don't have `millis()` enabled so it's not entirely useless!

Due to these chips being what Atmel calls "Reduced Core TinyAVR" the compiler can not make the code as efficient on 4/5/9/10, so even code that fits for example in a Tiny13 may not fit in a Tiny10 even though they have the same amount (1024 bytes) of code space.  **Think small!**

Installation & Usage
--------------------------------------------------------------------------------

Note that this has been tested with 1.6.9, older versions your milage may vary.

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

The ATtiny series also include additional menus under the Tools which allow you to trade-off certain features for more code space.

<sup>1</sup> Not all "boards" have a Processor Version menu.

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

