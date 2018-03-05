/** This is a cut-down version of tinytuner, it requires more 
 *  effort to use (that is, you have to send more x's)
 *  but takes less space.  How much less remains to be seen.
 */

#include <stdlib.h>
#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>

/*
 * Note that we use our own version of "boot.h"
 * <avr/boot.h> uses sts instructions, but this version uses out instructions
 * This saves cycles and program memory.  Sorry for the name overlap.
 */
#include "boot.h"


// We don't use <avr/wdt.h> as those routines have interrupt overhead we don't need.

/*
 * pin_defs.h
 * This contains most of the rather ugly defines that implement our
 * ability to use UART=n and LED=D3, and some avr family bit name differences.
 */
#include "pin_defs.h"


// Just make sure that if we haven't specified an LED that 
//  we have disabled the stuff that might use it.
#if ! (defined(LED) && defined(LED_PIN))
#undef LED 
#undef LED_DATA_FLASH 
#define LED_START_FLASHES 0
#endif

#ifndef LED_START_FLASHES
#define LED_START_FLASHES 0
#endif

// And again, make sure the led stuff is disabled totally if we 
// don't use it.
#if ! (defined(LED_DATA_FLASH) || (LED_START_FLASHES > 0))
  #undef LED
#endif

#define true 0x1
#define false 0x0

#ifndef TUNER_SERIAL_OUTPUT
  // Different levels of serial output are available
  //  0 = None
  //  1 = OSCCAL determined is output at finish
  //  2 = Instructions are put at start
  //  3 = Some indication of process for each keypress
  //  4 = Print the error (caution this requires snprintf, so won't fit in 2k)
	#if FLASHEND < 4095
		#define TUNER_SERIAL_OUTPUT 3
	#else
		#define TUNER_SERIAL_OUTPUT 4
	#endif
#endif

// For debugging you can force the search to start with a given
// value instead of whatever OSCCAL is set to, you can also 
// specify a known-good OSCCAL for the serial output so you can
// watch the search helpfully
#define DEBUG_START_WITH_OSCCAL 100
#define DEBUG_SERIAL_OUTPUT_USE_OSCCAL 147

#if TUNER_SERIAL_OUTPUT > 0
  void putcal();
  void putstr_t(const char* str);
	void putstr(const char* str);
  void putch_t(char ch);
#endif

#if TUNER_SERIAL_OUTPUT > 4
	#include <stdio.h>
#endif
	
void uartDelay_t() __attribute__ ((naked));
static uint16_t TimeNineBits( void );
uint8_t update( );

#define min(a,b) a<b?a:b
#define max(a,b) a<b?b:a
// #define abs(a)   a<0?0-a:a

