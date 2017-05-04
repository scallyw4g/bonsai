#! /bin/bash

BINARY="build/Bonsai"

if [ $WIN32 -eq 1 ]; then

  BINARY="$BINARY"".exe"
  msbuild.exe ./bin/

else # Unix

  cd build
  make "$@" 2>&1

fi


# Run it!
[ $? -eq 0 ] && ./$BINARY > /dev/tty

