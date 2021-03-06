#%/bin/bash

if [ -n "$1" ]
then
  VERSION=$1
else
  VERSION="$(date +%Y.%-m.%-d)"
fi

export VERSION

# Create Arduino 1.6.x Format Distribution
#  we need 1MHz @ 9600, 8MHz and 16MHz at 57600
REPOROOT="$(pwd)"
DIST="$(pwd)/dists"
if [ ! -d $DIST ]
then
  echo "You must run this from the directory containing the dists directory."
  exit 1
fi

function build_hexs
{
  #########################################################################
  # ATTENTION
  #########################################################################
  #  The tunable hex files are NOT just a concatenation of the 
  #   non-tunable bootloader and tuner.  The bootloader for tunable
  #   includes the OSCCAL_PROGMEM and DO_SPM stuff which the non-tunable
  #   does not.  So yes, they don't match, this is fine.  Don't waste
  #   a day trying to work out WTF is going on.
  #########################################################################

	# Call without AVR_FREQ and BAUD_RATE set to generate a standardised set
  if [ -z "$AVR_FREQ" ]
  then
    for FRQ in $(get_frequencies)
    do
      AVR_FREQ=$FRQ  BAUD_RATE=$BAUD_RATE build_hexs "$1" "$2" "$3"
    done
    #AVR_FREQ=1000000L  BAUD_RATE=$BAUD_RATE build_hexs "$1" "$2" "$3"
    #AVR_FREQ=8000000L  BAUD_RATE=$BAUD_RATE build_hexs "$1" "$2" "$3"
    #AVR_FREQ=16000000L BAUD_RATE=$BAUD_RATE build_hexs "$1" "$2" "$3"
    return
  fi
  
  if [ -z "$BAUD_RATE" ]
  then
    AVR_FREQ=$AVR_FREQ BAUD_RATE=$(baud_for_frequency $AVR_FREQ) build_hexs "$1" "$2" "$3"
    
    #case "$AVR_FREQ" in
		#	"1000000L") 
		#		AVR_FREQ=$AVR_FREQ BAUD_RATE=9600 build_hexs "$1" "$2" "$3"
		#		;;
		#		
		#	"8000000L") 
		#		AVR_FREQ=$AVR_FREQ BAUD_RATE=38400 build_hexs "$1" "$2" "$3"
		#		;;
		#		
		#	"16000000L") 
		#		AVR_FREQ=$AVR_FREQ BAUD_RATE=57600 build_hexs "$1" "$2" "$3"
		#		;;
    #esac
    return
  fi
  
  MAKEFILES="$1" 
  PACKAGENAME="$2"
  FEATURES="$3"
  
  pushd optiboot/bootloaders/optiboot   


  if ! FEATURES="$FEATURES" AVR_FREQ=$AVR_FREQ BAUD_RATE=$BAUD_RATE ./makeall $MAKEFILES
  then
    echo FEATURES=\""$FEATURES"\" AVR_FREQ=$AVR_FREQ BAUD_RATE=$BAUD_RATE ./makeall $MAKEFILES
    exit 1
  fi
  # If tunables were generated, copy those but remove the tunable_ prefix
  if [ -n "$(ls tunable_optiboot_*hex 2>/dev/null)" ]
  then
		for file in tunable_optiboot_*.hex
		do
			echo cp $file $DIST/$PACKAGENAME/avr/bootloaders/$(echo $file | sed -r 's/^tunable_//')
			cp $file $DIST/$PACKAGENAME/avr/bootloaders/$(echo $file | sed -r 's/^tunable_//')
	  done
	fi
	
	# If non-tunables were generated, copy those if a tunable was NOT created
	#  (it was copied above)
	for file in optiboot_*.hex
	do
		if ! [ -f $DIST/$PACKAGENAME/avr/bootloaders/$file ]
		then
			cp $file $DIST/$PACKAGENAME/avr/bootloaders/$file
		fi
  done
  make clean 

  popd
}