void tinyTuner(){

#if defined(DEBUG_START_WITH_OSCCAL)
	OSCCAL = DEBUG_START_WITH_OSCCAL;
#endif
	
#if defined(LED)
  #if defined(__AVR_ATtiny85__)
    TCCR1 = _BV(CS13) | _BV(CS12); // div 2048 as it is only 8bit, so having to half TCNT1 value
  #elif !defined(__AVR_ATtiny24__)
    TCCR1B = _BV(CS12) | _BV(CS10); // div 1024
  #endif
    /* Set LED pin as output */
    LED_DDR |= _BV(LED);
  #if !defined(__AVR_ATtiny24__)
    LED_PORT &= ~ _BV(LED);
  #endif
#endif

#ifdef SOFT_UART
  /* Set TX pin as output */
  UART_DDR |= _BV(UART_TX_BIT);
  UART_PORT |= _BV(UART_TX_BIT); //set high!
#endif
  
  //Tunes the oscillator (this code is overwritten by sketch files)
#if TUNER_SERIAL_OUTPUT > 1
  putstr_t(PSTR("Send x repeatedly to tune...\r\n"));
	putcal();
#endif
  
#if defined(LED) && defined(LED_START_FLASHES) && LED_START_FLASHES > 0
  flash_led(LED_START_FLASHES * 10);
#endif
  
  uint8_t running = true;
  
  while ( running )
  {
    running = update();
    
#if TUNER_SERIAL_OUTPUT > 1
//    putcal();
#endif
    
#if defined(LED)
#if !defined(__AVR_ATtiny24__)
    uint8_t i=0;
    for ( ; i < 4; i++ ){
      if(!(i & 1)){
        LED_PIN |= _BV(LED); //toggle pin
      }
    #if defined(__AVR_ATtiny85__)
      TCNT1 = -(char)(F_CPU/(2048U*16U)); //clocks/sec * 1/16th second
      while(!(TIFR & _BV(TOV1)));
      TIFR = _BV(TOV1); //For tiny85, sbi() doesn't reach this register, so it saves memory to use an assignment rather than a bitwise or.
    #else
      TCNT1 = -(F_CPU/(1024*16)); //clocks/sec * 1/16th second
      //TIFR1 = _BV(TOV1);
      TIFR1 |= _BV(TOV1); //at boot TIFR1 is 0, so can just set the TOV1 bit - save memory.
      while(!(TIFR1 & _BV(TOV1)));
    #endif
    }
#else 
    LED_PIN |= _BV(LED); //toggle pin
#endif
#endif
  }

#if TUNER_SERIAL_OUTPUT > 1
	putch_t('\n');
  putstr_t(PSTR("Tuning complete, saving...\r\n"));
#endif

#if TUNER_SERIAL_OUTPUT > 0
    putcal();
#endif
	
  uint16_t addrPtr;
  
  //For this code we are assuming that the cleared value of each byte in the temporary page buffer is 0xFF
  //This is important as we have to write a page at a time which means that we will be overwriting bytes we
  //don't want to change - by using 0xFF this isn't an issue as programming can only convert a bit from a 1
  //to a 0 (otherwise it needs to erase which is not being done here). So if say you had 0b00100101, and reprogrammed
  //it with 0b11111111, the result would be 0b00100101 as none on the 0's can be turned into 1's. 
  
  // The cal value goes into FLASHEND-2, but since this is word addressing 
  // that will be FLASHEND-3 with the high byte FF to not change it
  // ie for a 4096 byte device (eg Tiny44) FLASHEND is 4095
  //    so [calword=4092][cal=4093][v.maj=4094][v.min=FLASHEND=4095]
  //    FLASHEND-3 == 4095-3 == 4092
  addrPtr = (uint16_t)(void*)FLASHEND-3;
  
  SPMCSR = CTPB; //clear the temporary page buffer - this sets all bytes to 0xFF so as not to change any bytes we don't want to
  typedef union {
      uint16_t integer;
      uint8_t array[2];
  }twoByte;
  twoByte oscProg;
  oscProg.array[1] = OSCCAL; //store the new OSCCAL value in the program memory so it can be restored by the bootloader at startup.
  oscProg.array[0] = 0xFF;   //don't change the first word byte
  __boot_page_fill_short((uint16_t)(void*)addrPtr,oscProg.integer); //store the two oscProg bytes to the temporary buffer
  __boot_page_write_short((uint16_t)(void*)addrPtr); //program the whole page. Any byte where temp=0xFF will remain as they were.
  boot_spm_busy_wait(); //wait for completion

#if TUNER_SERIAL_OUTPUT > 1
  putstr_t(PSTR("Calibration saved\r\n"));
  putstr_t(PSTR("\r\n\r\nEnabling Bootloader and Rebooting\r\n\r\n"));
#endif

  __asm__ __volatile__ (
    // Jump to RST vector
    "clr r30\n"
    "clr r31\n"
    "ijmp\n"
  );
  while(1); //to shut the compiler up - really the code doesn't return.
}

int main() { tinyTuner(); return 0; }

#if TUNER_SERIAL_OUTPUT > 0
void putcal(){
  putstr_t(PSTR("OSCCAL="));
  
	char buf[4];
	utoa((unsigned int)OSCCAL, buf, 10);
	putch_t(buf[0]);
	putch_t(buf[1]);
	putch_t(buf[2]);
	
  //putch_t(OSCCAL);
  putstr_t(PSTR("\r\n"));
}

void putstr_t(const char *str){
  unsigned char c = pgm_read_byte(str++);
  while (c) {
    putch_t(c);
	c = pgm_read_byte(str++);
  }
}

