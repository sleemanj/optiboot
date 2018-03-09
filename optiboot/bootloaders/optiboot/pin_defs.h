/*
 * pin_defs.h
 * optiboot helper defining the default pin assignments (LED, SOFT_UART)
 * for the various chips that are supported.  This also has some ugly macros
 * for selecting among various UARTs and LED possibilities using command-line
 * defines like "UART=2 LED=B5"
 *
 * Copyright 2013-2015 by Bill Westfield.
 * Copyright 2010 by Peter Knight.
 * This software is licensed under version 2 of the Gnu Public Licence.
 * See optiboot.c for details.
 */

/*------------------------------------------------------------------------ */
#if    defined(__AVR_ATmega8__)   || defined(__AVR_ATmega8A__) \
    || defined(__AVR_ATmega48__)  || defined(__AVR_ATmega48A__)  || defined(__AVR_ATmega48P__)  || defined(__AVR_ATmega48PA__) || defined(__AVR_ATmega48PB__) \
    || defined(__AVR_ATmega88__)  || defined(__AVR_ATmega88A__)  || defined(__AVR_ATmega88P__)  || defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega88PB__)  \
    || defined(__AVR_ATmega168__) || defined(__AVR_ATmega168A__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega168PA__) \
    || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
/*------------------------------------------------------------------------ */

  // Denote these chips as ATMEGA8 series compatible
  //  sure would ne nice if Atmel has nice "family" names we could use for 
  //  all the pin compatible devices :-/
  #define __OPTI_ATMEGA8_COMPAT__ 1
  
  /* Onboard LED is connected to pin PB5 in Arduino NG, Diecimila, and Duemilanove
  */ 
  #if !defined(LED)
    #define LED B5
  #endif

  /* Defaults for soft UART
   *  james@gogo.co.nz: The original defaults were PD0 and PD1, but that doesn't really make
   *    much sense because PD0 and PD1 are the hardware uart, so you wouldn't need software
   *    uart on those.  So I have moved the default software uart for this chip series to
   *    PB0 and PB1  (correspond to Digiatl Pin 8 and Digital Pin 9 in arduino-speak)  
   */
  #ifdef SOFT_UART
    #ifndef UART_PORT
      #define UART_PORT   PORTB     
      #define UART_PIN    PINB
      #define UART_DDR    DDRB
      #define UART_RX_BIT 0
      #define UART_TX_BIT 1  
    #endif  
  #else
    /* Even though we use a hardware uart, for tinyTuner specify these as well. */
    #ifndef UART_PORT
      #define UART_PORT   PORTD     
      #define UART_PIN    PIND
      #define UART_DDR    DDRD
      #define UART_RX_BIT 0
      #define UART_TX_BIT 1  
    #endif  
  #endif
  
  // These specific types of this family can toggle a pin by writing a 1
  // to the appropriate bit of the appropriate PIN register, instead of 
  // doing an xor on the PORT register.
  //
  // To locate if your chip can do this, search the datasheet for
  // "toggling the pin", if it finds nothing, it doesn't support it
  // (I don't know of any comprehensive database of this capability)
  
  #if    defined(__AVR_ATmega48PA__) || defined(__AVR_ATmega48PB__)   \
      || defined(__AVR_ATmega88A__)  || defined(__AVR_ATmega88PA__)   \
      || defined(__AVR_ATmega168A__) ||  defined(__AVR_ATmega168PA__) \
      || defined(__AVR_ATmega328P__)
    
    #define TOGGLE_BY_PIN_REGISTER 1
  
  #endif
#endif


/** The following chips are pin compatible.  Note just because they are defined here doesn't mean that
 *  avr-libc or avr-gcc know about them all, yet, but if/when they do, so will we.
 * 
 */
