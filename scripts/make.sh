#! /bin/bash

if [ "$WIN32" == "1" ]; then
  echo " -- Building Win32"
  rm ../bin/Debug/*.pdb
  msbuild.exe /nologo /v:m ../bin/Bonsai.sln

else # Win32
  echo " -- Building Linux"
  cd build
  make DebugSystem Bonsai AnimationTest "$@" 2>&1
fi


