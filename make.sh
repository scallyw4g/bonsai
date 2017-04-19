#! /bin/bash

make 2>&1

if [ $? -eq 0 ]; then
  ./build/Bonsai.exe > /dev/tty
fi

