#! /bin/bash

BUILD_EVERYTHING=0

CheckoutMetaOutput=0

RunPoof=1

BuildExecutables=0
BuildDebugTests=0
BuildTests=0
BuildDebugSystem=0
BuildExamples=0

RunTests=0

. scripts/preamble.sh
. scripts/setup_for_cxx.sh

OPTIMIZATION_LEVEL="-O0"
EMCC=0


ROOT="."
SRC="$ROOT/src"
EXAMPLES="$ROOT/examples"
TESTS="$SRC/tests"
BIN="$ROOT/bin"
BIN_TEST="$BIN/tests"

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

# TODO(Jesse, tags: build_pipeline): Investigate -Wcast-align situation

  # -fsanitize=address

# Note(Jesse): Using -std=c++17 so I can mark functions with [[nodiscard]]

# TODO(Jesse): Figure out how to standardize on a compiler across machines such that
# we can remove -Wno-unknown-warning-optins
CXX_OPTIONS="
  --std=c++17
  -ferror-limit=2000

  -Weverything

  -Wno-unknown-warning-option

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
  -Wno-dollar-in-identifier-extension

  -Wno-class-varargs

  -Wno-unused-value
  -Wno-unused-variable
  -Wno-unused-parameter

  -Wno-implicit-int-float-conversion
  -Wno-extra-semi-stmt
  -Wno-reorder-init-list
  -Wno-unused-macros
  -Wno-atomic-implicit-seq-cst
"


EXAMPLES_TO_BUILD="
  $EXAMPLES/world_gen
  $EXAMPLES/building
"

  # $EXAMPLES/animation_test
  # $EXAMPLES/ssao_test
  # $EXAMPLES/space_invaders

EXECUTABLES_TO_BUILD="
  $SRC/game_loader.cpp
  $SRC/font/ttf.cpp
"
  #$SRC/net/server.cpp


# TODO(Jesse, tags: tests, release): The allocation tests crash in release mode because of some
# ultra-jank-tastic segfault recovery code.  Find another less janky way?
DEBUG_TESTS_TO_BUILD="
  $TESTS/allocation.cpp
"

function BuildExecutables
{
  echo ""
  ColorizeTitle "Executables"
  for executable in $EXECUTABLES_TO_BUILD; do
    SetOutputBinaryPathBasename "$executable" "$BIN"
    echo -e "$Building $executable"
    clang++                                          \
      $OPTIMIZATION_LEVEL                            \
      $CXX_OPTIONS                                   \
      $PLATFORM_CXX_OPTIONS                          \
      $PLATFORM_LINKER_OPTIONS                       \
      $PLATFORM_DEFINES                              \
      $PLATFORM_INCLUDE_DIRS                         \
      -I"$SRC"                                       \
      -o "$output_basename""$PLATFORM_EXE_EXTENSION" \
      $executable && echo -e "$Success $executable" &
  done
}

function BuildDebugTests
{
  echo ""
  ColorizeTitle "Debug Tests"
  for executable in $DEBUG_TESTS_TO_BUILD; do
    SetOutputBinaryPathBasename "$executable" "$BIN_TEST"
    echo -e "$Building $executable"
    clang++                                          \
      $CXX_OPTIONS                                   \
      $PLATFORM_CXX_OPTIONS                          \
      $PLATFORM_LINKER_OPTIONS                       \
      $PLATFORM_DEFINES                              \
      $PLATFORM_INCLUDE_DIRS                         \
      -I"$SRC"                                       \
      -o "$output_basename""$PLATFORM_EXE_EXTENSION" \
      $executable && echo -e "$Success $executable" &
  done
}

function BuildTests
{
  echo ""
  ColorizeTitle "Tests"
  for executable in $TESTS_TO_BUILD; do
    SetOutputBinaryPathBasename "$executable" "$BIN_TEST"
    echo -e "$Building $executable"
    clang++                                          \
      $OPTIMIZATION_LEVEL                            \
      $CXX_OPTIONS                                   \
      $PLATFORM_CXX_OPTIONS                          \
      $PLATFORM_LINKER_OPTIONS                       \
      $PLATFORM_DEFINES                              \
      $PLATFORM_INCLUDE_DIRS                         \
      -I"$SRC"                                       \
      -I"$SRC/bonsai_debug"                          \
      -o "$output_basename""$PLATFORM_EXE_EXTENSION" \
      $executable && echo -e "$Success $executable" &
  done
}

