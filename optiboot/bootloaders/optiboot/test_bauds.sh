#!/bin/bash

if [ -z "$1" ] || [ -z "$2" ]
then 
  echo "Usage: $0 chip frequency"
  exit 1
fi

for BAUD_RATE in 115200 57600 38400 19200 9600 2400 1200 300 150
do 
  if   AVR_FREQ=$2 BAUD_RATE=$BAUD_RATE ./makeall $1 >/dev/null
  then 
    echo -n $BAUD_RATE " "; cat $1.build_log | grep Error | head -1
  fi
done

