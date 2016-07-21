#!/bin/bash
#
# This file specifies external files/folders from github locations
#  which are used in this distribution and should be updated
#  as necessary to incorporate into the hardware definition.
#
# The repos mentioned are checked out into 
#     {dirpath_to_makedist.sh}/.sources.tmp/REPO_NAME
#  and you can edit in there if you want while developing.
# 

function main
{
  # See diy_attiny/avr/sources.sh for a more complete example of using sources.sh
  
  
  
  # Because of:
  #   https://github.com/arduino/Arduino/issues/4619
  # it is best that we duplicate programmers.txt and make a unique name for each one in it    
  wget https://raw.githubusercontent.com/arduino/Arduino/master/hardware/arduino/avr/programmers.txt -O programmers.tmp.txt
  cat programmers.tmp.txt | sed -r 's/\.name=(.*)/.name=DIY ATmega: \1/' >programmers.txt
  rm programmers.tmp.txt
}

function github
{
  local TMP_DIR=$(dirname $(realpath $0))/../../../.sources.tmp/  
  local OUT_DIR=$(dirname $(realpath $0))
  if [ ! -d $TMP_DIR ]
  then
    mkdir $TMP_DIR || exit 1
  fi
  
  local GIT="$(echo $1 | sed 's/\/tree.*/.git/')"
  local GITNAME="$(echo $GIT | sed 's/.*\///' | sed 's/\.git//' )"
  local BRANCH="$(echo $1 | sed 's/.*tree\///' | sed 's/\/.*//')"
  local FILES="$(echo $1 | sed "s/..*\/tree\/$BRANCH\///")"
  pushd $TMP_DIR
    if [ ! -d $GITNAME ]
    then
      git clone $GIT      
    fi
    pushd $GITNAME
      git checkout $BRANCH      
    popd
    rm -rf $OUT_DIR/$2
    cp -rP $GITNAME/$FILES $OUT_DIR/$2
  popd
}

main "$@"