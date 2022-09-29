#! /bin/bash

BUILD_EVERYTHING=0

RunPoof=0

BuildExecutables=0
BuildExamples=0
BuildDebugSystem=0

BuildTests=0
BuildDebugTests=0

RunTests=0

MakeDebugLibRelease=1

. scripts/preamble.sh
. scripts/setup_for_cxx.sh

OPTIMIZATION_LEVEL="-O2"
EMCC=0


# ROOT="$(pwd)"
ROOT="."
SRC="$ROOT/src"
INCLUDE="$ROOT/include"
EXAMPLES="$ROOT/examples"
TESTS="$SRC/tests"
BIN="$ROOT/bin"
BIN_TEST="$BIN/tests"

BONSAI_INTERNAL='-D BONSAI_INTERNAL'


EXAMPLES_TO_BUILD="
  $EXAMPLES/the_wanderer
  $EXAMPLES/asset_picker
"
  #$EXAMPLES/world_gen

EXECUTABLES_TO_BUILD="
  $SRC/tools/asset_packer.cpp
  $SRC/game_loader.cpp
"
  # $SRC/font/ttf.cpp  # $SRC/net/server.cpp


# TODO(Jesse, tags: tests, release): The allocation tests crash in release mode because of some
# ultra-jank-tastic segfault recovery code.  Find another less janky way?
DEBUG_TESTS_TO_BUILD="
  $TESTS/allocation.cpp
"


DEBUG_LIB_RELEASE_DIR="releases/debug_lib"
function MakeDebugLibRelease
{
  [ ! -d releases ] && mkdir releases
  [ -d $DEBUG_LIB_RELEASE_DIR ] && rm -Rf $DEBUG_LIB_RELEASE_DIR && mkdir $DEBUG_LIB_RELEASE_DIR

  BuildDebugSystem
  [ $? -ne 0 ] && exit 1

  cat include/bonsai_debug/headers/public.h       >  $DEBUG_LIB_RELEASE_DIR/api.h
  cat include/bonsai_stdlib/headers/primitives.h  >> $DEBUG_LIB_RELEASE_DIR/api.h
  cat include/bonsai_debug/headers/api.h          >> $DEBUG_LIB_RELEASE_DIR/api.h

  cp "$BIN/lib_bonsai_debug""$PLATFORM_LIB_EXTENSION" $DEBUG_LIB_RELEASE_DIR
}


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
      $BONSAI_INTERNAL                               \
      $PLATFORM_CXX_OPTIONS                          \
      $PLATFORM_LINKER_OPTIONS                       \
      $PLATFORM_DEFINES                              \
      $PLATFORM_INCLUDE_DIRS                         \
      -I "$ROOT"                                     \
      -I "$SRC"                                      \
      -I "$INCLUDE"                             \
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
      $BONSAI_INTERNAL \
      $PLATFORM_CXX_OPTIONS                          \
      $PLATFORM_LINKER_OPTIONS                       \
      $PLATFORM_DEFINES                              \
      $PLATFORM_INCLUDE_DIRS                         \
      -I "$ROOT"                                     \
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
      $BONSAI_INTERNAL \
      $PLATFORM_CXX_OPTIONS                          \
      $PLATFORM_LINKER_OPTIONS                       \
      $PLATFORM_DEFINES                              \
      $PLATFORM_INCLUDE_DIRS                         \
      -I "$ROOT"                                     \
      -I "$SRC"                                      \
      -I "$INCLUDE"                              \
      -o "$output_basename""$PLATFORM_EXE_EXTENSION" \
      $executable && echo -e "$Success $executable" &
  done
}

function BuildDebugSystem
{
  echo ""
  ColorizeTitle "DebugSystem"
  DEBUG_SRC_FILE="$INCLUDE/bonsai_debug/debug.cpp"
  echo -e "$Building $DEBUG_SRC_FILE"
  clang++                                               \
    $OPTIMIZATION_LEVEL                                 \
    $CXX_OPTIONS                                        \
    $BONSAI_INTERNAL                                    \
    $PLATFORM_CXX_OPTIONS                               \
    $PLATFORM_LINKER_OPTIONS                            \
    $PLATFORM_DEFINES                                   \
    $PLATFORM_INCLUDE_DIRS                              \
    $SHARED_LIBRARY_FLAGS                               \
    -I "$ROOT"                                          \
    -I "$SRC"                                           \
    -I "$INCLUDE"                                       \
    -o "$BIN/lib_bonsai_debug""$PLATFORM_LIB_EXTENSION" \
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
      $BONSAI_INTERNAL                                                                \
      -D BONSAI_DEBUG_SYSTEM_API=1                                                    \
      $PLATFORM_CXX_OPTIONS                                                           \
      $PLATFORM_LINKER_OPTIONS                                                        \
      $PLATFORM_DEFINES                                                               \
      $PLATFORM_INCLUDE_DIRS                                                          \
      $SHARED_LIBRARY_FLAGS                                                           \
      -I "$ROOT"                                                                      \
      -I "$INCLUDE"                                                                   \
      -I "$SRC"                                                                       \
      -I "$executable"                                                                \
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
    $BONSAI_INTERNAL \
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

  if [ $MakeDebugLibRelease == 1 ]; then
    MakeDebugLibRelease
  fi

  if [ $RunPoof == 1 ]; then
    RunPoof
  fi

  if [ $EMCC == 1 ]; then
    BuildAllEMCC
  else
    BuildAllClang
  fi

  if [ $RunTests == 1 ]; then
    ./scripts/run_tests.sh
  fi

}

function RunPoofHelper {

   # --log-level LogLevel_Debug   \
  poof                          \
   -I src/                      \
   -I include/                  \
   -D BONSAI_LINUX              \
   $BONSAI_INTERNAL             \
   -o generated                 \
   $1

}

function RunPoof
{
  echo -e ""
  echo -e "$Delimeter"
  echo -e ""

  ColorizeTitle "Poofing"
  [ -d src/generated ] && rm -Rf src/generated
  [ -d generated ] && rm -Rf generated

  RunPoofHelper src/game_loader.cpp && echo -e "$Success poofed src/game_loader.cpp" &
  RunPoofHelper include/bonsai_debug/debug.cpp && echo -e "$Success poofed src/include/bonsai_debug/debug.cpp" &
  RunPoofHelper examples/asset_picker/game.cpp && echo -e "$Success poofed examples/asset_picker/game.cpp" &
  # RunPoofHelper examples/the_wanderer/game.cpp && echo -e "$Success poofed examples/the_wanderer/game.cpp" &
  RunPoofHelper src/tools/asset_packer.cpp && echo -e "$Success poofed src/tools/asset_packer.cpp" &

  wait

  [ -d tmp ] && rmdir tmp
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
#   $TESTS/bonsai_string.cpp
#   $TESTS/objloader.cpp
#   $TESTS/callgraph.cpp
#   $TESTS/heap_allocation.cpp
#   $TESTS/rng.cpp
#   $TESTS/file.cpp
#   "
# fi

time RunEntireBuild