#if     defined (__AVR_ATmega32__)  || defined (__AVR_ATmega32A__) \
     || defined (__AVR_ATmega16__)  || defined (__AVR_ATmega16A__) \
     || defined (__AVR_ATmega164__) || defined (__AVR_ATmega164A__) || defined (__AVR_ATmega164P__) || defined (__AVR_ATmega164PA__) \
     || defined (__AVR_ATmega324__) || defined (__AVR_ATmega324A__) || defined (__AVR_ATmega324P__) || defined (__AVR_ATmega324PA__) \
     || defined (__AVR_ATmega644__) || defined (__AVR_ATmega644A__) || defined (__AVR_ATmega644P__) || defined (__AVR_ATmega644PA__) \
     || defined (__AVR_ATmega1284__) || defined (__AVR_ATmega1284P__)      
     
  // Denote these chips as ATmega644 Compatible  (the Sanguino uses 644, hence why choosing it)
  //  sure would ne nice if Atmel has nice "family" names we could use for 
  //  all the pin compatible devices :-/
  #define __OPTI_ATMEGA644_COMPAT__ 1
    
  /*------------------------------------------------------------------------ */
  /* Onboard LED is connected to pin PB0 on Sanguino */ 
  // @NOTE Sanguino probably most common "board" using this series, so this
  //  seems a reasonable default.
  #if !defined(LED)
  #define LED         B0
  #endif

  /* Defaults for soft UART
   *  james@gogo.co.nz: The original defaults were PD0 and PD1, but that doesn't really make
   *    much sense because PD0 and PD1 are the hardware uart, so you wouldn't need software
   *    uart on those.  So I have moved the default software uart for this chip series to
   *    PD6 and PD7  (correspond to Digiatl Pin 14 and Digital Pin 15 in arduino-speak)  
   */
  #ifdef SOFT_UART
    #ifndef UART_PORT
      #define UART_PORT   PORTD     
      #define UART_PIN    PIND
      #define UART_DDR    DDRD
      #define UART_RX_BIT 0
      #define UART_TX_BIT 1  
    #endif  
  #endif
  
  // These specific types of this family can toggle a pin by writing a 1
  // to the appropriate bit of the appropriate PIN register, instead of 
  // doing an xor on the PORT register.
  //
  // To locate if your chip can do this, search the datasheet for
  // "toggling the pin", if it finds nothing, it doesn't support it
  // (I don't know of any comprehensive database of this capability)
  
  #if    defined (__AVR_ATmega164A__) || defined (__AVR_ATmega164PA__) \
      || defined (__AVR_ATmega324A__) || defined (__AVR_ATmega324PA__) \
      || defined (__AVR_ATmega644A__) || defined (__AVR_ATmega644PA__) \
      || defined (__AVR_ATmega1284__) || defined (__AVR_ATmega1284P__)
    
    #define TOGGLE_BY_PIN_REGISTER 1
  
  #endif

#endif
     
#if    defined(__AVR_ATmega640__) \
    || defined (__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) \
    || defined (__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) 
  
  #define __OPTI_ATMEGA2560_COMPAT__ 1

  // Ardiuno Mega uses 2560, it has LED on PB7
  #if !defined(LED)
  #define LED         B7
  #endif

  // On the Arduino Mega the below defaults are "Digital Pin 10 for RX and Digital Pin 11
  // for TX
  #ifdef SOFT_UART
    #ifndef UART_PORT
      #define UART_PORT   PORTB     
      #define UART_PIN    PINB
      #define UART_DDR    DDRB
      #define UART_RX_BIT 4
      #define UART_TX_BIT 5  
    #endif  
  #endif  
  
  // These specific types of this family can toggle a pin by writing a 1
  // to the appropriate bit of the appropriate PIN register, instead of 
  // doing an xor on the PORT register.
  //
  // To locate if your chip can do this, search the datasheet for
  // "toggling the pin", if it finds nothing, it doesn't support it
  // (I don't know of any comprehensive database of this capability)
  // 
  // (For the 2560 series, it seems to be all the chips can support it)
  #define TOGGLE_BY_PIN_REGISTER 1
  
#endif  
     

#if    defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny84A__) \
    || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny44A__) \
    || defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny24A__)

  // Apparently something called "Luminet" uses A4 for an LED, but as below
  // we want to use that for our soft uart RX, I'm not sure how well this 
  // will work
  //
  // To be honest, I'm not so crazy about this whole flashing the led from
  // a bootloader idea, toggling a pin without knowing what's conected to
  // it seems like not a good idea to me.

  #if !defined(LED)
    #define LED         A4
  #endif
  
  // This series of chips does not have hardware UART, best we can do
  // is software UART.
  //
  // The TinySoftwareSerial code used in the ATTiny core
  //  https://github.com/TCWORLD/ATTinyCore/tree/master/tiny/cores/tiny
  // uses the analog comparator interrupt for the serial handling
  //
  // and so we need to use AIN0 as TX (PA.1)
  // and AIN1 as RX (PA.2)
  
  #define SOFT_UART 1
  #ifndef UART_PORT
    #define UART_PORT   PORTA     
    #define UART_PIN    PINA
    #define UART_DDR    DDRA
    #define UART_RX_BIT 2
    #define UART_TX_BIT 1
  #endif  

    
  // These specific types of this family can toggle a pin by writing a 1
  // to the appropriate bit of the appropriate PIN register, instead of 
  // doing an xor on the PORT register.
  //
  // To locate if your chip can do this, search the datasheet for
  // "toggling the pin", if it finds nothing, it doesn't support it
  // (I don't know of any comprehensive database of this capability)
  
  // (For the TinyX4 series, it seems to be all the chips can support it)
  #define TOGGLE_BY_PIN_REGISTER 1
  
