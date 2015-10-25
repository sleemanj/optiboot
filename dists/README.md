DIY AVR in the Arduino for both 1.6.x and 1.0.x IDE
--------------------------------------------------------------------------------

Included here are a large set of convenient boards.txt and bootloaders to handle
many Atmel AVR Microcontrollers in the Arduino IDE for people making their own
DIY Arduino based stuff.

Installing these is a breeze, especially in IDE 1.6.x, but also in 1.0.x (1.0.5 
tested)

Presently the list of included chips is as follows:

## ATmega8 Series

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
should work too, open a support ticket if not.

Installation & Usage
--------------------------------------------------------------------------------

## IDE Version 1.6.x

Note that this has been tested with 1.6.5, older versions your milage may vary.

### Automatic Installation through Board Manager

 * Start the Arduino IDE
 * Select File > Preferences
 * Locate the "Additional Boards Manager URLs" setting and click the button next to it.
 * Add the following URL to the list
 * https://raw.githubusercontent.com/sleemanj/optiboot/master/dists/package_gogo_diy_atmega8_series_index.json
 * Hit OK
 * Select Tools > Board > Boards Manager
 * After a few seconds you will find that the new boards are available for 
   install, select the new Boards and click the Install button
 * Close the Boards Manager window
 
### Manual Installation

  * Download the following zip file
  * https://raw.githubusercontent.com/sleemanj/optiboot/master/dists/diy_atmega8_series_ARDUINO_1_6_x_MANUAL_INSTALL.zip
  * Unzip the file
  * Move the folder you unzipped into a folder called "hardware" inside your Arduino Sketchbook (create one if it doesn't exist) - see the README.TXT in the file for full instructions on where to find your sketchbook folder
  * Restart your Arduino IDE
 
### Usage
 
 * Select Tools > Board > [the chip you want to program]
 * Select Tools > Processor Version > [the specific version of the chip]
 * Select Tools > Processor Speed > [the speed to run the chip at]
 * Select Tools > Bootloader > [ if you want/have a bootloader or not ]
 
If you need to burn a bootloader (if it hasn't already been done on your chip or you want to change the chip settings) or set fuses, you can then use the "Burn Bootloader" option to do so, if you selected to use a non-bootloaded setup, then Burn Bootloader will set the appropriate fuses for that too, so make sure you still do it!

## IDE Version 1.0.x

Note that this has been tested with 1.0.5, older versions your milage may vary.

### Manual Installation

  * Download the following zip file
  * https://raw.githubusercontent.com/sleemanj/optiboot/master/dists/diy_atmega8_series_ARDUINO_1_0_x.zip
  * Unzip the file
  * Move the folder you unzipped into a folder called "hardware" inside your Arduino Sketchbook (create one if it doesn't exist) - see the README.TXT in the file for full instructions on where to find your sketchbook folder
  * Restart your Arduino IDE

See the README.TXT file in the zip file for full install instructions.
  
### Usage

  * Select Tools > Board > [the chip, version, speed and bootloader settings you want]
  
If you need to burn a bootloader (if it hasn't already been done on your chip or you want to change the chip settings) or set fuses, you can then use the "Burn Bootloader" option to do so, if you selected to use a non-bootloaded setup, then Burn Bootloader will set the appropriate fuses for that too, so make sure you still do it!

Due to limitations of the 1.0.x IDE, only the most common chips have been enabled in boards.txt, if the chip you want is not found in the Board menu, you will need to edit the boards.txt you downloaded in the zip file, locate the section for the chip you want, and uncomment it.  