void putstr(const char *str){
  unsigned char c = *(str++);
  while (c) {
    putch_t(c);
		c = *(str++);
  }
}


void putch_t(char ch) {
	#if defined(DEBUG_SERIAL_OUTPUT_USE_OSCCAL)
		uint8_t OLD = OSCCAL;
		OSCCAL=DEBUG_SERIAL_OUTPUT_USE_OSCCAL;
	#endif
		
  __asm__ __volatile__ (
    "   com %[ch]\n" // ones complement, carry set
    "   sec\n"
    "1: brcc 2f\n"
    "   cbi %[uartPort],%[uartBit]\n"
    "   rjmp 3f\n"
    "2: sbi %[uartPort],%[uartBit]\n"
    "   nop\n"
    "3: rcall uartDelay_t\n"
    "   rcall uartDelay_t\n"
    //"   rcall uartDelay_t\n"
    //"   rcall uartDelay_t\n"
    "   lsr %[ch]\n"
    "   dec %[bitcnt]\n"
    "   brne 1b\n"
    :
    :
      [bitcnt] "d" ((uint8_t)10),
      [ch] "r" (ch),
      [uartPort] "I" (_SFR_IO_ADDR(UART_PORT)),
      [uartBit] "I" (UART_TX_BIT)
    :
      "r25"
  );
	
	#if defined(DEBUG_SERIAL_OUTPUT_USE_OSCCAL)
		OSCCAL=OLD;
	#endif
}
#endif

#define BAUD_VALUE (((F_CPU/9600)-29)/6)
#if BAUD_VALUE > 255
#error Baud rate too slow for soft UART
#endif

void uartDelay_t() {
  __asm__ __volatile__ (
    "ldi r25,%[count]\n"
    "1:dec r25\n"
    "brne 1b\n"
    "ret\n"
    ::[count] "M" (BAUD_VALUE):"r25"
  );
}

int16_t sampleError(uint8_t forThisOSCCAL)
{
	uint8_t oldOSCCAL = OSCCAL;
	OSCCAL = forThisOSCCAL;
	  // Time the 'x'
  uint16_t nbt = TimeNineBits();
  
  // Calculate the number of clock cycles spent in TimeNineBits
  int16_t clocks = (nbt-1)*5 + 5;
  
  // Calculate the difference between the actual number of cycles spent in TimeNineBits and the expected number of cycles
  int16_t error = clocks - 7500;
	
	#if TUNER_SERIAL_OUTPUT >= 3
		char buf[4];
		utoa((unsigned int)OSCCAL, buf, 10);
		putch_t(buf[0]);
		putch_t(buf[1]);
		putch_t(buf[2]);
	#endif
	
	OSCCAL = oldOSCCAL;
	return abs(error);
}

