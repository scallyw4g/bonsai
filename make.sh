#! /bin/bash

if [ -z $WIN32 ]; then # Unix

  BINARY="bin/Bonsai"

  cd build
  make "$@" 2>&1 && mv ../bin/libGame.so ../bin/libGameLoadable.so

  # [ $? -eq 0 ] && ../$BINARY > /dev/tty

else # Win32

  BINARY="bin/Debug/Bonsai.exe"
  msbuild.exe ./bin/Game.vcxproj

  cp ./bin/Debug/Game.dll ./bin/Debug/GameLoadable.dll


fi