#endif

#if    defined(__AVR_ATtiny85__) \
    || defined(__AVR_ATtiny45__) \
    || defined(__AVR_ATtiny25__)

  // We only have 1 timer on this series, and pins are so limited, 
  // so we won't support having a flashing LED On this series of chip
  #ifdef LED_START_FLASHES
    #undef LED_START_FLASHES
    #define LED_START_FLASHES 0
  #endif
  
  #ifdef LED
    #warning "Sorry, the ATtiny25/45/85 do not support a flashing LED."
    #undef LED
  #endif
  
  
  // This series of chips does not have hardware UART, best we can do
  // is software UART.
  //
  // The TinySoftwareSerial code used in the ATTiny core
  //  https://github.com/TCWORLD/ATTinyCore/tree/master/tiny/cores/tiny
  // uses the analog comparator interrupt for the serial handling
  // and so we need to use AIN0 as TX (which corresponds to MOSI)
  // and AIN1 as RX (which corresponds to MISO)  
  
  #define SOFT_UART 1
  #ifndef UART_PORT
    #define UART_PORT   PORTB     
    #define UART_PIN    PINB
    #define UART_DDR    DDRB
    
    // TX = MOSI (PB.0, Pin 5 of IC), RX = MISO (PB.1, Pin 6 of IC)
    #define UART_RX_BIT 1
    #define UART_TX_BIT 0  
  #endif  

    
  // These specific types of this family can toggle a pin by writing a 1
  // to the appropriate bit of the appropriate PIN register, instead of 
  // doing an xor on the PORT register.
  //
  // To locate if your chip can do this, search the datasheet for
  // "toggling the pin", if it finds nothing, it doesn't support it
  // (I don't know of any comprehensive database of this capability)
  
  // (For the TinyX5 series, it seems to be all the chips can support it)
  #define TOGGLE_BY_PIN_REGISTER 1
  
#endif

#if    defined(__AVR_ATtiny87__) \
    || defined(__AVR_ATtiny167__) 

    
  // We don't define an LED pin for this chip, I'm not aware of any 
  // "standard boards" that use this chipset and so there's no sensible 
  // default.  
  
  // Remember you can specify behaviour when compiling.
  // For example ( with led on PORTA.4 at 8MHz and 9600 Baud ):
  //
  //  AVR_FREQ=8000000L BAUD_RATE=9600 LED=A4 LED_START_FLASHES=4 make attiny167
     
  // This series of chips has a different type of hardware UART, which would
  // be a paint to implement, so we will just use Software Uart.
  //
  // However we will use the hardware "LIN/UART" RX and TX pins to do so.  
  #define SOFT_UART 1
  #ifndef UART_PORT
    #define UART_PORT   PORTA     
    #define UART_PIN    PINA
    #define UART_DDR    DDRA
    
    // TX = MOSI (PB.0, Pin 5 of IC), RX = MISO (PB.1, Pin 6 of IC)
    #define UART_RX_BIT 0
    #define UART_TX_BIT 1  
  #endif  

    
  // These specific types of this family can toggle a pin by writing a 1
  // to the appropriate bit of the appropriate PIN register, instead of 
  // doing an xor on the PORT register.
  //
  // To locate if your chip can do this, search the datasheet for
  // "toggling the pin", if it finds nothing, it doesn't support it
  // (I don't know of any comprehensive database of this capability)
  
  // (For the Tinyx7 series, it seems to be all the chips can support it)
  #define TOGGLE_BY_PIN_REGISTER 1
  
#endif

#if    defined(__AVR_ATtiny841__) \
    || defined(__AVR_ATtiny441__) 

  // Apparently something called "Luminet" uses A4 for an LED, so that's a 
  // reasonable default, I suppose.
  //
  // To be honest, I'm not so crazy about this whole flashing the led from
  // a bootloader idea, toggling a pin without knowing what's conected to
  // it seems like not a good idea to me.

  #if !defined(LED)
    #define LED         A4
  #endif
  
  // This series of chips does not have hardware UART, but the very similar
  // 441/841 do have two hardware uarts, UART0 on those chips however
  // is split over port B and port A (seriously Atmel, seriously)
  // so we use UART1 which is on PA4 (RX) and PA5 (TX)
  
  #define SOFT_UART 1
  #ifndef UART_PORT
    #define UART_PORT   PORTA     
    #define UART_PIN    PINA
    #define UART_DDR    DDRA
    #define UART_RX_BIT 4
    #define UART_TX_BIT 5  
  #endif  

  // These specific types of this family can toggle a pin by writing a 1
  // to the appropriate bit of the appropriate PIN register, instead of 
  // doing an xor on the PORT register.
  //
  // To locate if your chip can do this, search the datasheet for
  // "toggling the pin", if it finds nothing, it doesn't support it
  // (I don't know of any comprehensive database of this capability)
  
  // (For the TinyX41 series, it seems to be all the chips can support it)
  #define TOGGLE_BY_PIN_REGISTER 1
  