uint8_t update( ) 
{
	// What is the error at the current OSCCAL value
  static uint16_t error = 0xFFFF;
	if(error == 0xFFFF)
	{
		sampleError(OSCCAL);
		#if TUNER_SERIAL_OUTPUT >= 3
			putch_t('\n');
		#endif
	}
	
	// Now have a peek forward and backward
	uint8_t steps[] = { 30, 15, 5, 3, 1 };
	uint8_t x = 0, i = 0;
	
	int16_t peekError;
	int16_t peekCal;
	
	// When we get increased error, decrease this range
	//  we will then exclude stuff outside that range from the search
	static int16_t minGood = 0;
	static int16_t maxGood = 254;
	
	for(x = 0; x < sizeof(steps); x++)
	{
		// If we have not already looked above that distance from here
		//  try that
		peekCal = OSCCAL+steps[x];
		peekCal = min(peekCal, maxGood);
		peekCal = max(peekCal, minGood);
		
		if(peekCal<maxGood)
		{
			peekError = sampleError(peekCal);
			if(peekError < error)
			{
				#if TUNER_SERIAL_OUTPUT >= 3
				putch_t('\n');
				#endif
				
				// Error seems to improve, keep looking from there, do not 
				// come down below this point again
				minGood = OSCCAL;
				OSCCAL = peekCal;
				error  = peekError;
				return true;
			}
			else
			{
				if(peekError > error)
				{
					// We are looking too far, don't check here again
					maxGood = peekCal;
					#if TUNER_SERIAL_OUTPUT >= 3
					putch_t('!');
					#endif
				}
				
				#if TUNER_SERIAL_OUTPUT >= 3
					putch_t('\n');
				#endif
			}
		}
		
		peekCal = OSCCAL-steps[x];
		peekCal = min(peekCal, maxGood);
		peekCal = max(peekCal, minGood);
		if(peekCal > minGood)
		{
			peekError = sampleError(peekCal);
			if(peekError < error)
			{
				#if TUNER_SERIAL_OUTPUT >= 3
				putch_t('\n');
				#endif
				
				// Error seems to improve, keep looking from there,
				// do not come up above this point again
				maxGood = OSCCAL;
				OSCCAL = peekCal;
				error  = peekError;
				return true;
			}
			else
			{
				if(peekError > error)
				{
					// We are looking too far, don't check here again
					minGood = peekCal;
					#if TUNER_SERIAL_OUTPUT >= 3
					putch_t('!');
					#endif
				}
				
				#if TUNER_SERIAL_OUTPUT >= 3
					putch_t('\n');
				#endif
			}
		}
	}
	
	// If we got here then the error isn't improving nearby
	//   in either direction so we are probably as good as
	//   we are going to get	
	#if TUNER_SERIAL_OUTPUT >= 4
		putstr_t(PSTR("Final Error: "));
		char buf[20];	
		snprintf(buf, sizeof(buf), "%d/%d", error, 7500);	
		putstr(buf);
		putstr_t(PSTR(" with OSCCAL= "));
		snprintf(buf, sizeof(buf), "%d\n", OSCCAL);
		putstr(buf);
	#endif
	
	return false;
}

static uint16_t TimeNineBits( void ){
  // We need a fast (8 MHz) clock to maximize the accuracy
  #if (F_CPU != 8000000)
  uint8_t ClockDivisor = CLKPR;
  cli();
  CLKPR = _BV(CLKPCE);
  CLKPR = (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
  sei();
  #endif
  
  uint16_t Temp = 0;
  
  // lowercase 'x' on the wire...
  // ...1111111111 0 0001 1110 1 111111111...
  
  asm volatile
  (
    // Wait for a start bit
      "L%=wfsb: "
      "sbic  %[calreg], %[calbit]"              "\n\t"
      "rjmp  L%=wfsb"                           "\n\t"
      "cli"                                     "\n\t"

    // Ensure we can get exactly 7500 cycles (result = 1500)
      "nop"                                     "\n\t"
      "nop"                                     "\n\t"

    // Time the first segment (start bit + 3 least-significant bits of 'x'; lo bits)
      "L%=wfs1: "
      "adiw  %[reseult], 1"                     "\n\t"
      "sbis  %[calreg], %[calbit]"              "\n\t"
      "rjmp  L%=wfs1"                           "\n\t"

    // Time the second segment (middle bits of 'x'; hi bits)
      "L%=wfs2: "
      "adiw  %[reseult], 1"                     "\n\t"
      "sbic  %[calreg], %[calbit]"              "\n\t"
      "rjmp  L%=wfs2"                           "\n\t"

    // Finish at the third segment (most significant bit of 'x'; lo bit; hi stop bit terminates)
      "L%=wfs3: "
      "adiw  %[reseult], 1"                     "\n\t"
      "sbis  %[calreg], %[calbit]"              "\n\t"
      "rjmp  L%=wfs3"                           "\n\t"

      "sei"                                     "\n\t"

      : 
        [reseult] "+w" ( Temp )
      : 
      #ifdef UART_RX_PIN
        [calreg] "I" (_SFR_IO_ADDR(UART_RX_PIN)), //rx is on a different port.
      #else
        [calreg] "I" (_SFR_IO_ADDR(UART_PIN)),
      #endif
        [calbit] "I" ( UART_RX_BIT )
  );
  
  // Put the clock back the way we found it
  #if (F_CPU != 8000000)
  cli();
  CLKPR = _BV(CLKPCE);
  CLKPR = ClockDivisor;
  sei();
  #endif
  
  return( Temp );
}

