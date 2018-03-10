# Makefile for ATmegaBOOT
# E.Lins, 18.7.2005
# $Id$
#
# Instructions
#
# To make bootloader .hex file:
# make diecimila
# make lilypad
# make ng
# etc...
#
# To burn bootloader .hex file:
# make diecimila_isp
# make lilypad_isp
# make ng_isp
# etc...
#
# Edit History
# 20170915: aweatherguy: Tweak to the baudcheck target to fix errors which
#		         appear when building with Cygwin on Windows.
#                        These errors are due to the avr-gcc pre-processor
#                        output containing Windows line terminations (CR-LF).
#                        The resulting shell script will not run.
#                        Fix was to add a call to sed to remove the CR's
# 201406xx: WestfW: More Makefile restructuring.
#                   Split off Makefile.1284, Makefile.extras, Makefile.custom
#                   So that in theory, the main Makefile contains only the
#                   official platforms, and does not need to be modified to
#                   add "less supported" chips and boards.
# 201303xx: WestfW: Major Makefile restructuring.
#                   Allows options on Make command line "make xx LED=B3"
#                   (see also pin_defs.h)
#                   Divide into "chip" targets and "board" targets.
#                   Most boards are (recursive) board targets with options.
#                   Move isp target to separate makefile (fixes m8 EFUSE)
#                   Some (many) targets will now be rebuilt when not
#                     strictly necessary, so that options will be included.
#                     (any "make" with options will always compile.)
#                   Set many variables with ?= so they can be overridden
#                   Use arduinoISP settings as default for ISP targets
#
#
# * Copyright 2013-2015 by Bill Westfield.  Part of Optiboot.
# * This software is licensed under version 2 of the Gnu Public Licence.
# * See optiboot.c for details.

#----------------------------------------------------------------------
#
# program name should not be changed...
PROGRAM    = optiboot

# The default behavior is to build using tools that are in the users
# current path variables, but we can also build using an installed
# Arduino user IDE setup, or the Arduino source tree.
# Uncomment this next lines to build within the arduino environment,
# using the arduino-included avrgcc toolset (mac and pc)
# ENV ?= arduino
# ENV ?= arduinodev
# OS ?= macosx
# OS ?= windows

# export symbols to recursive makes (for ISP)
export

# Makefiles are annoying, we need some help to "escape" patterns
comma := ,
space := 
space += 
equal := =


# Feature Wrangling, mainly for backcompat we allow the maker to specify
#  features in two ways.
#
# A "FEATURES" environment variable which specifies a list of one or more 
#  features to set, for example, this is the newer way of doing it
#
#  FEATURES=OSCCAL_PROGMEM,SUPPORT_EEPROM,OSCCAL_EEPROM_ADDR=123 make attiny24
#
# or by specifying these as individual environment variables
#
#  OSCCAL_PROGMEM=1 SUPPORT_EEPROM=1 OSCCAL_EEPROM_ADDR=123 make attiny24
#
# This is where we take the individual environment variables if given
#  and stuff them into FEATURES instead.
ifndef FEATURES
	ifdef SOFT_UART
		FEATURES += SOFT_UART
	endif

	ifdef VIRTUAL_BOOT_PARTITION
		FEATURES += VIRTUAL_BOOT_PARTITION
	endif

	ifdef LED
		FEATURES += LED=$(strip $(LED))
	endif

	ifdef LED_START_FLASHES
		FEATURES += LED_START_FLASHES=$(strip $(LED_START_FLASHES))
	endif

	ifdef LED_DATA_FLASH
		FEATURES += LED_DATA_FLASH
	endif

	ifdef BIGBOOT
		FEATURES += BIGBOOT
	endif

	ifdef SUPPORT_EEPROM
		FEATURES += SUPPORT_EEPROM
	endif

	ifdef OSCCAL_EEPROM_ADDR
		FEATURES += OSCCAL_EEPROM_ADDR=$(strip $(OSCCAL_EEPROM_ADDR))
	endif

	ifdef OSCCAL_PROGMEM
		FEATURES += OSCCAL_PROGMEM
	endif

	ifdef TUNABLE
		FEATURES += TUNABLE OSCCAL_PROGMEM
	endif
endif

ifdef AVR_FREQ
	FEATURES += AVR_FREQ=$(strip $(AVR_FREQ))
endif

ifdef BAUD_RATE
	FEATURES += BAUD_RATE=$(strip $(BAUD_RATE))
endif


