#! /bin/bash

WarningLevel="-Wall"

Optimizations=""
#Optimizations="-Ofast"

DebuggerSymbols="-ggdb"

if [ "$WIN32" == "1" ]; then

  Source="src/game.cpp"

  LinkerDirectories="/link /LIBPATH:./lib/"

  IncludeDirectories="
    /I external/glm-0.9.7.1/
    /I src
    /I src/GLFW
    /I src/GL
  "

  cl /MD /EHsc                  \
      /D_WIN32=1                \
      /DGLEW_BUILD=GLEW_STATIC  \
      $Source                   \
      $Optimizations            \
      $IncludeDirectories       \
      $LinkerDirectories        \
      lib/glfw3.lib             \
      Gdi32.lib                 \
      User32.lib                \
      Shell32.lib               \
      Opengl32.lib

  cp game.exe build/bonsai.exe
  rm game.*

else

  Source="src/game.cpp -o build/bonsai"

  LinkerFlags="-lglfw -lGLEW -lGLU -lGL"

  IncludeDirectories="
  -I./external/glm-0.9.7.1/
  -I./common/
  -I./src/GLFW/
  -I./src/
  "

  g++ -DLINUX=1         \
      $Optimizations    \
      $WarningLevel     \
      $DebuggerSymbols  \
      $Source           \
      $LinkerFlags      \
      $IncludeDirectories


fi

# Successful build, launch the thing!
if [ $? -eq 0 ]; then

  cd build/

  ./bonsai >> /dev/tty

fi

