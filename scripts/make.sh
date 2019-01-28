#! /bin/bash

cd build

which cmake > /dev/null
[ $? -ne 0 ] && echo "Please install Cmake" && exit 1

which clang > /dev/null
[ $? -ne 0 ] && echo "Please install Clang" && exit 1

if [ ! -f Makefile ]; then
  echo " -- First Time - Running CMAKE"
  CMAKE_CXX_COMPILER="/usr/bin/clang" cmake .
fi

echo " -- Building Linux"
make -j32 all "$@" 2>&1

if [ "$EMCC" == "1" ]; then
  ./scripts/emcc.sh
fi


