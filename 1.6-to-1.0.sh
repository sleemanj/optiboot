#!/bin/bash

IN_16_BOARDS="$1"

function usage
{
  echo "$0 /path/to/boards.txt"
}

if [ ! -f "$IN_16_BOARDS" ]
then
  echo "The input boards.txt does not exist."
  usage 
  exit 1
fi

BOARD_TYPES=$(cat $IN_16_BOARDS | grep -oP "^\s*([A-Za-z0-9_]+).name" | sed "s/^\s*//" | sed "s/\.name//" | sort | uniq | sed ':a;N;$!ba;s/\n/ /g' )
#echo "Board Types: $BOARD_TYPES"

# Find the menu options
MENU_OPTIONS=$(cat $IN_16_BOARDS | grep -oP "^(?:\s*)menu.([A-Za-z0-9_]+)(?:\s*=)" | sed "s/.*menu.//" | sed "s/\s*=//")
#echo "Menu: " $MENU_OPTIONS

function escape_sed
{
  echo "$1" | sed -e 's/[\/&]/\\&/g'
}

function get_from_settings
{
  echo "$1" | sed "s/\s+=/=/" | grep -F "$2=" | sed "s/^.*=\s*//" | sed "s/\s*$//"
}

function build_settings
{
  # Arguments are
  #   BOARD_NAME
  #   FULL_NAME_FOR_BOARD
  #   CURRENT_SETTINGS
  #   MENUS_LEFT_TO_PROCESS
  local BOARD_NAME="$1"
  local FULL_NAME_FOR_BOARD="$2"
  local CURRENT_SETTINGS="$3"
  local MENUS_LEFT_TO_PROCESS="$4"
  
  if [ -z "$MENUS_LEFT_TO_PROCESS" ]
  then 
  
    # The 1.0.x IDE doesn't support {build.mcu} {build.f_cpu} and {upload.speed} 
    #  replacemenet s in the bootloader filename, so we have to resolve these
    #  now.
    local BUILD_MCU="$(get_from_settings "$CURRENT_SETTINGS" build.mcu)"
    local BUILD_F_CPU="$(get_from_settings "$CURRENT_SETTINGS" build.f_cpu)"
    local UPLOAD_SPEED="$(get_from_settings "$CURRENT_SETTINGS" upload.speed)"
    
    CURRENT_SETTINGS="$(echo "$CURRENT_SETTINGS" | sed "s/{build.mcu}/$BUILD_MCU/" | sed "s/{build.f_cpu}/$BUILD_F_CPU/" | sed "s/{upload.speed}/$UPLOAD_SPEED/")"
    
    # It doesn't know about bootloader.tool or upload.tool, so strip those out
    #  (not that they do any harm, but don't want to cause confusion)
    CURRENT_SETTINGS="$(echo "$CURRENT_SETTINGS" | grep -v upload.tool | grep -v bootloader.tool)"
    
    echo "# $FULL_NAME_FOR_BOARD"
    echo "#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"    
    echo "$CURRENT_SETTINGS" | sed s/^\s*$BOARD_NAME./$FULL_NAME_FOR_BOARD./
    echo "$FULL_NAME_FOR_BOARD.bootloader.path="
    
    echo
    echo
  else
  
    # Take one menu off the stack
    local OUR_MENU="$(echo "$MENUS_LEFT_TO_PROCESS" | head -1)"    
    local FOLLOWING_MENUS="$(echo "$MENUS_LEFT_TO_PROCESS" | tail -n +2)"
    
    #echo "Processing $OUR_MENU for $BOARD_NAME"
    
    # Find the options that our board knows about in our menu
    local OUR_MENU_OPTIONS="$(cat $IN_16_BOARDS | grep -oP "^\s*$BOARD_NAME.menu.$OUR_MENU.([A-Za-z0-9_]+)(?:\s*=)" | sed "s/.*menu.$OUR_MENU.//" | sed "s/\s*=//" | sort | uniq )"
   # echo "$OUR_MENU:" $OUR_MENU_OPTIONS
    local THISOPT
    for THISOPT in $OUR_MENU_OPTIONS
    do
    
      # Pull out our special settings and roll them into the standard settings to pass forward
      local NEW_SETTINGS="$CURRENT_SETTINGS"
      local CURRENT_BOARD_TEXT_NAME=$(echo "$NEW_SETTINGS" | grep -P "^\s*$BOARD_NAME.name\s*=" | sed "s/^.*=\s*//" | sed "s/\s*$//")
      local OPTION_NAME="$(cat $IN_16_BOARDS | grep -P "^\s*$BOARD_NAME.menu.$OUR_MENU.$THISOPT\s*=" | sed "s/^.*=\s*//")"
      
      # Adjust the [boardname].name to reflect the selected options
      if [ "$OUR_MENU" = "version" ]
      then  
        # The version is a special case, we replace the main chip name with it.        
        NEW_SETTINGS="$(echo "$NEW_SETTINGS" | sed "s/$BOARD_NAME\.name\s*=.*$/$BOARD_NAME.name=$(escape_sed "$OPTION_NAME")/")"
      elif [ "$OUR_MENU" = "boot" ]
      then  
        # The boot menu is also a special case
        if [ "$THISOPT" = "none" ]
        then          
          NEW_SETTINGS="$(echo "$NEW_SETTINGS" | sed "s/$BOARD_NAME\.name\s*=.*$/$BOARD_NAME.name=$(escape_sed "$CURRENT_BOARD_TEXT_NAME, No Bootloader")/")"
        elif [ "$THISOPT" = "optiboot" ]
        then
          # We won't mark this with anything special because it is the "default"
          echo -n
        else
          NEW_SETTINGS="$(echo "$NEW_SETTINGS" | sed "s/$BOARD_NAME\.name\s*=.*$/$BOARD_NAME.name=$(escape_sed "$CURRENT_BOARD_TEXT_NAME, $OPTION_NAME")/")"
        fi        
      else      
        NEW_SETTINGS="$(echo "$NEW_SETTINGS" | sed "s/$BOARD_NAME\.name\s*=.*$/$BOARD_NAME.name=$(escape_sed "$CURRENT_BOARD_TEXT_NAME, $OPTION_NAME")/")"
      fi
      
      # Get all the override settings for THISOPT and put them into the new settings
      local OVERRIDES="$(cat $IN_16_BOARDS | grep -P "^\s*$BOARD_NAME.menu.$OUR_MENU.$THISOPT." | sed "s/^\s*$BOARD_NAME\.menu\.$OUR_MENU\.$THISOPT\.//" | sed "s/\s*=.*$//" | sed "s/\s*$//" )"
      local OVERRIDE
      
      for OVERRIDE in $OVERRIDES
      do
        # Strip out the existing one if any
        NEW_SETTINGS="$(echo "$NEW_SETTINGS" | grep -Pv "^\s*$BOARD_NAME.$OVERRIDE\s*=")"
        NEW_SETTINGS="$NEW_SETTINGS
$(cat $IN_16_BOARDS | grep -P "^\s*$BOARD_NAME\.menu\.$OUR_MENU\.$THISOPT\.$OVERRIDE\s*=" | sed "s/$BOARD_NAME\.menu\.$OUR_MENU\.$THISOPT\.$OVERRIDE\s*=/$BOARD_NAME.$OVERRIDE=/" | sed "s/\s*$//" )"
      done
      
      build_settings "$BOARD_NAME" "${FULL_NAME_FOR_BOARD}_${THISOPT}" "$NEW_SETTINGS" "$FOLLOWING_MENUS"
    done
  fi  
}


echo "
# 1.0.x (1.0.5+ Perhaps) Arduino IDE Compatible boards.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#
# This boards.txt has been automatically generated from a
# 1.6.x compatible version which contains sub-menus for each
# board to choose the specific variant (A, P etc), clock
# speed, and bootloader or not.
#
# Because the 1.0.x IDE does not have a submenu feature, 
# we include ALL the possible combinations below.
#
# However because the 1.0.x IDE also has a bit of a problem
# with long menus, we have disabled all but the most common
# by commenting them out.
#
# If you would like to enable one of the currently disabled
# options, simply remove the comment markers (hash character)
# from the start of all the lines of that option.
#
#############################################################

"

for BOARD in $BOARD_TYPES
do
  STANDARD_SETTINGS=$(cat $IN_16_BOARDS | grep -P "^\s*$BOARD." | grep -v "menu"  )
  build_settings "$BOARD" "$BOARD" "$STANDARD_SETTINGS" "$MENU_OPTIONS" 
done