#endif

// =============================================================================
// =============================================================================
// =============================================================================
// =============================================================================
//
// FINISH OF TYPICAL CONFIGURATION OPTIONS 
//  
// The rest of this file is implementational details.
//
// =============================================================================
// =============================================================================
// =============================================================================
// =============================================================================
     
     
// Some (older) devices have differently named registers for the UART
// (but equivalent function) so we will rename them for consistency
// in optiboot.c
//
// This is only going to be the case when there is a single UART.
//
// For brevity we assume if one is not set they all are not set

#if defined(UCSRA)  && !defined(UCSR0A)
  #define UCSR0A  UCSRA
  #define UCSR0B  UCSRB
  #define UCSR0C  UCSRC
  #define UDR0    UDR
  #define UDRE0   UDRE  
  #define RXC0    RXC
  #define FE0     FE  
  #define UBRR0L  UBRRL
  #define U2X0    U2X
  #define RXEN0   RXEN
  #define TXEN0   TXEN
  #define UCSZ00  UCSZ0
  #define UCSZ01  UCSZ1  
#endif

#if !defined(TIFR1) && defined(TIFR)
  #define TIFR1 TIFR
#endif

// I'm not sure if these two WDT renamings are entirely kosher.
#if !defined(WDTCSR) && defined(WDTCR)
  #define WDTCSR WDTCR
#endif

#if !defined(WDCE) && defined(WDTOE)
  #define WDCE    WDTOE
#endif

/*
 * This may or may not be necessary, looking at the files in /usr/lib/avr/include/avr/io*.h
 * some of them do not define WDE, but it appears to be universally standard that
 * it is but 3 of the WDTCSR (WDTCR), I'm not sure why I didn't notice any compile
 * errors before (maybe I didn't try)
 */

#if !defined(WDE)
  #define WDE 3
#endif

// A few chips call the MCUSR "MCUCSR" instead
//  some of them already alias it (eg mega8) in avr-libc
#if !defined(MCUSR) && defined(MCUCSR)
  #define MCUSR MCUCSR
#endif

/*
 * Handle devices with up to 4 uarts (eg m1280.)  Rather inelegantly.
 * Note that mega8/m32 still needs special handling, because ubrr is handled
 * differently.
 */
#if UART == 0
# define UART_SRA UCSR0A
# define UART_SRB UCSR0B
# define UART_SRC UCSR0C
# define UART_SRL UBRR0L
# define UART_SRH UBRR0H
# define UART_UDR UDR0
# define UART_DRE UDRE0
# define UART_U2X U2X0
# define UART_RXEN RXEN0
# define UART_TXEN TXEN0
# define UART_UCSZ0 UCSZ00
# define UART_UCSZ1 UCSZ01
# define UART_UCSZ2 UCSZ02
#elif UART == 1
#if !defined(UDR1)
#error UART == 1, but no UART1 on device
#endif
# define UART_SRA UCSR1A
# define UART_SRB UCSR1B
# define UART_SRC UCSR1C
# define UART_SRL UBRR1L
# define UART_SRH UBRR1H
# define UART_UDR UDR1
# define UART_DRE UDRE1
# define UART_U2X U2X1
# define UART_RXEN RXEN1
# define UART_TXEN TXEN1
# define UART_UCSZ0 UCSZ10
# define UART_UCSZ1 UCSZ11
# define UART_UCSZ2 UCSZ12
#elif UART == 2
#if !defined(UDR2)
#error UART == 2, but no UART2 on device
#endif
# define UART_SRA UCSR2A
# define UART_SRB UCSR2B
# define UART_SRC UCSR2C
# define UART_SRL UBRR2L
# define UART_SRH UBRR2H
# define UART_UDR UDR2
# define UART_DRE UDRE2
# define UART_U2X U2X2
# define UART_RXEN RXEN2
# define UART_TXEN TXEN2
# define UART_UCSZ0 UCSZ20
# define UART_UCSZ1 UCSZ21
# define UART_UCSZ2 UCSZ22
#elif UART == 3
#if !defined(UDR3)
#error UART == 3, but no UART3 on device
#endif
# define UART_SRA UCSR3A
# define UART_SRB UCSR3B
# define UART_SRC UCSR3C
# define UART_SRL UBRR3L
# define UART_SRH UBRR3H
# define UART_UDR UDR3
# define UART_DRE UDRE3
# define UART_U2X U2X3
# define UART_RXEN RXEN3
# define UART_TXEN TXEN3
# define UART_UCSZ0 UCSZ30
# define UART_UCSZ1 UCSZ31
# define UART_UCSZ2 UCSZ32
#endif