function BuildDebugSystem
{
  echo ""
  ColorizeTitle "DebugSystem"
  DEBUG_SRC_FILE="$SRC/bonsai_debug/debug.cpp"
  echo -e "$Building $DEBUG_SRC_FILE"
  clang++                                               \
    $OPTIMIZATION_LEVEL                                 \
    $CXX_OPTIONS                                        \
    $PLATFORM_CXX_OPTIONS                               \
    $PLATFORM_LINKER_OPTIONS                            \
    $PLATFORM_DEFINES                                   \
    $PLATFORM_INCLUDE_DIRS                              \
    $SHARED_LIBRARY_FLAGS                               \
    -I"$SRC"                                            \
    -I"$SRC/bonsai_debug"                               \
    -o "$BIN/lib_debug_system""$PLATFORM_LIB_EXTENSION" \
    "$DEBUG_SRC_FILE" && echo -e "$Success $DEBUG_SRC_FILE" &
}

function BuildExamples
{
  echo ""
  ColorizeTitle "Examples"
  for executable in $EXAMPLES_TO_BUILD; do
    echo -e "$Building $executable"
    SetOutputBinaryPathBasename "$executable" "$BIN"
    clang++                                                                           \
      $OPTIMIZATION_LEVEL                                                             \
      $CXX_OPTIONS                                                                    \
      $PLATFORM_CXX_OPTIONS                                                           \
      $PLATFORM_LINKER_OPTIONS                                                        \
      $PLATFORM_DEFINES                                                               \
      $PLATFORM_INCLUDE_DIRS                                                          \
      $SHARED_LIBRARY_FLAGS                                                           \
      -I"$SRC"                                                                        \
      -I"$executable"                                                                 \
      -o "$output_basename"                                                           \
      "$executable/game.cpp" &&                                                       \
      mv "$output_basename" "$output_basename""_loadable""$PLATFORM_LIB_EXTENSION" && \
      echo -e "$Success $executable" &
  done
}

function BuildAllClang
{
  which clang++ > /dev/null
  [ $? -ne 0 ] && echo -e "Please install clang++" && exit 1

  echo -e ""
  echo -e "$Delimeter"

  [[ $BuildExecutables == 1 || $BUILD_EVERYTHING == 1 ]] && BuildExecutables
  [[ $BuildDebugTests == 1  || $BUILD_EVERYTHING == 1 ]] && BuildDebugTests
  [[ $BuildTests == 1       || $BUILD_EVERYTHING == 1 ]] && BuildTests
  [[ $BuildDebugSystem == 1 || $BUILD_EVERYTHING == 1 ]] && BuildDebugSystem
  [[ $BuildExamples == 1    || $BUILD_EVERYTHING == 1 ]] && BuildExamples

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

function BuildAllEMCC {
  which emcc > /dev/null
  [ $? -ne 0 ] && echo -e "Please install emcc" && exit 1

  emcc                              \
    -s WASM=1                       \
    -s LLD_REPORT_UNDEFINED         \
    -s FULL_ES3=1                   \
    -s ALLOW_MEMORY_GROWTH=1        \
    -s ASSERTIONS=1                 \
    -s DEMANGLE_SUPPORT=1           \
    -std=c++17                      \
    -Wno-c99-designator             \
    -Wno-reorder-init-list          \
    -ferror-limit=2000              \
    -fno-exceptions                 \
    -O2                             \
    -g4                             \
    --source-map-base /             \
    --emrun                         \
    -msse                           \
    -msimd128                       \
    -DEMCC=1                        \
    -DWASM=1                        \
    -I src                          \
    -I src/bonsai_debug             \
    -I examples                     \
    src/game_loader.cpp                \
    -o bin/wasm/platform.html

    # --embed-file shaders     \
    # --embed-file models      \

}


if [ ! -d "$BIN" ]; then
  mkdir "$BIN"
fi

if [ ! -d "$BIN/wasm" ]; then
  mkdir "$BIN/wasm"
fi


if [ ! -d "$BIN_TEST" ]; then
  mkdir "$BIN_TEST"
fi

if [ ! -d "$BIN_TEST" ]; then
  mkdir "$BIN_TEST"
fi

function RunEntireBuild {

  if [ $CheckoutMetaOutput == 1 ]; then
    git checkout "src/generated"
  fi

  if [ $EMCC == 1 ]; then
    BuildAllEMCC
  else
    BuildAllClang
  fi

  if [ $RunTests == 1 ]; then
    ./scripts/run_tests.sh
  fi

  if [ $RunPoof == 1 ]; then
    RunPoof
  fi


}

function RunPoof {

  poof                         \
   --log-level LogLevel_Debug  \
   -I src/                     \
   -I include/                 \
   -D BONSAI_LINUX             \
   -o src/generated            \
   src/game_loader.cpp

}


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
  $TESTS/rng.cpp
  $TESTS/file.cpp
"

# if [[ $BUILD_EVERYTHING == 0 ]]; then
#   TESTS_TO_BUILD="
#   "
# fi

time RunEntireBuild
