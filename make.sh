#! /bin/bash

if [ "$WIN32" == "1" ]; then

  echo " -- Building Win32"
  rm bin/Debug/*.pdb

  BINARY="bin/Debug/Bonsai.exe"
  msbuild.exe /nologo /v:m ./bin/Game.vcxproj

  mv bin/Debug/Game.dll bin/Debug/GameLoadable.dll

  # [ $? -eq 0 ] && ../$BINARY > /dev/tty

else # Win32

  BINARY="bin/Debug/Bonsai.exe"
  msbuild.exe ./bin/Game.vcxproj

  cd build
  make "$@" 2>&1 && mv ../bin/libGame.so ../bin/libGameLoadable.so

  # [ $? -eq 0 ] && ../$BINARY > /dev/tty

fi