/* @NOTE Set to PORTx PINx and DDRx where X is the port, err, letter you want
 *    to use.  For those not familiar, PORTx = output state, PINx = input state,
 *    and DDRx = pin direction (read/write).
 * 
 * INPUT:         DDRx.Y=0, PORTx.Y=1, and read your result from PINx.Y
 * INPUT_PULLUP:  DDRx.Y=0, PORTx.Y=0, and read your result from PINx.Y
 * OUTPUT HIGH:   DDRx.Y=1, PORTx.Y=1
 * OUTPUT LOW:    DDRx.Y=1, PORTx.Y=0,
 * 
 * UART_TX_BIT and RX_BIT are the bit numbers in the PORTx to use
 * In this case for the ATtiny24/44/84/441/841 (note there is also 
 * ATtiny20 with the same pinout, but it can't self program so bootloader
 * is not possible on it)
 * 
 *   PORTA.2 = TX = Pin 11 on DIP/SOIC, Pin 3 on QFN/MLF
 *   PORTA.3 = RX = Pin 10 on DIP/SOIC, Pin 2 on QFN/MLF
 * 
 */

#ifdef SOFT_UART  
  #if ! ( defined(UART_PORT) && defined(UART_PIN) && defined(UART_DDR) && defined(UART_TX_BIT) && defined(UART_RX_BIT) )
     #error "SOFT_UART requested but not fully defined, you must set UART_PORT/_PIN/_DDR/_TX_BIT/_RX_BIT"    
  #endif  
#endif

/*
 * ------------------------------------------------------------------------
 * A bunch of macros to enable the LED to be specifed as "B5" for bit 5 
 * of port B, and similar.
 */

#define A0 0x100
#define A1 0x101
#define A2 0x102
#define A3 0x103
#define A4 0x104
#define A5 0x105
#define A6 0x106
#define A7 0x107

#define B0 0x200
#define B1 0x201
#define B2 0x202
#define B3 0x203
#define B4 0x204
#define B5 0x205
#define B6 0x206
#define B7 0x207

#define C0 0x300
#define C1 0x301
#define C2 0x302
#define C3 0x303
#define C4 0x304
#define C5 0x305
#define C6 0x306
#define C7 0x307

#define D0 0x400
#define D1 0x401
#define D2 0x402
#define D3 0x403
#define D4 0x404
#define D5 0x405
#define D6 0x406
#define D7 0x407

#define E0 0x500
#define E1 0x501
#define E2 0x502
#define E3 0x503
#define E4 0x504
#define E5 0x505
#define E6 0x506
#define E7 0x507

#define F0 0x600
#define F1 0x601
#define F2 0x602
#define F3 0x603
#define F4 0x604
#define F5 0x605
#define F6 0x606
#define F7 0x607

#define G0 0x700
#define G1 0x701
#define G2 0x702
#define G3 0x703
#define G4 0x704
#define G5 0x705
#define G6 0x706
#define G7 0x707

#define H0 0x800
#define H1 0x801
#define H2 0x802
#define H3 0x803
#define H4 0x804
#define H5 0x805
#define H6 0x806
#define H7 0x807

#define J0 0xA00
#define J1 0xA01
#define J2 0xA02
#define J3 0xA03
#define J4 0xA04
#define J5 0xA05
#define J6 0xA06
#define J7 0xA07

#define K0 0xB00
#define K1 0xB01
#define K2 0xB02
#define K3 0xB03
#define K4 0xB04
#define K5 0xB05
#define K6 0xB06
#define K7 0xB07

#define L0 0xC00
#define L1 0xC01
#define L2 0xC02
#define L3 0xC03
#define L4 0xC04
#define L5 0xC05
#define L6 0xC06
#define L7 0xC07


#ifndef LED
  // NOP
#elif LED == B0
#undef LED
#define LED_DDR     DDRB
#define LED_PORT    PORTB
#define LED_PIN     PINB
#define LED	    PINB0
#elif LED == B1
#undef LED
#define LED_DDR     DDRB
#define LED_PORT    PORTB
#define LED_PIN     PINB
#define LED	    PINB1
#elif LED == B2
#undef LED
#define LED_DDR     DDRB
#define LED_PORT    PORTB
#define LED_PIN     PINB
#define LED	    PINB2
#elif LED == B3
#undef LED
#define LED_DDR     DDRB
#define LED_PORT    PORTB
#define LED_PIN     PINB
#define LED	    PINB3
#elif LED == B4
#undef LED
#define LED_DDR     DDRB
#define LED_PORT    PORTB
#define LED_PIN     PINB
#define LED	    PINB4
#elif LED == B5
#undef LED
#define LED_DDR     DDRB
#define LED_PORT    PORTB
#define LED_PIN     PINB
#define LED	    PINB5
#elif LED == B6
#undef LED
#define LED_DDR     DDRB
#define LED_PORT    PORTB
#define LED_PIN     PINB
#define LED	    PINB6
#elif LED == B7
#undef LED
#define LED_DDR     DDRB
#define LED_PORT    PORTB
#define LED_PIN     PINB
#define LED	    PINB7

