#! /bin/bash

make

if [ $? -eq 0 ]; then
  cd build/
  gdb -ex run Bonsai >> /dev/tty
fi