# Some defaults, if you don't have a bash shell these might not work so good.
# you can override these by specifying the non-underscore-prefixed-variable
# in your target, for example...
#
#  attiny24: BOOTLOADER_ADDRESS=0x100
#
# Each target must minimally provide MCU_TARGET, FLASH_SIZE
#  and if it will use virtual boot, then the save vector ("RESET_VECTOR")
#  which is a vector (ie interrupt vector) we will use to store the uploaded
#  firmware's entry point (rjmp to it).  Note the RESET_VECTOR is a 
#  0 based decimal vector number, 14 or 6 are commonly used (EE_RDY)
#
#  attiny24: MCU_TARGET = attiny24
#  attiny24: FLASH_SIZE = 1234
#  attiny24: RESET_VECTOR=14
#
#  Each target can specify various features
#
#    attiny24: FEATURES += SOFT_UART, VIRTUAL_BOOT_PARTITION
#
#  Additional features can also be passed in the environment either
#    in a FEATURES variable, or as separate variables
#
#    FEATURES=OSCCAL_PROGMEM,SUPPORT_EEPROM make attiny24
#    OSCCAL_PROGMEM=1 SUPPORT_EEPROM=1 make attiny24
#
# Some features require parameters
#
#    attiny24: FEATURES += SUPPORT_EEPROM,OSCCAL_EEPROM_ADDR=123
#
#    FEATURES=SUPPORT_EEPROM,OSCCAL_EEPROM_ADDR=123 make attiny24
#
# Failed attempts to wrangle make, the first one was supposed to 
#  be able to find an env var with that name or the name in the FEATURES
#  list, but for some reason origin $(1) is undefined all the time 
# 
# The second was suposed to be able to use has_feature for both 
#  simple features (OSCCAL_PROGMEM) and values (OSCCAL_EEPROM_ADDR=123)
#  but for some reason the call to findstring = doesn't work 
#  (even if I try and findstring something more obvious like ADDR
#   it still doesn't work, even though it clearly DOES contain that)
#
# Make is a PITA

#has_feature					 = $(if $(or \
															$(filter-out undefined, $(origin $(1))) ,\
															$(findstring $(1), $(subst $(comma),$(space),$(FEATURES)))\
														 ),\
													$(if $(2),\
														$(2), \
														-D$(strip $(1))=1 \
													 ), \
													$(1)===$(origin $(1))\
												)
#has_feature					 = $(if $(call find_in_features, $(1)),\
													$(if $(2),\
														$(2), \
														-D$(strip $(call find_in_features, $(1)))$(if $(findstring =, $(call find_in_features $(1))),  ,=1) \
													 ) \
												)
find_in_features		 = $(filter $(1)%, $(subst $(comma),$(space),$(strip $(FEATURES))))
find_var_in_features		 = $(filter $(1)=%, $(subst $(comma),$(space),$(strip $(FEATURES))))

has_feature					 = $(if $(call find_in_features, $(1)),\
													$(if $(2),\
														$(2),\
														-D$(strip $(call find_in_features, $(1)))=1 \
													 ) \
												)
												
var_feature					 = $(if $(call find_var_in_features, $(1)),\
													$(if $(2),\
														$(2),\
														-D$(strip $(call find_var_in_features, $(1))) \
													 ) \
												)

val_of_feature			 = $(lastword $(subst =, $(space), $(call var_feature, $(1))))

# Some things make the bootloader significantly bigger, we make a difference
#  between a "small" bootloader, and a "big" bootloader here just to determine
#  a useful default reserved size (which is only really applicable for virtual
#  boot bootloaders).
is_big_bootloader    = $(strip $(call has_feature, BIGBOOT) $(call has_feature, SUPPORT_EEPROM) $(call has_feature, OSCCAL_EEPROM_ADDR))