#elif LED == C0
#undef LED
#define LED_DDR     DDRC
#define LED_PORT    PORTC
#define LED_PIN     PINC
#define LED	    PINC0
#elif LED == C1
#undef LED
#define LED_DDR     DDRC
#define LED_PORT    PORTC
#define LED_PIN     PINC
#define LED	    PINC1
#elif LED == C2
#undef LED
#define LED_DDR     DDRC
#define LED_PORT    PORTC
#define LED_PIN     PINC
#define LED	    PINC2
#elif LED == C3
#undef LED
#define LED_DDR     DDRC
#define LED_PORT    PORTC
#define LED_PIN     PINC
#define LED	    PINC3
#elif LED == C4
#undef LED
#define LED_DDR     DDRC
#define LED_PORT    PORTC
#define LED_PIN     PINC
#define LED	    PINC4
#elif LED == C5
#undef LED
#define LED_DDR     DDRC
#define LED_PORT    PORTC
#define LED_PIN     PINC
#define LED	    PINC5
#elif LED == C6
#undef LED
#define LED_DDR     DDRC
#define LED_PORT    PORTC
#define LED_PIN     PINC
#define LED	    PINC6
#elif LED == C7
#undef LED
#define LED_DDR     DDRC
#define LED_PORT    PORTC
#define LED_PIN     PINC
#define LED	    PINC7

#elif LED == D0
#undef LED
#define LED_DDR     DDRD
#define LED_PORT    PORTD
#define LED_PIN     PIND
#define LED	    PIND0
#elif LED == D1
#undef LED
#define LED_DDR     DDRD
#define LED_PORT    PORTD
#define LED_PIN     PIND
#define LED	    PIND1
#elif LED == D2
#undef LED
#define LED_DDR     DDRD
#define LED_PORT    PORTD
#define LED_PIN     PIND
#define LED	    PIND2
#elif LED == D3
#undef LED
#define LED_DDR     DDRD
#define LED_PORT    PORTD
#define LED_PIN     PIND
#define LED	    PIND3
#elif LED == D4
#undef LED
#define LED_DDR     DDRD
#define LED_PORT    PORTD
#define LED_PIN     PIND
#define LED	    PIND4
#elif LED == D5
#undef LED
#define LED_DDR     DDRD
#define LED_PORT    PORTD
#define LED_PIN     PIND
#define LED	    PIND5
#elif LED == D6
#undef LED
#define LED_DDR     DDRD
#define LED_PORT    PORTD
#define LED_PIN     PIND
#define LED	    PIND6
#elif LED == D7
#undef LED
#define LED_DDR     DDRD
#define LED_PORT    PORTD
#define LED_PIN     PIND
#define LED	    PIND7

#elif LED == E0
#undef LED
#define LED_DDR     DDRE
#define LED_PORT    PORTE
#define LED_PIN     PINE
#define LED	    PINE0
#elif LED == E1
#undef LED
#define LED_DDR     DDRE
#define LED_PORT    PORTE
#define LED_PIN     PINE
#define LED	    PINE1
#elif LED == E2
#undef LED
#define LED_DDR     DDRE
#define LED_PORT    PORTE
#define LED_PIN     PINE
#define LED	    PINE2
#elif LED == E3
#undef LED
#define LED_DDR     DDRE
#define LED_PORT    PORTE
#define LED_PIN     PINE
#define LED	    PINE3
#elif LED == E4
#undef LED
#define LED_DDR     DDRE
#define LED_PORT    PORTE
#define LED_PIN     PINE
#define LED	    PINE4
#elif LED == E5
#undef LED
#define LED_DDR     DDRE
#define LED_PORT    PORTE
#define LED_PIN     PINE
#define LED	    PINE5
#elif LED == E6
#undef LED
#define LED_DDR     DDRE
#define LED_PORT    PORTE
#define LED_PIN     PINE
#define LED	    PINE6
#elif LED == E7
#undef LED
#define LED_DDR     DDRE
#define LED_PORT    PORTE
#define LED_PIN     PINE
#define LED	    PINE7

