#! /bin/bash

make

if [ $? -eq 0 ]; then
  cd build/
  ./Bonsai >> /dev/tty
fi