_MCU_TARGET          = $(word 1, $(MCU_TARGET) $(TARGET) atmega328)
_AVR_FREQ            = $(word 1, $(AVR_FREQ)  $(call val_of_feature, AVR_FREQ)  8000000L)
_BAUD_RATE           = $(word 1, $(BAUD_RATE) $(call val_of_feature, BAUD_RATE) 9600)
_BOOTLOADER_RESERVED = $(word 1, $(BOOTLOADER_RESERVED) $(if $(call is_big_bootloader), 700) 600)
_BOOTLOADER_ADDRESS  = $(word 1, $(BOOTLOADER_ADDRESS) $(shell printf "0x%0.4X" $$(( $(FLASH_SIZE) - $(_BOOTLOADER_RESERVED) / 2 * 2 ))))
_VERSION_ADDRESS     = $(word 1, $(VERSION_ADDRESS)    $(shell printf "0x%0.4X" $$(( $(FLASH_SIZE) - 2 / 2 * 2 ))))
_CAL_ADDRESS         = $(word 1, $(CAL_ADDRESS)        $(shell printf "0x%0.4X" $$(( $(FLASH_SIZE) - 3 ))))
_RESET_VECTOR        = $(word 1, $(RESET_VECTOR) 0)
_CFLAGS							 = $(call has_feature, SOFT_UART)              \
											 $(call has_feature, VIRTUAL_BOOT_PARTITION) \
											 $(call var_feature, LED)                    \
											 $(call var_feature, LED_START_FLASHES)      \
											 $(call has_feature, LED_DATA_FLASH)         \
											 $(call has_feature, BIGBOOT)                \
											 $(call has_feature, SUPPORT_EEPROM)         \
											 $(call var_feature, OSCCAL_EEPROM_ADDR)     \
											 $(call has_feature, TUNABLE, -DOSCCAL_PROGMEM)\
											 $(if $(call has_feature, VIRTUAL_BOOT_PARTITION), , $(call has_feature, TUNABLE, '-DDO_SPM'))\
											 $(call has_feature, OSCCAL_PROGMEM)         \
											 $(if $(call has_feature, VIRTUAL_BOOT_PARTITION), , $(call has_feature, OSCCAL_PROGMEM, '-DDO_SPM'))\
											 $(call has_feature, DO_SPM)

LDSECTIONS  = -Wl,--section-start=.text=$(_BOOTLOADER_ADDRESS) \
							-Wl,--section-start=.osccal=$(_CAL_ADDRESS)      \
							-Wl,--section-start=.version=$(_VERSION_ADDRESS) \
							-Wl,--gc-sections -Wl,--undefined=optiboot_version

# Build environments
# Start of some ugly makefile-isms to allow optiboot to be built
# in several different environments.  See the README.TXT file for
# details.

# default
fixpath = $(1)
SH := bash