#elif LED == F0
#undef LED
#define LED_DDR     DDRF
#define LED_PORT    PORTF
#define LED_PIN     PINF
#define LED	    PINF0
#elif LED == F1
#undef LED
#define LED_DDR     DDRF
#define LED_PORT    PORTF
#define LED_PIN     PINF
#define LED	    PINF1
#elif LED == F2
#undef LED
#define LED_DDR     DDRF
#define LED_PORT    PORTF
#define LED_PIN     PINF
#define LED	    PINF2
#elif LED == F3
#undef LED
#define LED_DDR     DDRF
#define LED_PORT    PORTF
#define LED_PIN     PINF
#define LED	    PINF3
#elif LED == F4
#undef LED
#define LED_DDR     DDRF
#define LED_PORT    PORTF
#define LED_PIN     PINF
#define LED	    PINF4
#elif LED == F5
#undef LED
#define LED_DDR     DDRF
#define LED_PORT    PORTF
#define LED_PIN     PINF
#define LED	    PINF5
#elif LED == F6
#undef LED
#define LED_DDR     DDRF
#define LED_PORT    PORTF
#define LED_PIN     PINF
#define LED	    PINF6
#elif LED == F7
#undef LED
#define LED_DDR     DDRF
#define LED_PORT    PORTF
#define LED_PIN     PINF
#define LED	    PINF7

#elif LED == G0
#undef LED
#define LED_DDR     DDRG
#define LED_PORT    PORTG
#define LED_PIN     PING
#define LED	    PING0
#elif LED == G1
#undef LED
#define LED_DDR     DDRG
#define LED_PORT    PORTG
#define LED_PIN     PING
#define LED	    PING1
#elif LED == G2
#undef LED
#define LED_DDR     DDRG
#define LED_PORT    PORTG
#define LED_PIN     PING
#define LED	    PING2
#elif LED == G3
#undef LED
#define LED_DDR     DDRG
#define LED_PORT    PORTG
#define LED_PIN     PING
#define LED	    PING3
#elif LED == G4
#undef LED
#define LED_DDR     DDRG
#define LED_PORT    PORTG
#define LED_PIN     PING
#define LED	    PING4
#elif LED == G5
#undef LED
#define LED_DDR     DDRG
#define LED_PORT    PORTG
#define LED_PIN     PING
#define LED	    PING5
#elif LED == G6
#undef LED
#define LED_DDR     DDRG
#define LED_PORT    PORTG
#define LED_PIN     PING
#define LED	    PING6
#elif LED == G7
#undef LED
#define LED_DDR     DDRG
#define LED_PORT    PORTG
#define LED_PIN     PING
#define LED	    PING7

#elif LED == H0
#undef LED
#define LED_DDR     DDRH
#define LED_PORT    PORTH
#define LED_PIN     PINH
#define LED	    PINH0
#elif LED == H1
#undef LED
#define LED_DDR     DDRH
#define LED_PORT    PORTH
#define LED_PIN     PINH
#define LED	    PINH1
#elif LED == H2
#undef LED
#define LED_DDR     DDRH
#define LED_PORT    PORTH
#define LED_PIN     PINH
#define LED	    PINH2
#elif LED == H3
#undef LED
#define LED_DDR     DDRH
#define LED_PORT    PORTH
#define LED_PIN     PINH
#define LED	    PINH3
#elif LED == H4
#undef LED
#define LED_DDR     DDRH
#define LED_PORT    PORTH
#define LED_PIN     PINH
#define LED	    PINH4
#elif LED == H5
#undef LED
#define LED_DDR     DDRH
#define LED_PORT    PORTH
#define LED_PIN     PINH
#define LED	    PINH5
#elif LED == H6
#undef LED
#define LED_DDR     DDRH
#define LED_PORT    PORTH
#define LED_PIN     PINH
#define LED	    PINH6
#elif LED == H7
#undef LED
#define LED_DDR     DDRH
#define LED_PORT    PORTH
#define LED_PIN     PINH
#define LED	    PINH7

#elif LED == J0
#undef LED
#define LED_DDR     DDRJ
#define LED_PORT    PORTJ
#define LED_PIN     PINJ
#define LED	    PINJ0
#elif LED == J1
#undef LED
#define LED_DDR     DDRJ
#define LED_PORT    PORTJ
#define LED_PIN     PINJ
#define LED	    PINJ1
#elif LED == J2
#undef LED
#define LED_DDR     DDRJ
#define LED_PORT    PORTJ
#define LED_PIN     PINJ
#define LED	    PINJ2
#elif LED == J3
#undef LED
#define LED_DDR     DDRJ
#define LED_PORT    PORTJ
#define LED_PIN     PINJ
#define LED	    PINJ3
#elif LED == J4
#undef LED
#define LED_DDR     DDRJ
#define LED_PORT    PORTJ
#define LED_PIN     PINJ
#define LED	    PINJ4
#elif LED == J5
#undef LED
#define LED_DDR     DDRJ
#define LED_PORT    PORTJ
#define LED_PIN     PINJ
#define LED	    PINJ5
#elif LED == J6
#undef LED
#define LED_DDR     DDRJ
#define LED_PORT    PORTJ
#define LED_PIN     PINJ
#define LED	    PINJ6
#elif LED == J7
#undef LED
#define LED_DDR     DDRJ
#define LED_PORT    PORTJ
#define LED_PIN     PINJ
#define LED	    PINJ7

