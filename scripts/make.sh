#! /bin/bash

# COMMON_OPTIMIZATION_OPTIONS="-O2"

RED="\x1b[31m"
BLUE="\x1b[34m"
GREEN="\x1b[32m"
YELLOW="\x1b[33m"
WHITE="\x1b[37m"

Delimeter="$RED-----------------------------------------------------------$WHITE"
Success="$GREEN  ✔ $WHITE"
Building="$BLUE  Building $WHITE"
Failed="$RED  ✗ $WHITE"

ROOT="."
SRC="$ROOT/src"
EXAMPLES="$ROOT/examples"
TESTS="$SRC/tests"
BIN="$ROOT/bin"
BIN_TEST="$BIN/tests"
META_OUT="$SRC/metaprogramming/output"

function SetOutputBinaryPathBasename()
{
  base_file="${1##*/}"
  output_basename="$2/${base_file%%.*}"
}

function ColorizeTitle()
{
  echo -e "$YELLOW$1$WHITE"
  echo -e ""
}

INCLUDE_DIRECTORIES="$SRC"
OUTPUT_DIRECTORY="$BIN"

# NOTE(Jesse): -Wno-global-constructors can be turned off when the defaultPallette
# in colors.h gets axed .. I think.

# TODO(Jesse, tags: build_pipeline): Investigate -Wcast-align situation

  # -fsanitize=address

COMMON_COMPILER_OPTIONS="
  -ferror-limit=2000
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
  -Wno-old-style-cast
  -Wno-float-equal
  -Wno-double-promotion
  -Wno-padded
  -Wno-global-constructors
  -Wno-cast-align
  -Wno-switch-enum
  -Wno-undef
  -Wno-covered-switch-default
  -Wno-c99-extensions
  -Wno-reserved-id-macro
"

COMMON_LINKER_OPTIONS="-lpthread -lX11 -ldl -lGL"
SHARED_LIBRARY_FLAGS="-shared -fPIC"

EXAMPLES_TO_BUILD="
  $EXAMPLES/world_gen
"

  # $EXAMPLES/animation_test
  # $EXAMPLES/ssao_test
  # $EXAMPLES/space_invaders

EXECUTABLES_TO_BUILD="
  $SRC/platform.cpp
  $SRC/font/ttf.cpp
  $SRC/net/server.cpp
"

# TODO(Jesse, tags: tests, release): The allocation tests crash in release mode because of some
# ultra-jank-tastic segfault recovery code.  Find another less janky way?
DEBUG_TESTS_TO_BUILD="
  $TESTS/allocation.cpp
"

TESTS_TO_BUILD="
  $TESTS/ui_command_buffer.cpp
  $TESTS/m4.cpp
  $TESTS/colladaloader.cpp
  $TESTS/test_bitmap.cpp
  $TESTS/chunk.cpp
  $TESTS/bonsai_string.cpp
  $TESTS/objloader.cpp
  $TESTS/callgraph.cpp
  $TESTS/heap_allocation.cpp
  $TESTS/preprocessor.cpp
  $TESTS/rng.cpp
  $TESTS/file.cpp
"

function BuildPreprocessor {

  which clang++ > /dev/null
  [ $? -ne 0 ] && echo -e "Please install clang++" && exit 1

  echo -e ""
  echo -e "$Delimeter"
  echo -e ""

  ColorizeTitle "Building Preprocessor"
  executable="$SRC/metaprogramming/preprocessor.cpp"
  SetOutputBinaryPathBasename "$executable" "$BIN"
  echo -e "$Building $executable"
  clang++                \
    $COMMON_OPTIMIZATION_OPTIONS \
    $COMMON_COMPILER_OPTIONS     \
    $COMMON_LINKER_OPTIONS       \
    -D BONSAI_INTERNAL=1         \
    -I"$SRC"                     \
    -o "$output_basename"        \
    $executable

  if [ $? -eq 0 ]; then
   echo -e "$Success $executable"
  else
   echo ""
   echo -e "$Failed Error building preprocessor, exiting."
   exit 1
  fi

  echo -e ""
  echo -e "$Delimeter"
  echo -e ""
}

