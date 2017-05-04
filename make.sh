#! /bin/bash

if [ $WIN32 -eq 1 ]; then

  BINARY="bin/Debug/Bonsai.exe"
  msbuild.exe ./bin/Game.vcxproj

  cp ./bin/Debug/Game.dll ./bin/Debug/GameLoadable.dll

else # Unix

  BINARY="bin/Bonsai"

  cd build
  make "$@" 2>&1 && cp ./bin/Game.so ./bin/GameLoadable.so

  [ $? -eq 0 ] && ./$BINARY > /dev/tty

fi

