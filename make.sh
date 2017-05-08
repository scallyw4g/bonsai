#! /bin/bash

if [ "$WIN32" == "1" ]; then

  BINARY="bin/Bonsai"
  cd build
  make "$@" 2>&1 && mv ../bin/libGame.so ../bin/libGameLoadable.so

  # [ $? -eq 0 ] && ../$BINARY > /dev/tty

else # Win32

  BINARY="bin/Debug/Bonsai.exe"
  msbuild.exe ./bin/Game.vcxproj

  cd build
  make "$@" 2>&1 && mv ../bin/libGame.so ../bin/libGameLoadable.so

  [ $? -eq 0 ] && ../$BINARY > /dev/tty

fi

