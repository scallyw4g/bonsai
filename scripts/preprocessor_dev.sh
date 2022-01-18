#! /bin/bash

. scripts/preamble.sh

if [ "$Platform" == "Linux" ] ; then

  bin/preprocessor_dev                   \
    src/metaprogramming/preprocessor.cpp \
    -I src                               \
    -I /usr/include/x86_64-linux-gnu     \
    -I /usr/include

elif [ "$Platform" == "Windows" ] ; then

    # src/metaprogramming/preprocessor.cpp                                                                         \
    # src/bonsai_types.h                                                                         \

  bin/preprocessor_dev $COLORFLAG                                                                                           \
    src/metaprogramming/preprocessor.cpp                                                                         \
    -I ./src                                                                                                     \
    # -I "C:\Program Files\LLVM\lib\clang\11.0.0\include"                                                          \
    # -I "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.28.29333\include"         \
    # -I "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.28.29333\atlmfc\include"  \
    # -I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.18362.0\ucrt"                                        \
    # -I "C:\Program Files (x86)\Windows Kits\10\include\10.0.18362.0\shared"                                      \
    # -I "C:\Program Files (x86)\Windows Kits\10\include\10.0.18362.0\um"                                          \
    # -I "C:\Program Files (x86)\Windows Kits\10\include\10.0.18362.0\winrt"

else
  echo "Unknown platform"
fi
