#%/bin/bash

if [ -n "$1" ]
then
  VERSION=$1
else
  VERSION="$(date +%Y.%m.%d)"
fi

# Create Arduino 1.6.x Format Distribution
#  we need 1MHz @ 9600, 8MHz and 16MHz at 57600
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
  rm -r $PACKAGENAME
  mv $PACKAGENAME.zip ../$PACKAGENAME.zip
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


package_for Makefile.atmega8 diy_atmega8_series