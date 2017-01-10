#! /bin/bash

WarningLevel="-Wall"

Optimizations=""
#Optimizations="-Ofast"

DebuggerSymbols="-ggdb"

Source="src/game.cpp -o build/bonsai"

LinkerFlags="-lglfw -lGLEW -lGLU -lGL"

IncludeDirectories="
-I./external/glfw-3.1.2/include/GLFW/
-I./external/glfw-3.1.2/include/GLFW/
-I./external/glm-0.9.7.1/
-I./common/
-I./src/
"

g++ $Optimizations $WarningLevel $DebuggerSymbols \
\
  $Source \
  $LinkerFlags \
  $IncludeDirectories


# Successful build, launch the thing!
if [ $? -eq 0 ]; then

  cd build/

  ./bonsai >> /dev/tty

fi

