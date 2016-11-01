#! /bin/bash

g++ -g3 -od src/game.cpp -o build/game \
-lglfw -lGLEW -lGLU -lGL \
-I/home/scallywag/game/external/glfw-3.1.2/include/GLFW/ \
-I/home/scallywag/game/external/glm-0.9.7.1/ \
-I/home/scallywag/game/common/ \
-I/home/scallywag/game/src/ \

# Successful build, launch the thing!
if [ $? -eq 0 ]; then

  cd build/

  ./game >> /dev/tty

fi

