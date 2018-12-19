#! /bin/bash

if [ "$WIN32" == "1" ]; then
  echo " -- Building Win32"
  rm ../bin/Debug/*.pdb
  msbuild.exe /nologo /v:m ../bin/Bonsai.sln

else # Win32
  echo " -- Building Linux"
  cd build
  CMAKE_CXX_COMPILER="/usr/bin/clang" make Bonsai DebugSystem WorldGen "$@" 2>&1
fi

if [ "$EMCC" == "1" ]; then
  ./scripts/emcc.sh
fi