function BuildWithClang {
  which clang++ > /dev/null
  [ $? -ne 0 ] && echo -e "Please install clang++" && exit 1

  echo -e ""
  echo -e "$Delimeter"
  echo -e ""

  ColorizeTitle "Executables"
  for executable in $EXECUTABLES_TO_BUILD; do
    SetOutputBinaryPathBasename "$executable" "$BIN"
    echo -e "$Building $executable"
    clang++                        \
      $COMMON_OPTIMIZATION_OPTIONS \
      $COMMON_COMPILER_OPTIONS     \
      $COMMON_LINKER_OPTIONS       \
      -D BONSAI_INTERNAL=1         \
      -I"$SRC"                     \
      -o "$output_basename"        \
      $executable && echo -e "$Success $executable" &
  done

  echo ""
  ColorizeTitle "Debug Tests"
  for executable in $DEBUG_TESTS_TO_BUILD; do
    SetOutputBinaryPathBasename "$executable" "$BIN_TEST"
    echo -e "$Building $executable"
    clang++                      \
      $COMMON_COMPILER_OPTIONS   \
      $COMMON_LINKER_OPTIONS     \
      -D BONSAI_INTERNAL=1       \
      -I"$SRC"                   \
      -o "$output_basename"      \
      $executable && echo -e "$Success $executable" &
  done

  echo ""
  ColorizeTitle "Tests"
  for executable in $TESTS_TO_BUILD; do
    SetOutputBinaryPathBasename "$executable" "$BIN_TEST"
    echo -e "$Building $executable"
    clang++                        \
      $COMMON_OPTIMIZATION_OPTIONS \
      $COMMON_COMPILER_OPTIONS     \
      $COMMON_LINKER_OPTIONS       \
      -D BONSAI_INTERNAL=1         \
      -I"$SRC"                     \
      -I"$SRC/debug_system"        \
      -o "$output_basename"        \
      $executable && echo -e "$Success $executable" &
  done

  echo ""
  ColorizeTitle "DebugSystem"
  DEBUG_SRC_FILE="$SRC/debug_system/debug.cpp"
  echo -e "$Building $DEBUG_SRC_FILE"
  clang++                          \
    $COMMON_OPTIMIZATION_OPTIONS   \
    $COMMON_COMPILER_OPTIONS       \
    $SHARED_LIBRARY_FLAGS          \
    $COMMON_LINKER_OPTIONS         \
    -D BONSAI_INTERNAL=1           \
    -I"$SRC"                       \
    -I"$SRC/debug_system"          \
    -o "$BIN/lib_debug_system.so"  \
    "$DEBUG_SRC_FILE" && echo -e "$Success $DEBUG_SRC_FILE" &

  echo ""
  ColorizeTitle "Examples"
  for executable in $EXAMPLES_TO_BUILD; do
    echo -e "$Building $executable"
    SetOutputBinaryPathBasename "$executable" "$BIN"
    clang++                                                     \
      $SHARED_LIBRARY_FLAGS                                     \
      $COMMON_OPTIMIZATION_OPTIONS                              \
      $COMMON_COMPILER_OPTIONS                                  \
      $COMMON_LINKER_OPTIONS                                    \
      -D BONSAI_INTERNAL=1                                      \
      -I"$SRC"                                                  \
      -I"$executable"                                           \
      -o "$output_basename"                                     \
      "$executable/game.cpp" &&                                 \
      mv "$output_basename" "$output_basename""_loadable.so" && \
      echo -e "$Success $executable" &
  done

  echo -e ""
  echo -e "$Delimeter"
  echo -e ""
  ColorizeTitle "Complete"

  wait

  echo -e ""
  echo -e "$Delimeter"
  echo -e ""

  echo -e ""
}

function BuildWithEmcc {
  which emcc > /dev/null
  [ $? -ne 0 ] && echo -e "Please install emcc" && exit 1

  emcc src/font/ttf.cpp              \
    -D BONSAI_INTERNAL=1             \
    -I src                           \
    -I /usr/include                  \
    -I /usr/include/x86_64-linux-gnu \
    -o bin/emscripten/ttf.html
}


if [ ! -d "$BIN" ]; then
  mkdir "$BIN"
fi

if [ ! -d "$BIN_TEST" ]; then
  mkdir "$BIN_TEST"
fi

if [ ! -d "$BIN_TEST" ]; then
  mkdir "$BIN_TEST"
fi

# SOURCE_FILES="src/metaprogramming/preprocessor.h"


function RunEntireBuild {

  # git checkout "src/metaprogramming/output"

  # rm -Rf $META_OUT
  # mkdir $META_OUT

  SOURCE_FILES="$(find src -type f -name "*.h" -and -not -wholename "src/metaprogramming/defines.h" | tr '\n' ' ') $(find src -type f -name "*.cpp" | tr '\n' ' ')"
  ColorizeTitle "Preprocessing"
  bin/preprocessor $SOURCE_FILES
  if [ $? -ne 0 ]; then
    echo ""
    echo -e "$Failed Preprocessing failed, exiting." 
    git checkout "src/metaprogramming/output"
    exit 1
  fi

  BuildPreprocessor
  [ ! -x bin/preprocessor ] && echo -e "$Failed Couldn't find preprocessor, exiting." && exit 1

  SOURCE_FILES="$(find src -type f -name "*.h" -and -not -wholename "src/metaprogramming/defines.h" | tr '\n' ' ') $(find src -type f -name "*.cpp" | tr '\n' ' ')"
  ColorizeTitle "Preprocessing"
  bin/preprocessor $SOURCE_FILES
  if [ $? -ne 0 ]; then
    echo ""
    echo -e "$Failed Preprocessing failed, exiting." 
    git checkout "src/metaprogramming/output"
    exit 1
  fi

  # if [ "$EMCC" == "1" ]; then
  #   BuildWithEmcc
  # else
  #   BuildWithClang
  # fi

  # ./scripts/run_tests.sh

  # SOURCE_FILES="$(find src -type f -name "*.h" -and -not -wholename "src/metaprogramming/defines.h" | tr '\n' ' ') $(find src -type f -name "*.cpp" | tr '\n' ' ')"
  # ColorizeTitle "Preprocessing"
  # bin/preprocessor $SOURCE_FILES
  # if [ $? -ne 0 ]; then
  #   echo ""
  #   echo -e "$Failed Preprocessing failed, exiting." 
  #   git checkout "src/metaprogramming/output"
  #   exit 1
  # fi

}

time RunEntireBuild