#elif LED == K0
#undef LED
#define LED_DDR     DDRK
#define LED_PORT    PORTK
#define LED_PIN     PINK
#define LED	    PINK0
#elif LED == K1
#undef LED
#define LED_DDR     DDRK
#define LED_PORT    PORTK
#define LED_PIN     PINK
#define LED	    PINK1
#elif LED == K2
#undef LED
#define LED_DDR     DDRK
#define LED_PORT    PORTK
#define LED_PIN     PINK
#define LED	    PINK2
#elif LED == K3
#undef LED
#define LED_DDR     DDRK
#define LED_PORT    PORTK
#define LED_PIN     PINK
#define LED	    PINK3
#elif LED == K4
#undef LED
#define LED_DDR     DDRK
#define LED_PORT    PORTK
#define LED_PIN     PINK
#define LED	    PINK4
#elif LED == K5
#undef LED
#define LED_DDR     DDRK
#define LED_PORT    PORTK
#define LED_PIN     PINK
#define LED	    PINK5
#elif LED == K6
#undef LED
#define LED_DDR     DDRK
#define LED_PORT    PORTK
#define LED_PIN     PINK
#define LED	    PINK6
#elif LED == K7
#undef LED
#define LED_DDR     DDRK
#define LED_PORT    PORTK
#define LED_PIN     PINK
#define LED	    PINK7

#elif LED == L0
#undef LED
#define LED_DDR     DDRL
#define LED_PORT    PORTL
#define LED_PIN     PINL
#define LED	    PINL0
#elif LED == L1
#undef LED
#define LED_DDR     DDRL
#define LED_PORT    PORTL
#define LED_PIN     PINL
#define LED	    PINL1
#elif LED == L2
#undef LED
#define LED_DDR     DDRL
#define LED_PORT    PORTL
#define LED_PIN     PINL
#define LED	    PINL2
#elif LED == L3
#undef LED
#define LED_DDR     DDRL
#define LED_PORT    PORTL
#define LED_PIN     PINL
#define LED	    PINL3
#elif LED == L4
#undef LED
#define LED_DDR     DDRL
#define LED_PORT    PORTL
#define LED_PIN     PINL
#define LED	    PINL4
#elif LED == L5
#undef LED
#define LED_DDR     DDRL
#define LED_PORT    PORTL
#define LED_PIN     PINL
#define LED	    PINL5
#elif LED == L6
#undef LED
#define LED_DDR     DDRL
#define LED_PORT    PORTL
#define LED_PIN     PINL
#define LED	    PINL6
#elif LED == L7
#undef LED
#define LED_DDR     DDRL
#define LED_PORT    PORTL
#define LED_PIN     PINL
#define LED	    PINL7

#elif LED == A0
#undef LED
#define LED_DDR     DDRA
#define LED_PORT    PORTA
#define LED_PIN     PINA
#define LED	    PINA0
#elif LED == A1
#undef LED
#define LED_DDR     DDRA
#define LED_PORT    PORTA
#define LED_PIN     PINA
#define LED	    PINA1
#elif LED == A2
#undef LED
#define LED_DDR     DDRA
#define LED_PORT    PORTA
#define LED_PIN     PINA
#define LED	    PINA2
#elif LED == A3
#undef LED
#define LED_DDR     DDRA
#define LED_PORT    PORTA
#define LED_PIN     PINA
#define LED	    PINA3
#elif LED == A4
#undef LED
#define LED_DDR     DDRA
#define LED_PORT    PORTA
#define LED_PIN     PINA
#define LED	    PINA4
#elif LED == A5
#undef LED
#define LED_DDR     DDRA
#define LED_PORT    PORTA
#define LED_PIN     PINA
#define LED	    PINA5
#elif LED == A6
#undef LED
#define LED_DDR     DDRA
#define LED_PORT    PORTA
#define LED_PIN     PINA
#define LED	    PINA6
#elif LED == A7
#undef LED
#define LED_DDR     DDRA
#define LED_PORT    PORTA
#define LED_PIN     PINA
#define LED	    PINA7

#else
#error -------------------------------------------
#error Unrecognized LED name.  Should be like "B5"
#error -------------------------------------------
#endif
