#! /bin/bash

. scripts/set_platform.sh
. scripts/check_if_terminal_interactive.sh

if [ $RunningInInteractiveTerminal == 1 ]; then
  echo "Colorizing Output."
  . scripts/colors.sh
fi

COLORFLAG=""
if [ $RunningInInteractiveTerminal == 0 ]; then
  COLORFLAG="--colors-off"
fi