function package_for
{  
  PACKAGENAME="$2"
  IDE10X_LIMIT_TO="$3"
  
  local VERSION_FILE_NAME="$(echo "$VERSION" | sed "s/\./_/g")"
  local BOARD_MANAGER_PACKAGE="${PACKAGENAME}_${VERSION_FILE_NAME}.zip"
  
  # This is used by makeall to determine which
  #  chips we want to compile for (compiles all in those makefiles)
  if ! type "$1" | grep -q 'function' 2>/dev/null
  then    
    build_hexs "$1" "$2"
  else        
    $1 "$2"
  fi

  # Pull in external sources if any
  if [ -x $DIST/$PACKAGENAME/avr/sources.sh ]
  then
    pushd $DIST/$PACKAGENAME/avr
    ./sources.sh
    popd
  fi  
  
  # Make a "manual install" package suitable for sketchbook/hardware/[whatever]
  #  in Arduino 1.6
  pushd dists
  echo "Arduino 1.6.x boards.txt - MANUAL INSTALLATION PACKAGE
--------------------------------------------------------------------------------

This file contains manual installation files for boards.txt and optiboot
bootloaders for various microcontrollers for the 1.6.x Ardiuno IDE.

Arduino IDE 1.6.x allows you to use a boards manager to automatically install
boards such as these and this is recommended, instead of using this zip file.

See the following URL for the automatic installation instructions

  http://github.com/sleemanj/optiboot/dists/README.md

If you wish to install manually, copy the entire {$PACKAGENAME} folder into your Arduino 
Sketchbook \"hardware\" sub folder.

You can find what your sketchbook folder is by opening the Preferences in the 
Arduino IDE and you will see the \"sketchbook location\", commonly the sketchbook
folder for 1.6.x is...

  * In Windows \`Documents\Arduino\`
  * In Mac  \`Documents/Arduino\`
  * In Linux \`$HOME/Arduino\`

So for example, if your sketchbook folder is \`Documents\Arduino\` you would copy
the {$PACKAGENAME} folder to become \`Documents\Arduino\hardware\{$PACKAGENAME}\`

After installation, restart your Arduino IDE and you can then select the 
  
  * The family of chip you want to use (eg ATmega328), from the Boards menu
  * Processor Version (eg ATmega328p)- after choosing the board, this will appear in a menu
     underneath the Boards menu.
  * Processor Speed (eg 8MHz) - after choosing the board, this will appear in a menu
     underneath the Boards menu.
  * Bootloader (Yes or No) - after choosing the board, this will appear in a menu
     underneat the Boards menu.
     
" >>$PACKAGENAME/README.TXT
  zip -r ${PACKAGENAME}_ARDUINO_1_6_x_MANUAL_INSTALL.zip $PACKAGENAME
  rm $PACKAGENAME/README.TXT
  popd 
  
  pushd dists/$PACKAGENAME
  # For the board manager, the mid-level "platform" folder (avr) is omitted
  #  so we copy it to have the same name as the upper folder and zip that.
  #  Stupid board manager.  Annoying duplication.
  cp -rp avr $PACKAGENAME
    echo "Arduino 1.6.x boards.txt - AUTOMATIC INSTALLATION PACKAGE
--------------------------------------------------------------------------------

This file contains automatic installation files for boards.txt and optiboot
bootloaders for various microcontrollers for the 1.6.x Ardiuno IDE.

Arduino IDE 1.6.x allows you to use a boards manager to automatically install
boards such as these, this file is used by the Boards Manager

See the following URL for the automatic installation instructions

  http://github.com/sleemanj/optiboot/dists/README.md
     
" >>$PACKAGENAME/README.TXT
  zip -r $BOARD_MANAGER_PACKAGE $PACKAGENAME   
  rm $PACKAGENAME/README.TXT
  mv $BOARD_MANAGER_PACKAGE ../board_manager_packages/
  
  if [ 0 = 1 ]
  then
    # That makes the 1.6.x package now we create a boards.txt for 1.0.x
    #  except we comment out all the boards  
    #  and then uncomment the ones we want
    echo "Generating 1.0.x boards.txt"
    local LOOKING_FOR="(_(($(echo "$IDE10X_LIMIT_TO" | sed "s/ /)|(/g")))_[^.]*((optiboot|none))\.)"  
    echo "$LOOKING_FOR"
    $REPOROOT/1.6-to-1.0.sh "avr/boards.txt" | sed -r "s/^([a-z0-9_\.])/# \1/" \
      | sed -r "s/^# (.*${LOOKING_FOR}.*)$/\1/" \
      >$PACKAGENAME/boards.txt
    echo "...generation complete."
    
    
    # We will copy this into the package distribution source
    # just to help debugging really
    cp $PACKAGENAME/boards.txt avr/1_0_x_boards.txt
    
    # 1.0.x doesn't need platform.txt
    rm $PACKAGENAME/platform.txt
    rm $PACKAGENAME/platform.*.txt
      
    # A readme will be necessary, you can make one in 
    # the distrubution source called 1_0_x_README.1.txt
    # or if not then the automatic one will be created below
    # any further 1_0_x_README.x.txt will be concatenated
    
    if [ ! -f $PACKAGENAME/1_0_x_readme.1.txt ]
    then
    echo "Arduino 1.0.x Installation
  --------------------------------------------------------------------------------

  This file contains boards.txt and optiboot bootloaders for various 
  microcontrollers and is made for the 1.0.x IDE (or rather, 1.0.5 or later, I
  have not tested in older).

  To install this, copy the entire ${PACKAGENAME} folder into your Arduino 
  Sketchbook \"hardware\" sub folder.

  You can find what your sketchbook folder is by opening the Preferences in the 
  Arduino IDE and you will see the \"sketchbook location\", commonly the sketchbook
  folder is...

    * In Windows \`Documents\Arduino\`
    * In Mac  \`Documents/Arduino\`
    * In Linux \`$HOME/sketchbook\`

  So for example, if your sketchbook folder is \`Documents\Arduino\` you would copy
  the ${PACKAGENAME} folder to become \`Documents\\Arduino\\hardware\\${PACKAGENAME}\`

  When you have copied the folder, restart the Arduino IDE and you should be able
  to choose from new items in the Boards menu.  Note that configurations have been
  made for chips both with and without bootloader, the \"Upload Bootloader\"
  function for those without bootloaders just erases the chip and sets appropriate 
  fuses.

  Due to limitations of the Arduino 1.0.x IDE, there are likely many chips in
  the boards.txt which have been disabled in order to not have your Boards menu
  too long.  The most common chips have been left enabled.

  So if you find your particular chip is not present in the Boards 
  menu, open ${PACKAGENAME}/boards.txt and search for it, then you can uncomment
  (remove the # marks) from the settings for that board to enable it.
  " >$PACKAGENAME/README.TXT
    else 
      echo -n "" >$PACKAGENAME/README.TXT
    fi
    
    # May be some extra information there too
    cat $PACKAGENAME/1_0_x_readme.*.txt >>$PACKAGENAME/README.TXT
    rm  $PACKAGENAME/1_0_x_readme.*.txt
    
    # And for debugging ease only we copy this across
    cp $PACKAGENAME/README.TXT avr/1_0_x_README.TXT
    
    zip -r "${PACKAGENAME}_ARDUINO_1_0_x.zip" $PACKAGENAME   
    mv  "${PACKAGENAME}_ARDUINO_1_0_x.zip" ../
  fi
  rm -rf $PACKAGENAME
  popd   
  
  # Update the json file for this package with the new hash, size and 
  #  updated version information
  pushd dists/board_manager_packages
  HASH=$(cat $BOARD_MANAGER_PACKAGE | sha256sum | sed "s/ .*//")
  SIZE=$(stat -c %s $BOARD_MANAGER_PACKAGE)
  
  # Locate the most recent platform JSON for this package and use it as
  # a template for the data of this new package
  #PACKAGEFILE=$(ls -t ${PACKAGENAME}_*_platform.json | head -1)  
  PACKAGEFILE=$(ls ${PACKAGENAME}_*_platform.json | sed -r 's/(20[0-9]+)_([1-9])_/\1_0\2_/' | sed -r 's/_([1-9])_platform/_0\1_platform/' | sort | tail -1)
  if ! [ -e "$PACKAGEFILE" ]
  then
    PACKAGEFILE=$(echo $PACKAGEFILE | sed -r 's/_[0]/_/g')
  fi
  
  
  cat $PACKAGEFILE | sed -r "s/SHA-256:([^\"]+)/SHA-256:$HASH/" \
                  | sed -r "s/\"size\":.*/\"size\": \"$SIZE\",/" \
                  | sed -r "s/\"version\":.*,/\"version\": \"$VERSION\",/" \
                  | sed -r "s/\"archiveFileName\":.*,/\"archiveFileName\": \"$BOARD_MANAGER_PACKAGE\",/" \
                  | sed -r "s/(\"url\":.*\/)[^\/]+\.zip/\1$BOARD_MANAGER_PACKAGE/" \
                  > "${PACKAGENAME}_${VERSION_FILE_NAME}_platform.json.tmp"
  mv ${PACKAGENAME}_${VERSION_FILE_NAME}_platform.json.tmp ${PACKAGENAME}_${VERSION_FILE_NAME}_platform.json
  popd
  
  pushd dists
  # Locate the package index for this package and replace the platforms with
  #  the platform json's
  local INDEXFILE=$(ls *_${PACKAGENAME}_index.json | head -1)
  local PACKAGEHEADER="$(head -n $(cat $INDEXFILE | grep -n \"platforms\" | sed "s/:.*//") $INDEXFILE)"
  local PACKAGEFOOTER="$(tail -n +$(expr $(cat $INDEXFILE | grep -n \"tools\" | sed "s/:.*//" ) - 1) $INDEXFILE)"
  echo "$PACKAGEHEADER" >> $INDEXFILE.tmp
  echo >> $INDEXFILE.tmp
  local X=0
  for file in $(ls -t board_manager_packages/${PACKAGENAME}_*_platform.json)
  do
    if [ $X = 0 ]
    then
      X=1
    else
      echo ",">> $INDEXFILE.tmp
    fi
    echo >> $INDEXFILE.tmp
    cat $file >> $INDEXFILE.tmp
  done
  echo >> $INDEXFILE.tmp
  echo "$PACKAGEFOOTER" >> $INDEXFILE.tmp
  mv $INDEXFILE.tmp $INDEXFILE
  popd
  
    
}

function build_atmega8_hexs
{
  PACKAGENAME="$1"
  rm $DIST/$PACKAGENAME/avr/bootloaders/*.hex
    
  # Make the standard non tunable ones for all standard frequency
  #  and baud rates
  build_hexs Makefile.atmega8 "$PACKAGENAME"
  
  # Make non-default upload speeds for some frequencies
  FORCE_BAUD=1 AVR_FREQ=16000000L BAUD_RATE=115200 build_hexs "Makefile.atmega8" "$PACKAGENAME"
  FORCE_BAUD=1 AVR_FREQ=8000000L  BAUD_RATE=57600 build_hexs "Makefile.atmega8" "$PACKAGENAME"

  # Copy all those to notuner_ variants
  # Note that this means that except where tunable ones are generated again
  # in the next step, optiboot_xxxx.hex and notuner_optiboot_xxxx.hex 
  # will be duplicates.  
  #
  # In other words, for 16MHz and 20MHz both bootloaders optiboot_xxxx.hex
  # and notuner_optiboot_xxxx.hex are the same.  
  for file in $DIST/$PACKAGENAME/avr/bootloaders/optiboot*.hex
  do
    cp $file $(dirname $file)/notuner_$(basename $file)
  done
  
  # Make tunable for internal oscillator frequencies
  
  # 128kHz doesn't compile for tinyTuner, it's too slow
  #AVR_FREQ=128000L build_hexs Makefile.atmega8 "$PACKAGENAME" "TUNABLE"
  
  AVR_FREQ=1000000L build_hexs Makefile.atmega8 "$PACKAGENAME" "TUNABLE"
  AVR_FREQ=2000000L build_hexs Makefile.atmega8 "$PACKAGENAME" "TUNABLE"
  AVR_FREQ=4000000L build_hexs Makefile.atmega8 "$PACKAGENAME" "TUNABLE"
  AVR_FREQ=8000000L build_hexs Makefile.atmega8 "$PACKAGENAME" "TUNABLE"
  FORCE_BAUD=1 AVR_FREQ=8000000L  BAUD_RATE=57600 build_hexs "Makefile.atmega8" "$PACKAGENAME" "TUNABLE"
}

function build_attiny_hexs
{
  PACKAGENAME="$1"
  rm $DIST/$PACKAGENAME/avr/bootloaders/*.hex
  
  # Make the standard non tunable ones, and copy them to notuner_ variants
  # note that this means that except where tunable ones are generated again
  # in the next step, optiboot_xxxx.hex and notuner_optiboot_xxxx.hex 
  # will be duplicates.  
  #
  # In other words, for 16MHz, both bootloaders optiboot_xxxx.hex
  # and notuner_optiboot_xxxx.hex are the same.
  
  build_hexs "Makefile.attinyx4 Makefile.attinyx5" "$PACKAGENAME"
  
  # Make non-default upload speeds for some frequencies
  FORCE_BAUD=1 AVR_FREQ=16000000L BAUD_RATE=115200 build_hexs "Makefile.attinyx4 Makefile.attinyx5" "$PACKAGENAME"
  FORCE_BAUD=1 AVR_FREQ=8000000L  BAUD_RATE=57600 build_hexs "Makefile.attinyx4 Makefile.attinyx5" "$PACKAGENAME"

  
  for file in $DIST/$PACKAGENAME/avr/bootloaders/optiboot*.hex
  do
    cp $file $(dirname $file)/notuner_$(basename $file)
  done

  # Make tunable for internal oscillator freqs (incl PLL)
  
  # 128kHz doesn't compile for tinyTuner, it's too slow
  # AVR_FREQ=128000L build_hexs "Makefile.attinyx4 Makefile.attinyx5" "$PACKAGENAME" "TUNABLE"
  
  AVR_FREQ=1000000L build_hexs "Makefile.attinyx4 Makefile.attinyx5" "$PACKAGENAME" "TUNABLE"
  AVR_FREQ=8000000L build_hexs "Makefile.attinyx4 Makefile.attinyx5" "$PACKAGENAME" "TUNABLE"
  FORCE_BAUD=1 AVR_FREQ=8000000L  BAUD_RATE=57600 build_hexs "Makefile.attinyx4 Makefile.attinyx5" "$PACKAGENAME" "TUNABLE"
  
  # Only X5 has these
  AVR_FREQ=6400000L build_hexs "Makefile.attinyx4 Makefile.attinyx5" "$PACKAGENAME" "TUNABLE"  
  
  # 16MHz doesn't compile for tinyTuner, complains about 57600 being too slow for Software Serial
  # AVR_FREQ=16000000L build_hexs "Makefile.attinyx5" "$PACKAGENAME" "TUNABLE"
}

function get_frequencies
{
   cat $DIST/diy_atmega8_series/avr/boards.txt $DIST/diy_attiny/avr/boards.txt \
    | grep -v "#" \
    | grep f_cpu= | sed -r 's/.*f_cpu=//' | sort | uniq | sort -n | sed -r 's/\r|\n//'
}

function baud_for_frequency
{
   cat $DIST/diy_atmega8_series/avr/boards.txt $DIST/diy_attiny/avr/boards.txt \
     | grep "$(cat $DIST/diy_atmega8_series/avr/boards.txt $DIST/diy_attiny/avr/boards.txt | grep f_cpu="$1" | grep advfrq | head -1 | sed -r 's/\.build\.f_cpu=.*//').upload.speed" | head -1 | sed -r 's/.*=//' | sed -r 's/\r|\n//'

}

# Because of the menu-size in 1.0.x boards, we limit to only the 
#  most common ones, where most common has been determined by me 
#  having a look at aliexpress to see what's most often available, 
#  cheap, and sold a number of, also, what I have in my parts bin.
#
# The 1.6.x format boards.txt includes all of the IC configs
#  (well, most) by way of sub menus for variant, speed and boot
#  so we don't need a long menu in 1.6.x
AVAILOUT="$(pwd)/upload_maximum_size.txt"
rm -f "$AVAILOUT"
export AVAILOUT
package_for build_atmega8_hexs diy_atmega8_series "atmega328p atmega128a atmega88 atmega8a atmega48pa"
package_for build_attiny_hexs  diy_attiny         "attiny8[45]_[^.]*_FullCore attiny4[45]_[^.]*_FullCore attiny2[45]_[^.]*_SmallerCore attiny13_1_2MHz_[^.]*1p6"