ifeq ($(ENV), arduino)
# For Arduino, we assume that we're connected to the optiboot directory
# included with the arduino distribution, which means that the full set
# of avr-tools are "right up there" in standard places.
# (except that in 1.5.x, there's an additional level of "up")
TESTDIR := $(firstword $(wildcard ../../../tools/*))
ifeq (,$(TESTDIR))
# Arduino 1.5.x tool location compared to optiboot dir
  TOOLROOT = ../../../../tools
else
# Arduino 1.0 (and earlier) tool location
  TOOLROOT = ../../../tools
endif
GCCROOT = $(TOOLROOT)/avr/bin/

ifeq ($(OS), windows)
# On windows, SOME of the tool paths will need to have backslashes instead
# of forward slashes (because they use windows cmd.exe for execution instead
# of a unix/mingw shell?)  We also have to ensure that a consistent shell
# is used even if a unix shell is installed (ie as part of WINAVR)
fixpath = $(subst /,\,$1)
SHELL = cmd.exe
SH = sh
endif

else ifeq ($(ENV), arduinodev)
# Arduino IDE source code environment.  Use the unpacked compilers created
# by the build (you'll need to do "ant build" first.)
ifeq ($(OS), macosx)
TOOLROOT = ../../../../build/macosx/work/Arduino.app/Contents/Resources/Java/hardware/tools
endif
ifeq ($(OS), windows)
TOOLROOT = ../../../../build/windows/work/hardware/tools
endif

GCCROOT = $(TOOLROOT)/avr/bin/
AVRDUDE_CONF = -C$(TOOLROOT)/avr/etc/avrdude.conf

else ifdef ARDUINO_DIR
# Compile using a specified arduino IDE distribution
TOOLROOT = $(ARDUINO_DIR)/hardware/tools
GCCROOT = $(TOOLROOT)/avr/bin/
AVRDUDE_CONF = -C$(TOOLROOT)/avr/etc/avrdude.conf
else
GCCROOT =
AVRDUDE_CONF =
endif

STK500 = "C:\Program Files\Atmel\AVR Tools\STK500\Stk500.exe"
STK500-1 = $(STK500) -e -d$(_MCU_TARGET) -pf -vf -if$(PROGRAM)_$(TARGET).hex \
           -lFF -LFF -f$(HFUSE)$(LFUSE) -EF8 -ms -q -cUSB -I200kHz -s -wt
STK500-2 = $(STK500) -d$(_MCU_TARGET) -ms -q -lCF -LCF -cUSB -I200kHz -s -wt
#
# End of build environment code.


OBJ        = $(PROGRAM).o
OPTIMIZE = -Os -fno-split-wide-types -mrelax -fno-caller-saves

# -fno-caller-saves
# -----------------------------------------------------------------------------
#   This causes a decent reduction in code size (10 bytes)
#   @majekw implemented it here
#      https://github.com/majekw/optiboot/commit/6262a28c994034c9303185ce7d46a31ded0c8a00
#   in order to fix what I think was an optimisation regression in GCC 4.7/4.8
#
#   According to 
#      https://gcc.gnu.org/viewcvs/gcc/branches/gcc-6-branch/gcc/common/config/avr/avr-common.c?view=markup&pathrev=239080
#      https://gcc.gnu.org/bugzilla/show_bug.cgi?id=70677
#      https://gcc.gnu.org/bugzilla/show_bug.cgi?id=52025
#   this is the default in GCC 6 so it shouldn't cause any problem going by 
#   the comments.
#
#   However it still doesn't resolve an 8-10 byte opt regression I see 
#   between my local 4.8.2 and  the arduino distribution 4.9.2 with virboot 
#   targets.

DEFS       = 

#
# platforms support EEPROM and large bootloaders need the eeprom functions that
# are defined in libc, even though we explicity remove it with -nostdlib because
# of the space-savings.
LIBS       =  -lc

CC         = $(GCCROOT)avr-gcc

# Override is only needed by avr-lib build system.

override CFLAGS        = -g -Wall $(OPTIMIZE) -mmcu=$(_MCU_TARGET) -DF_CPU=$(_AVR_FREQ) -DBAUD_RATE=$(_BAUD_RATE) -Dsave_vect_num=$(_RESET_VECTOR) $(_CFLAGS) $(COMMON_OPTIONS) $(DEFS)
override LDFLAGS       = -g $(LDSECTIONS) -Wl,--relax -nostartfiles
#-nostdlib
#-Wl,--gc-sections

OBJCOPY        = $(GCCROOT)avr-objcopy
OBJDUMP        = $(call fixpath,$(GCCROOT)avr-objdump)

SIZE           = $(GCCROOT)avr-size

#
# Make command-line Options.
# Permit commands like "make atmega328 LED_START_FLASHES=10" to pass the
# appropriate parameters ("-DLED_START_FLASHES=10") to gcc
#

# For backcompat, take a feature-given baud rate and stuff it into BAUD_RATE
#  and also set a default here
ifndef BAUD_RATE
BAUD_RATE=$(word 1, $(call val_of_feature, BAUD_RATE) 115200)
endif

# For backcompat, take a feature-given AVR_FREQ and stuff it into AVR_FREQ
# and also set a default here
ifndef AVR_FREQ
AVR_FREQ=$(word 1, $(call val_of_feature, AVR_FREQ) 8000000L)
endif


COMMON_OPTIONS =

#UART is handled separately and only passed for devices with more than one.
ifdef UART
UART_CMD = -DUART=$(UART)
endif

# Not supported yet
# ifdef TIMEOUT_MS
# TIMEOUT_MS_CMD = -DTIMEOUT_MS=$(TIMEOUT_MS)
# dummy = FORCE
# endif
#

.PRECIOUS: %.elf

#---------------------------------------------------------------------------
# "Chip-level Platform" targets.
# A "Chip-level Platform" compiles for a particular chip, but probably does
# not have "standard" values for things like clock speed, LED pin, etc.
# Makes for chip-level platforms should usually explicitly define their
# options like: "make atmega1285 AVR_FREQ=16000000L LED=D0"
#---------------------------------------------------------------------------
#
# Note about fuses:
# the efuse should really be 0xf8; since, however, only the lower
# three bits of that byte are used on the atmega168, avrdude gets
# confused if you specify 1's for the higher bits, see:
# http://tinker.it/now/2007/02/24/the-tale-of-avrdude-atmega168-and-extended-bits-fuses/
#
# similarly, the lock bits should be 0xff instead of 0x3f (to
# unlock the bootloader section) and 0xcf instead of 0x2f (to
# lock it), but since the high two bits of the lock byte are
# unused, avrdude would get confused.
#---------------------------------------------------------------------------
#

# Test platforms
# Virtual boot block test
virboot8: TARGET = atmega8
virboot8: MCU_TARGET = atmega8
virboot8: CFLAGS += $(COMMON_OPTIONS) '-DVIRTUAL_BOOT_PARTITION' '-Dsave_vect_num=EE_RDY_vect_num'
virboot8: AVR_FREQ ?= 16000000L 
virboot8: LDSECTIONS  = -Wl,--section-start=.text=0x1c00 -Wl,--section-start=.osccal=0x1ffd -Wl,--section-start=.version=0x1ffe
virboot8: $(PROGRAM)_virboot8.hex
virboot8: $(PROGRAM)_virboot8.lst


virboot328: TARGET = atmega328
virboot328: MCU_TARGET = atmega328p
virboot328: CFLAGS += $(COMMON_OPTIONS) '-DVIRTUAL_BOOT_PARTITION'
virboot328: AVR_FREQ ?= 16000000L
virboot328: LDSECTIONS  = -Wl,--section-start=.text=0x7d80 -Wl,--section-start=.osccal=0x7ffd -Wl,--section-start=.version=0x7ffe
virboot328: $(PROGRAM)_virboot328.hex
virboot328: $(PROGRAM)_virboot328.lst

virboot328_isp: virboot328
virboot328_isp: TARGET = virboot328
virboot328_isp: MCU_TARGET = atmega328p
# no boot section, SPIEN
virboot328_isp: HFUSE ?= DF
# Low power xtal (16MHz) 16KCK/14CK+65ms
virboot328_isp: LFUSE ?= FF
# 2.7V brownout
virboot328_isp: EFUSE ?= 05
virboot328_isp: isp

#---------------------------------------------------------------------------
# "Board-level Platform" targets.
# A "Board-level Platform" implies a manufactured platform with a particular
# AVR_FREQ, LED, and so on.  Parameters are not particularly changable from
# the "make" command line.
# Most of the board-level platform builds should envoke make recursively
#  appropriate specific options
#---------------------------------------------------------------------------
# 20MHz clocked platforms
#
# These are capable of 230400 baud, or 115200 baud on PC (Arduino Avrdude issue)
#

pro20: TARGET = pro_20mhz
pro20: CHIP = atmega168
pro20:
	$(MAKE) atmega168 AVR_FREQ=20000000L LED_START_FLASHES=3
	mv $(PROGRAM)_$(CHIP).hex $(PROGRAM)_$(TARGET).hex
	mv $(PROGRAM)_$(CHIP).lst $(PROGRAM)_$(TARGET).lst

pro20_isp: pro20
pro20_isp: TARGET = pro_20mhz
# 4.3V brownout (for max speed!)
pro20_isp: HFUSE ?= DC
# Full swing xtal (20MHz) 258CK/14CK+4.1ms
pro20_isp: LFUSE ?= F7
# 512 byte boot
pro20_isp: EFUSE ?= 04
pro20_isp: isp

# 16MHz clocked platforms
#
# These are capable of 230400 baud, or 115200 baud on PC (Arduino Avrdude issue)
#

pro16: TARGET = pro_16MHz
pro16: CHIP = atmega168
pro16:
	$(MAKE) $(CHIP) AVR_FREQ=16000000L LED_START_FLASHES=3
	mv $(PROGRAM)_$(CHIP).hex $(PROGRAM)_$(TARGET).hex
	mv $(PROGRAM)_$(CHIP).lst $(PROGRAM)_$(TARGET).lst

pro16_isp: pro16
pro16_isp: TARGET = pro_16MHz
# 2.7V brownout
pro16_isp: HFUSE ?= DD
# Full swing xtal (20MHz) 258CK/14CK+4.1ms
pro16_isp: LFUSE ?= F7
# 512 byte boot
pro16_isp: EFUSE ?= 04
pro16_isp: isp

diecimila: TARGET = diecimila
diecimila: CHIP = atmega168
diecimila:
	$(MAKE) $(CHIP) AVR_FREQ=16000000L LED_START_FLASHES=3
	mv $(PROGRAM)_$(CHIP).hex $(PROGRAM)_$(TARGET).hex
	mv $(PROGRAM)_$(CHIP).lst $(PROGRAM)_$(TARGET).lst

diecimila_isp: diecimila
diecimila_isp: TARGET = diecimila
# 2.7V brownout
diecimila_isp: HFUSE ?= DD
# Low power xtal (16MHz) 16KCK/14CK+65ms
diecimila_isp: LFUSE ?= F7
# 512 byte boot
diecimila_isp: EFUSE ?= 04
diecimila_isp: isp

# MEGA1280 Board (this is different from the atmega1280 chip platform)
# Mega has a minimum boot size of 1024 bytes, so enable extra functions
# Note that optiboot does not (can not) work on the MEGA2560
#mega: TARGET = atmega1280
mega1280: atmega1280

mega1280_isp: mega1280
mega1280_isp: TARGET = atmega1280
mega1280_isp: MCU_TARGET = atmega1280
# 1024 byte boot
mega1280_isp: HFUSE ?= DE
# Low power xtal (16MHz) 16KCK/14CK+65ms
mega1280_isp: LFUSE ?= FF
# 2.7V brownout; wants F5 for some reason...
mega1280_isp: EFUSE ?= F5
mega1280_isp: isp

# 8MHz clocked platforms
#
# These are capable of 115200 baud
# Note that "new" Arduinos with an AVR as USB/Serial converter will NOT work
# with an 8MHz target Arduino.  The bitrate errors are in opposite directions,
# and total too large a number.
#

lilypad: TARGET = $@
lilypad: CHIP = atmega168
lilypad:
	$(MAKE) $(CHIP) AVR_FREQ=8000000L LED_START_FLASHES=3
	mv $(PROGRAM)_$(CHIP).hex $(PROGRAM)_$(TARGET).hex
	mv $(PROGRAM)_$(CHIP).lst $(PROGRAM)_$(TARGET).lst

lilypad_isp: lilypad
lilypad_isp: TARGET = lilypad
# 2.7V brownout
lilypad_isp: HFUSE ?= DD
# Internal 8MHz osc (8MHz) Slow rising power
lilypad_isp: LFUSE ?= E2
# 512 byte boot
lilypad_isp: EFUSE ?= 04
lilypad_isp: isp

# lilypad_resonator is the same as a 8MHz lilypad, except for fuses.
lilypad_resonator: lilypad

lilypad_resonator_isp: lilypad
lilypad_resonator_isp: TARGET = lilypad
# 2.7V brownout
lilypad_resonator_isp: HFUSE ?= DD
# Full swing xtal (20MHz) 258CK/14CK+4.1ms
lilypad_resonator_isp: LFUSE ?= C6
# 512 byte boot
lilypad_resonator_isp: EFUSE ?= 04
lilypad_resonator_isp: isp

pro8: TARGET = pro_8MHz
pro8: CHIP = atmega168
pro8:
	$(MAKE) $(CHIP) AVR_FREQ=8000000L LED_START_FLASHES=3
	mv $(PROGRAM)_$(CHIP).hex $(PROGRAM)_$(TARGET).hex
	mv $(PROGRAM)_$(CHIP).lst $(PROGRAM)_$(TARGET).lst

pro8_isp: pro8
pro8_isp: TARGET = pro_8MHz
# 2.7V brownout
pro8_isp: HFUSE ?= DD
# Full swing xtal (20MHz) 258CK/14CK+4.1ms
pro8_isp: LFUSE ?= C6
# 512 byte boot
pro8_isp: EFUSE ?= 04
pro8_isp: isp

atmega328_pro8: TARGET = atmega328_pro_8MHz
atmega328_pro8: CHIP = atmega328
atmega328_pro8:
	$(MAKE) $(CHIP) AVR_FREQ=8000000L LED_START_FLASHES=3
	mv $(PROGRAM)_$(CHIP).hex $(PROGRAM)_$(TARGET).hex
	mv $(PROGRAM)_$(CHIP).lst $(PROGRAM)_$(TARGET).lst

atmega328_pro8_isp: atmega328_pro8
atmega328_pro8_isp: TARGET = atmega328_pro_8MHz
atmega328_pro8_isp: MCU_TARGET = atmega328p
# 512 byte boot, SPIEN
atmega328_pro8_isp: HFUSE ?= DE
# Low power xtal (16MHz) 16KCK/14CK+65ms
atmega328_pro8_isp: LFUSE ?= FF
# 2.7V brownout
atmega328_pro8_isp: EFUSE ?= 05
atmega328_pro8_isp: isp

#
# Include additional platforms
include Makefile.atmel
include Makefile.extras
include Makefile.1284
include Makefile.custom
include Makefile.atmega8
include Makefile.attinyx5
include Makefile.attinyx4
include Makefile.attinyx7
include Makefile.atmegaxx4-16-32
include Makefile.atmega640-1280-1281-2560-2561
#---------------------------------------------------------------------------
#
# Generic build instructions
#

FORCE:

hex_relocate_reset.bin:
	@echo
	@echo "Compiling Hex Combining Tool"
	@echo "--------------------------------------------------------------------------------"
	gcc -std=c99 hex_relocate_reset.c -o hex_relocate_reset.bin
	@echo "--------------------------------------------------------------------------------"
	@echo 
	
%.tuner.elf:
	$(CC) $(CFLAGS) -o $@ veryTinyTuner.c
	avr-size $@
	cp $@ /tmp/

%.tuner.hex: %.tuner.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@
	
%_combine_with_tuner.hex: RESET_VECTOR := $(word 1, $(RESET_VECTOR) 0)
%_combine_with_tuner.hex: hex_relocate_reset.bin
	# Build the tuner (veryTinyTuner) for the given AVR frequency
	#  strip off the end-of-file marker and put it in a temporary file
	MCU_TARGET=$(word 1, $(MCU_TARGET) $(*)) AVR_FREQ=$(_AVR_FREQ) $(MAKE) $*_$(_AVR_FREQ).tuner.hex
	cat $*_$(_AVR_FREQ).tuner.hex | grep -vP '^:00000001FF\r' >$@.tmp
	
	# Build optiboot for the frequency and concatenate it to the tuner	
	AVR_FREQ=$(_AVR_FREQ) BAUD_RATE=$(BAUD_RATE) BOOTLOADER_ADDRESS=$(BOOTLOADER_ADDRESS) $(MAKE) $*
	cat $(PROGRAM)_$(*)_$(_AVR_FREQ)_$(BAUD_RATE).hex >>$@.tmp
	
	@echo
	@echo "Combining Tiny Tuner and Bootloader"
	@echo "--------------------------------------------------------------------------------"
	# Fix the reset vector for the virtual boot
	cat $@.tmp | ./hex_relocate_reset.bin $(_RESET_VECTOR) $(_BOOTLOADER_ADDRESS) >$@
	rm  $@.tmp
	@echo "--------------------------------------------------------------------------------"
	@echo
	
# This is done so that chips can pass in things like BOOTLOADER_ADDRESS and RESET_VECTOR
# by overriding this (see Makefile.attinyx4 for example)
%_with_tuner.hex: %_combine_with_tuner.hex
	mv $< $@
	
baudcheck: FORCE
	- @$(CC) --version
	- @$(CC) $(CFLAGS) -E baudcheck.c -o baudcheck.tmp
	- @sed 's/\r//' baudcheck.tmp >baudcheck.tmp.sh
	- @rm baudcheck.tmp
	- @$(SH) baudcheck.tmp.sh

isp: $(TARGET)
	$(MAKE) -f Makefile.isp isp TARGET=$(TARGET)

isp-stk500: $(PROGRAM)_$(TARGET).hex
	$(STK500-1)
	$(STK500-2)

%.elf: $(OBJ) $(dummy)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)
	$(SIZE) $@

clean:
	@echo
	@echo "Making Clean"
	@echo "--------------------------------------------------------------------------------"
	rm -rf *.o *.elf *.lst *.map *.sym *.lss *.eep *.srec *.bin *.hex *.tmp.sh *.build_log
	@echo "--------------------------------------------------------------------------------"
	@echo
	
%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -j .version -j .osccal --set-section-flags .version=alloc,load --set-section-flags .osccal=alloc,load -O ihex $< $@
	
%.srec: %.elf
	$(OBJCOPY) -j .text -j .data -j .version -j .osccal -set-section-flags .version=alloc,load --set-section-flags .osccal=alloc,load -O srec $< $@

%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -j .version -j .osccal --set-section-flags .version=alloc,load --set-section-flags .osccal=alloc,load -O binary $< $@

# This target is called from the individual targets in the other makefiles
#  eg attiny24: _optiboot_
_optiboot_:  $(if $(call has_feature, TUNABLE), _optiboot_tuner_, _optiboot_only_ )

_optiboot_tuner_: _optiboot_only_ 
_optiboot_tuner_: hex_relocate_reset.bin
	@echo
	@echo "Building Tiny Tuner"
	@echo "--------------------------------------------------------------------------------"
	# Build the tuner (veryTinyTuner) for the given AVR frequency
	#  strip off the end-of-file marker and put it in a temporary file
	MCU_TARGET=$(_MCU_TARGET) AVR_FREQ=$(_AVR_FREQ) $(MAKE) $(_MCU_TARGET)_$(_AVR_FREQ).tuner.hex
	@echo "--------------------------------------------------------------------------------"
	@echo
	
	@echo
	@echo "Combining Tiny Tuner and Bootloader"
	@echo "--------------------------------------------------------------------------------"
	
	cat $(_MCU_TARGET)_$(_AVR_FREQ).tuner.hex | grep -vP '^:00000001FF\r' >$@.tmp
	
	cat $(PROGRAM)_$(TARGET)_$(AVR_FREQ)_$(BAUD_RATE).hex >>$@.tmp
	
	# Fix the reset vector for the virtual boot
	cat $@.tmp | ./hex_relocate_reset.bin $(_RESET_VECTOR) $(_BOOTLOADER_ADDRESS) >tunable_$(PROGRAM)_$(TARGET)_$(AVR_FREQ)_$(BAUD_RATE).hex
	rm  $@.tmp
	@echo "--------------------------------------------------------------------------------"
	@echo "Bootloader: $(PROGRAM)_$(TARGET)_$(AVR_FREQ)_$(BAUD_RATE).hex"
	@echo "Tuner     : $(_MCU_TARGET)_$(_AVR_FREQ).tuner.hex"
	@echo "Tunable   : tunable_$(PROGRAM)_$(TARGET)_$(AVR_FREQ)_$(BAUD_RATE).hex"
	@echo "Available : $$((0x$$($(OBJDUMP) -h  $(PROGRAM)_$(TARGET)_$(AVR_FREQ)_$(BAUD_RATE).hex | grep .sec1 | sed -r 's/^\s*[0-9]+\s.sec1\s*[0-9a-f]+\s*0+([0-9a-f]{3,8}).*$$/\1/') - 1)) Bytes"
	@echo 
	
_optiboot_only_: _check_baud_rate_
	@echo
	@echo "Building Optiboot Bootloader"
	@echo "--------------------------------------------------------------------------------"
	TARGET=$(TARGET) \
	MCU_TARGET=$(_MCU_TARGET) \
	FLASH_SIZE=$(FLASH_SIZE) \
	BOOTLOADER_RESERVED=$(_BOOTLOADER_RESERVED) \
	BOOTLOADER_ADDRESS=$(_BOOTLOADER_ADDRESS) \
	VERSION_ADDRESS=$(_VERSION_ADDRESS) \
	CAL_ADDRESS=$(_CAL_ADDRESS) \
	RESET_VECTOR=$(_RESET_VECTOR) \
	FEATURES="$(FEATURES)" \
	  make $(PROGRAM)_$(TARGET)_$(AVR_FREQ)_$(BAUD_RATE).hex
	@echo "--------------------------------------------------------------------------------"
	@echo "Bootloader: $(PROGRAM)_$(TARGET)_$(AVR_FREQ)_$(BAUD_RATE).hex"
	@echo "Available : $$((0x$$($(OBJDUMP) -h  $(PROGRAM)_$(TARGET)_$(AVR_FREQ)_$(BAUD_RATE).hex | grep .sec1 | sed -r 's/^\s*[0-9]+\s.sec1\s*[0-9a-f]+\s*0+([0-9a-f]{3,8}).*$$/\1/'))) Bytes"
	@echo
	
# This target replaces the old baudcheck target and causes the process to stop
#  if the error is >= 2%, experience shows that >= 2% is unreliable especially
#  with the CH340G when reading flash through avrdude for some unknown reason
#  it actually works ok at (-)2% for general sending, but not for reading flash
#  I don't know why.  This is experienced using 8MHz clock and 57600 Baud Rate
#  dropping to 38400 Baud reduces the error to near zero and this is fine at 8MHz
#
# Even tuning the oscillator doesn't help, which is weird, you'd think it would.
#
# Anyway, the BAUD_ACTUAL is derived from the calculations in the old baudcheck.c
#  don't ask me how it works I didn't decipher it.
#
_check_baud_rate_:BAUD_ACTUAL=$$(( $$(echo "($(AVR_FREQ)/(8 * (($$(( $$(echo "(( ($(AVR_FREQ) + $(BAUD_RATE) * 4) / (($(BAUD_RATE) * 8))) - 1 )" | sed -r 's/L//g') )))+1)))" | sed -r 's/L//g') ))
_check_baud_rate_:
	@echo
	@echo "Check That Baud Rate Will Probably Be Ok"
	@echo "--------------------------------------------------------------------------------"
	@echo "Desired                 : $(BAUD_RATE)"
	@echo "Achieved (Theoretically): $(BAUD_ACTUAL)"
	@echo -n "Error                   : "
	@echo $$( echo "scale=1;(( 100*( $(BAUD_RATE) - $(BAUD_ACTUAL)) ) / $(BAUD_RATE))"| bc )"%"
ifndef FORCE_BAUD
	@if [ $$(echo "scale=1;x = (( 100*( $(BAUD_RATE) - $(BAUD_ACTUAL)) ) / $(BAUD_RATE));if(x<0)x=0-x;if(x >= 1.9){x=1}else{x=0};x"| bc ) = 1 ]; then echo "Acceptable              : NO\n"; echo "    Error is too large, this probably will result in an unreliable bootloader.\n    To ignore, set the FORCE_BAUD=1 environment variable.\n    Otherwise a different (not necessarily lower) BAUD_RATE or AVR_FREQ.\n    Common combinations are 16MHz:57600, 8MHz:38400, 1MHz:9600"; echo "--------------------------------------------------------------------------------"; exit 1; else echo "Acceptable              : YES"; echo "--------------------------------------------------------------------------------"; exit 0; fi
endif
	@echo