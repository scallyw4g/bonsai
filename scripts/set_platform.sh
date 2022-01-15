#! /bin/bash

Platform="Unknown"
UNAME=$(uname)
if [ "$UNAME" == "Linux" ] ; then
  Platform="Linux"
elif [[ "$UNAME" == CYGWIN* || "$UNAME" == MINGW* ]] ; then
  Platform="Windows"
fi
