DIY AVR in the Arduino for both 1.6.x and 1.0.x IDE
--------------------------------------------------------------------------------

Included here are a large set of convenient boards.txt and bootloaders to handle
many Atmel AVR Microcontrollers in the Arduino IDE for people making their own
DIY Arduino based stuff.

Installing these is a breeze, especially in IDE 1.6.x, but also in 1.0.x (1.0.5 
tested)

Presently the list of included chips is as follows:

## ATmega8 Series

[IDE 1.6.x Board Manager JSON for ATmega 8/48/88/168/328](https://raw.githubusercontent.com/sleemanj/optiboot/master/dists/package_gogo_diy_atmega8_series_index.json)

This series of chips are pin compatible in both 28 pin DIP (through hole) and
32 pin surface mount packages.  Consult the datasheets for the specific 
differences, but for most people the biggest difference is simply the amount
of memory.  

 * ATmega8,   ATmega8A,
 * ATmega88,  ATmega88A,  ATmega88P,  ATmega88PA, ATmega88PB 
 * ATmega168, ATmega168A, ATmega168P, ATmega168PA, ATmega168PB
 * ATmega328, ATmega328P, ATmega328PB
 * ATmega48,  ATmega48A,  ATmega48P, ATmega48PA, ATmega48PB
 
Note at current time only 8A, 48P and 328P have been tested, but the others 
should work too, open an issue if not.

## ATtiny

[IDE 1.6.x Board Manager JSON for ATtiny 25/45/85/24/44/84/13](https://raw.githubusercontent.com/sleemanj/optiboot/master/dists/package_gogo_diy_attiny_index.json)

### ATtiny x5 Series

 * ATtiny25
 * ATtiny45
 * ATtiny85
 
Note only 85 has been tested, but the others should work too, open an issue if not.

[See Arduino Pinout Diagram](diy_attiny/avr/Pin_Mapping_ATtiny25_45_85.svg)

### ATtiny x4 Series

 * ATtiny24, ATtiny24A
 * ATtiny44, ATtiny44A
 * ATtiny84, ATtiny84A
 
Note only 84A has been tested, but the others should work too, open an issue if not.

[See Arduino Pinout Diagram](diy_attiny/avr/Pin_Mapping_ATtiny24_44_84.svg)

### ATtiny13

This series of chips is pin compatible with the ATtinyx5 series, but have only 1K of flash and 64 Bytes of SRAM,  as a result they are a tight squeeze for the Arduino environment.  There is no room for bootloaders so you need an ISP programmer.  The core used for this series of chips is more limited and you may find that some functions do not operate, for example, the "Serial" interface is somewhat limited (directly speaking, it is HalfDuplex, there is no buffer, it's good-enough for debug output).

 * ATtiny13, ATtiny13A

[See Arduino Pinout Diagram](diy_attiny/avr/Pin_Mapping_ATtiny13.svg)

Remember that although there is no bootloader, you still want to "Burn Bootloader" in order to set the fuses for your chosen clock speed etc.
 
Installation & Usage
--------------------------------------------------------------------------------

## IDE Version 1.6.x

Note that this has been tested with 1.6.5, older versions your milage may vary.

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
 * Select Tools > Processor Version > [the specific version of the chip]
 * Select Tools > Processor Speed > [the speed to run the chip at]
 * Select Tools > Bootloader > [ if you want/have a bootloader or not ]
 
If you need to burn a bootloader (if it hasn't already been done on your chip or you want to change the chip settings) or set fuses, you can then use the "Burn Bootloader" option to do so, if you selected to use a non-bootloaded setup, then Burn Bootloader will set the appropriate fuses for that too, so make sure you still do it!

The ATtiny series also include a "Core Size vs. Capability" menu, using this you can elect to have a smaller core at the expense of losing out on some functionality.  This is useful on the small chips.

## IDE Version 1.0.x

Note that this has been tested with 1.0.5, older versions your milage may vary.

### Manual Installation

  * Download the following zip file(s) as you desire them.
    * [ATmega8 Series (8/48/88/168/328)](diy_atmega8_series_ARDUINO_1_0_x.zip?raw=true)
    * [ATtiny](diy_attiny_ARDUINO_1_0_x.zip?raw=true)
  * Unzip the file(s)
  * Move the folder you unzipped into a folder called "hardware" inside your Arduino Sketchbook (create one if it doesn't exist) - see the README.TXT in the file for full instructions on where to find your sketchbook folder and any special instructions.
  * Restart your Arduino IDE

See the README.TXT file in the zip file for full install instructions.
  
### Usage

  * Select Tools > Board > [the chip, version, speed and bootloader settings you want]
  
If you need to burn a bootloader (if it hasn't already been done on your chip or you want to change the chip settings) or set fuses, you can then use the "Burn Bootloader" option to do so, if you selected to use a non-bootloaded setup, then Burn Bootloader will set the appropriate fuses for that too, so make sure you still do it!

Due to limitations of the 1.0.x IDE, only the most common chips have been enabled in boards.txt, if the chip you want is not found in the Board menu, you will need to edit the boards.txt you downloaded in the zip file, locate the section for the chip you want, and uncomment it.  
