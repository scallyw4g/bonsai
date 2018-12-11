emcc src/font/ttf.cpp              \
  -D BONSAI_INTERNAL=1             \
  -I src                           \
  -I src/datatypes                 \
  -I /usr/include                  \
  -I /usr/include/x86_64-linux-gnu \
  -o bin/emscripten/ttf.html
