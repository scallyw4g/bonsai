#! /bin/bash

WarningLevel="-Wall"

#Optimizations=""
Optimizations="-Ofast"

DebuggerSymbols="-ggdb"

Source="src/game.cpp -o build/game"

LinkerFlags="-lglfw -lGLEW -lGLU -lGL"

IncludeDirectories="
-I/home/scallywag/game/external/glfw-3.1.2/include/GLFW/
-I/home/scallywag/game/external/glm-0.9.7.1/
-I/home/scallywag/game/common/
-I/home/scallywag/game/src/
"

g++ $Optimizations $WarningLevel $DebuggerSymbols \
\
  $Source \
  $LinkerFlags \
  $IncludeDirectories


# Successful build, launch the thing!
if [ $? -eq 0 ]; then

  cd build/

  ./game >> /dev/tty

fi

