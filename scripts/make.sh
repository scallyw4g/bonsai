#! /bin/bash

RED="\x1b[31m"
BLUE="\x1b[34m"
GREEN="\x1b[32m"
YELLOW="\x1b[33m"
WHITE="\x1b[37m"

Delimeter="$RED""-----------------------------------------------------------""$WHITE"
Done="$GREEN""Done""$WHITE"
Building="$BLUE""Building""$WHITE"

ROOT="."
SRC="$ROOT/src"
EXAMPLES="$ROOT/examples"
TESTS="$ROOT/tests"
BIN="$ROOT/bin"
BIN_TEST="$BIN/tests"

function SetOutputBinaryPathBasename()
{
  base_file="${1##*/}"
  output_basename="$2/${base_file%%.*}"
}

function ColorizeTitle()
{
  echo -e "$YELLOW""$1""$WHITE"
}

INCLUDE_DIRECTORIES="$SRC"
OUTPUT_DIRECTORY="$BIN"


# NOTE(Jesse): -Wno-global-constructors can be turned off when the defaultPallette
# in colors.h gets axed .. I think.

# TODO(Jesse): Investigate -Wcast-align situation

COMMON_COMPILER_OPTIONS="
  -ggdb
  -Weverything
  -Wno-c++98-compat-pedantic
  -Wno-gnu-anonymous-struct
  -Wno-missing-prototypes
  -Wno-zero-as-null-pointer-constant
  -Wno-format-nonliteral
  -Wno-cast-qual
  -Wno-unused-function
  -Wno-four-char-constants
  -Wno-unused-variable
  -Wno-old-style-cast
  -Wno-float-equal
  -Wno-double-promotion
  -Wno-padded
  -Wno-global-constructors
  -Wno-cast-align
  -Wno-switch-enum
  -Wno-undef
  -Wno-covered-switch-default
"

COMMON_LINKER_OPTIONS="-lpthread -lX11 -ldl -lGL"
COMMON_GL_DEFINES="-D GL_GLEXT_PROTOTYPES=1"
SHARED_LIBRARY_FLAGS="-shared -fPIC"

EXAMPLES_TO_BUILD="
  $EXAMPLES/world_gen
"

EXECUTABLES_TO_BUILD="
  $SRC/platform.cpp
  $SRC/font/ttf.cpp
  $SRC/net/server.cpp
"

TESTS_TO_BUILD="
  $TESTS/m4.cpp
  $TESTS/colladaloader.cpp
  $TESTS/bitmap.cpp
  $TESTS/chunk.cpp
  $TESTS/bonsai_string.cpp
  $TESTS/objloader.cpp
  $TESTS/allocation.cpp
  $TESTS/callgraph.cpp
  $TESTS/heap_allocation.cpp
"

if [ ! -d "$BIN" ]; then
  mkdir "$BIN"
fi

if [ ! -d "$BIN_TEST" ]; then
  mkdir "$BIN_TEST"
fi

if [ "$EMCC" == "1" ]; then

  which emcc > /dev/null
  [ $? -ne 0 ] && echo -e "Please install emcc" && exit 1

  emcc src/font/ttf.cpp              \
    -D BONSAI_INTERNAL=1             \
    -I src                           \
    -I src/datatypes                 \
    -I /usr/include                  \
    -I /usr/include/x86_64-linux-gnu \
    -o bin/emscripten/ttf.html

else

  which clang++ > /dev/null
  [ $? -ne 0 ] && echo -e "Please install clang++" && exit 1

  echo -e ""
  echo -e "$Delimeter"
  echo -e ""

  ColorizeTitle "Executables"
  for executable in $EXECUTABLES_TO_BUILD; do
    SetOutputBinaryPathBasename "$executable" "$BIN"
    echo -e "$Building $executable"
    clang++                    \
      $COMMON_COMPILER_OPTIONS \
      $COMMON_LINKER_OPTIONS   \
      $COMMON_GL_DEFINES       \
      -D BONSAI_INTERNAL=1     \
      -I"$SRC"                 \
      -I"$SRC/datatypes"       \
      -o "$output_basename"    \
      $executable && echo -e "$Done $executable" &
  done

  echo ""
  ColorizeTitle "Tests"
  for executable in $TESTS_TO_BUILD; do
    SetOutputBinaryPathBasename "$executable" "$BIN_TEST"
    echo -e "$Building $executable"
    clang++                      \
      $COMMON_COMPILER_OPTIONS   \
      $COMMON_LINKER_OPTIONS     \
      $COMMON_GL_DEFINES         \
      -D BONSAI_INTERNAL=1       \
      -I"$SRC"                   \
      -I"$SRC/datatypes"         \
      -I"$TESTS"                 \
      -o "$output_basename" \
      $executable && echo -e "$Done $executable" &
  done

  echo ""
  ColorizeTitle "DebugSystem"
  DEBUG_SRC_FILE="$SRC/debug_system/debug.cpp"
  echo -e "$Building $DEBUG_SRC_FILE"
  clang++                         \
    $COMMON_COMPILER_OPTIONS      \
    $SHARED_LIBRARY_FLAGS         \
    $COMMON_LINKER_OPTIONS        \
    $COMMON_GL_DEFINES            \
    -D BONSAI_INTERNAL=1          \
    -I"$SRC"                      \
    -I"$SRC/GL"                   \
    -I"$SRC/datatypes"            \
    -I"$SRC/debug_system"         \
    -o "$BIN/lib_debug_system.so" \
    "$DEBUG_SRC_FILE" && echo -e "$Done $executable" &

  echo ""
  ColorizeTitle "Examples"
  for executable in $EXAMPLES_TO_BUILD; do
    echo -e "$Building $executable"
    SetOutputBinaryPathBasename "$executable" "$BIN"

    clang++                                                     \
      $SHARED_LIBRARY_FLAGS                                     \
      $COMMON_COMPILER_OPTIONS                                  \
      $COMMON_LINKER_OPTIONS                                    \
      $COMMON_GL_DEFINES                                        \
      -D BONSAI_INTERNAL=1                                      \
      -I"$SRC"                                                  \
      -I"$SRC/datatypes"                                        \
      -I"$executable"                                           \
      -o "$output_basename"                                     \
      "$executable/game.cpp" &&                                 \
      mv "$output_basename" "$output_basename""_loadable.so" && \
      echo -e "$Done $executable" &
  done

  echo -e ""
  echo -e "$Delimeter"
  echo -e ""

  wait

  echo -e ""

fi


