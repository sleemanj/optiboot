#%/bin/bash

if [ -n "$1" ]
then
  VERSION=$1
else
  VERSION="$(date +%Y.%m.%d)"
fi

# Create Arduino 1.6.x Format Distribution
#  we need 1MHz @ 9600, 8MHz and 16MHz at 57600
REPOROOT="$(pwd)"
DIST="$(pwd)/dists"
if [ ! -d $DIST ]
then
  echo "You must run this from the directory containing the dists directory."
  exit 1
fi

function package_for
{
  USINGMAKEFILES="$1" 
  PACKAGENAME="$2"
  IDE10X_LIMIT_TO="$3"
  
  export USINGMAKEFILES
  
  rm $DIST/$PACKAGENAME/avr/bootloaders/*.hex
  pushd optiboot/bootloaders/optiboot   
  
  AVR_FREQ=1000000L BAUD_RATE=9600 ./makeall
  cp *hex $DIST/$PACKAGENAME/avr/bootloaders
  make clean 

  AVR_FREQ=8000000L BAUD_RATE=57600 ./makeall
  cp *hex $DIST/$PACKAGENAME/avr/bootloaders
  make clean 

  AVR_FREQ=16000000L BAUD_RATE=57600 ./makeall
  cp *hex $DIST/$PACKAGENAME/avr/bootloaders
  make clean 
  popd

  pushd dists/$PACKAGENAME
  cp -rp avr $PACKAGENAME
  zip -r $PACKAGENAME.zip $PACKAGENAME   
  mv $PACKAGENAME.zip ../$PACKAGENAME.zip
  
  # That makes the 1.6.x package now we create a boards.txt for 1.0.x
  #  except we comment out all the boards  
  #  and then uncomment the ones we want
  
  #$REPOROOT/1.6-to-1.0.sh "avr/boards.txt" | sed "s/^\([a-z0-9_\.]\)/# \1/" | sed "/^# \(.*\\.\($(echo "$IDE10X_LIMIT_TO" | sed "s/ /|/")\)\)/\1/" >$PACKAGENAME/boards.txt
  local LOOKING_FOR="(_(($(echo "$IDE10X_LIMIT_TO" | sed "s/ /)|(/g")))_)"
  echo "s/^# \(.*\.${LOOKING_FOR}_[0-9]+MHz\)/\1/"
  $REPOROOT/1.6-to-1.0.sh "avr/boards.txt" | sed "s/^\([a-z0-9_\.]\)/# \1/" \
    | sed -r "s/^# (.*${LOOKING_FOR}.*)$/\1/" \
    >$PACKAGENAME/boards.txt
  
  #rm -r $PACKAGENAME
  popd 
  
  pushd dists
  HASH=$(cat $PACKAGENAME.zip | sha256sum | sed "s/ .*//")
  SIZE=$(stat -c %s $PACKAGENAME.zip)
  
  PACKAGEFILE=$(ls *_${PACKAGENAME}_index.json | head -1)
  cat $PACKAGEFILE | sed -r "s/SHA-256:([^\"]+)/SHA-256:$HASH/" \
                   | sed -r "s/\"size\":.*/\"size\": \"$SIZE\",/" \
                   | sed -r "s/\"version\":.*,/\"version\": \"$VERSION\",/" \
                   > $PACKAGEFILE.$VERSION.json  
  mv $PACKAGEFILE.$VERSION.json $PACKAGEFILE        
  popd
  
  
  
}


# Because of the menu-size in 1.0.x boards, we limit to only the 
#  most common ones, where most common has been determined by me 
#  having a look at aliexpress to see what's most often available, 
#  cheap, and sold a number of, also, what I have in my parts bin.
#
# The 1.6.x format boards.txt includes all of the IC configs
#  (well, most) by way of sub menus for variant, speed and boot
#  so we don't need a long menu in 1.6.x

package_for Makefile.atmega8 diy_atmega8_series "atmega328p atmega128a atmega88 atmega8a atmega48pa"