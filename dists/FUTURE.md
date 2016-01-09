Future Plans Not Yet Supported But Possibly One Day
--------------------------------------------------- 

Various notes about potential chips to support.

# ATtiny

## Likely Candidates For Inclusion

  * ATtiny48, ATtiny88? - supported by the tinyX8 core variant, but not sure if worthwhile, seems to be broadly "pin compaible" with megax8 in so far as they can be, or more, not pin incompatible.  So I'd expect most people to just use a megax8 instead of a tinyx8 in arduino land.
  * ATtiny87, ATtiny167 - supported by the tinyX7 core variant.  20pin, similar but not the same pinout as tiny26. Can self program.
  * ATtiny261, ATtiny461, ATtiny861 - supported by the tinyX61 core variant, 20 pin SOIC, 20 PIN QFN/MLF, 32 pin QFN/MLF.  Can self program.
  * [ATtiny2313, ATtiny2313A, ATtiny4313](http://www.atmel.com/images/doc8246.pdf) - supported by tinyX313.  20pin DIP/SOIC/MLF/VQFN.  Can self program
  * [ATtiny4, ATtiny5, ATtiny9, ATtiny10 - SOT-23/UDFN](http://www.atmel.com/images/atmel-8127-avr-8-bit-microcontroller-attiny4-attiny5-attiny9-attiny10_datasheet.pdf) - note these use TPI instead of SPI for programming, [USBasp supposedly supports TPI as well](https://www.google.co.nz/webhp?q=USBasp%20TPI).  These do not support self programming, so no bootloader.  The SOT-23-6 package is very small but hand-solderable, this is probably a good one to support even though there's no self-programming.  The 5 and 10 have ADC, the 4/5 are 500k flash, the 9/10 are 1k
  * [ATtiny43U](http://www.atmel.com/images/doc8048.pdf) - interesting chip with very low power (0.7v) operation with a built in boost converter (needs external inductor).  Can self program.  20pin SOIC/QFN/MLF
  
## Less Likely Candidates For Inclusion

  * [ATtiny11, ATtiny12](http://www.atmel.com/images/1006s.pdf) - the 12 supports ISP, but the 11 does not (high voltage serial programming only), these are deprecated chips so may not be worth putting any time into although the 12 should be easy to enable.  These do not support self programming, so no bootloader.  Basically pin-compatible with x5
  * [ATtiny15](http://www.atmel.com/images/doc1187.pdf) an old precursor to the x5 series, note that PB4 and PB3 are swapped (interesting note, the tiny25 can be put into a special mode to have these swapped to be compatible with 15!).  No self programming I think, so no bootloader.
  * [ATtiny20](http://www.atmel.com/images/atmel-8235-8-bit-avr-microcontroller-attiny20_datasheet.pdf), 14 pin seems same pinout as x4.  Does not support self programming, so no bootloader.
  * [ATtiny22](http://avr.hw.cz/pdf/attiny_22.pdf) - seems to be the same pinout as x5, a link indicates [this has been renamed AT90S2343](http://www.avrfreaks.net/forum/attiny22-datasheet), don't think it can self program.  I think it's probably deprecated in favour of x5 series.
  * [ATtiny26](http://www.atmel.com/images/doc1477.pdf) - 20pin DIP similar to 87/167 with port name changes, 32 MLF, no self programming.
  * ATtiny28 - 28pin dip sort of compatible with tinyx8 with some differences, I'm not really sure there's a lot of point in supporting this, or tinyx8 in general because they are essentially the same packages as megax8, I doubt there's any real use-case in arduino-land for this.
  * [ATtiny40](http://www.atmel.com/images/atmel-8263-8-bit-avr-microcontroller-tinyavr-attiny40_datasheet.pdf) - 20pin soic/qfn, no self programming.

  
  