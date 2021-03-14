#! /bin/bash

Platform="Unknown"
UNAME=$(uname)
if [ "$UNAME" == "Linux" ] ; then
  Platform="Linux"
  PLATFORM_LINKER_OPTIONS="-lpthread -lX11 -ldl -lGL"
  PLATFORM_DEFINES="-DBONSAI_LINUX"
  PLATFORM_INCLUDE_DIRS=""
  PLATFORM_CXX_OPTIONS="-ggdb"

  # TODO(Jesse): What does -fPIC acutally do?  I found the option documented here,
  # but with no explanation of what it's doing.  Apparently it's unsupported on
  # Windows, so hopefully it's not necessary for anything.

  # Turns out that -fPIC turns on rip-relative addressing (among other things?)
  # such that functions work regardless of where they're loaded into memory.
  # This is important (obviously) for dynamically-loaded libs and ASLR.

  #
  # https://clang.llvm.org/docs/ClangCommandLineReference.html
  # @unsupported_fPIC_flag_windows
  SHARED_LIBRARY_FLAGS="-shared -fPIC"
  PLATFORM_EXE_EXTENSION=""
  PLATFORM_LIB_EXTENSION=".so"

elif [[ "$UNAME" == CYGWIN* || "$UNAME" == MINGW* ]] ; then
  Platform="Windows"
  PLATFORM_LINKER_OPTIONS="-lgdi32 -luser32 -lopengl32 -lglu32"
  PLATFORM_DEFINES="-DBONSAI_WIN32"
  PLATFORM_INCLUDE_DIRS=""
  PLATFORM_CXX_OPTIONS="-g -gcodeview"

  # @unsupported_fPIC_flag_windows
  SHARED_LIBRARY_FLAGS="-shared"

  PLATFORM_EXE_EXTENSION=".exe"
  PLATFORM_LIB_EXTENSION=".dll"
fi


EMCC=0

# OPTIMIZATION_LEVEL="-O2"

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

# PREPROCESSOR_EXECUTABLE="bin/preprocessor_dev"
# PREPROCESSOR_EXECUTABLE="bin/preprocessor_current"

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

# NOTE(Jesse): -Wno-global-constructors can be turned off when the defaultPallette
# in colors.h gets axed .. I think.

# TODO(Jesse, tags: build_pipeline): Investigate -Wcast-align situation

  # -fsanitize=address

# Note(Jesse): Using -std=c++17 so I can mark functions with [[nodiscard]]

CXX_OPTIONS="
  --std=c++17
  -ferror-limit=2000

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
  $SRC/platform.cpp
  $SRC/font/ttf.cpp
"
  #$SRC/net/server.cpp


# TODO(Jesse, tags: tests, release): The allocation tests crash in release mode because of some
# ultra-jank-tastic segfault recovery code.  Find another less janky way?
DEBUG_TESTS_TO_BUILD="
  $TESTS/allocation.cpp
"


TESTS_TO_BUILD="
  $TESTS/preprocessor.cpp
"

# TESTS_TO_BUILD="
#   $TESTS/ui_command_buffer.cpp
#   $TESTS/m4.cpp
#   $TESTS/colladaloader.cpp
#   $TESTS/test_bitmap.cpp
#   $TESTS/chunk.cpp
#   $TESTS/bonsai_string.cpp
#   $TESTS/objloader.cpp
#   $TESTS/callgraph.cpp
#   $TESTS/heap_allocation.cpp
#   $TESTS/preprocessor.cpp
#   $TESTS/rng.cpp
#   $TESTS/file.cpp
# "

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
  clang++                                                \
    $OPTIMIZATION_LEVEL                                  \
    $CXX_OPTIONS                                         \
    $PLATFORM_CXX_OPTIONS                                \
    $PLATFORM_LINKER_OPTIONS                             \
    $PLATFORM_DEFINES                                    \
    $PLATFORM_INCLUDE_DIRS                               \
    -I"$SRC"                                             \
    -o "$output_basename""_dev""$PLATFORM_EXE_EXTENSION" \
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

function BuildExecutables
{
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
      -I"$SRC/debug_system"                          \
      -o "$output_basename""$PLATFORM_EXE_EXTENSION" \
      $executable && echo -e "$Success $executable" &
  done
}

function BuildDebugSystem
{
  echo ""
  ColorizeTitle "DebugSystem"
  DEBUG_SRC_FILE="$SRC/debug_system/debug.cpp"
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
    -I"$SRC/debug_system"                               \
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
  echo -e ""

  [ $BuildExecutables == 1 ] && BuildExecutables
  [ $BuildDebugTests == 1 ] && BuildDebugTests
  [ $BuildTests == 1 ] && BuildTests
  [ $BuildDebugSystem == 1 ] && BuildDebugSystem
  [ $BuildExamples == 1 ] && BuildExamples

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
    -I src/debug_system             \
    -I examples                     \
    src/platform.cpp                \
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

# SOURCE_FILES="src/metaprogramming/preprocessor.h"

function SetSourceFiles
{
  rm -Rf $META_OUT
  mkdir $META_OUT
  SOURCE_FILES="                                                   \
    $(find src -type f -name "*.h"                                 \
    -and -not -wholename "src/net/network.h"                       \
    -and -not -wholename "src/bonsai_stdlib/headers/stream.h"      \
    -and -not -wholename "src/bonsai_stdlib/headers/perlin.h"      \
    -and -not -wholename "src/bonsai_stdlib/headers/primitives.h"  \
    -and -not -wholename "src/metaprogramming/defines.h"           \
    -and -not -wholename "src/win32_platform.h"                    \
    -and -not -path      "src/tests/*" )                           \
                                                                   \
    $(find src -type f -name "*.cpp"                               \
    -and -not -wholename "src/bonsai_stdlib/cpp/stream.cpp"        \
    -and -not -wholename "src/net/server.cpp"                      \
    -and -not -wholename "src/win32_platform.cpp"                  \
    -and -not -path "src/tests/*" )                                \
  "
}

function RunPreprocessor
{
  SetSourceFiles
  ColorizeTitle "Preprocessing"
  if [ -x $PREPROCESSOR_EXECUTABLE ]; then
    $PREPROCESSOR_EXECUTABLE $SOURCE_FILES
    if [ $? -ne 0 ]; then
      echo ""
      echo -e "$Failed Preprocessing failed, exiting." 
      git checkout "src/metaprogramming/output"
      exit 1
    fi
  fi
}

function RunEntireBuild {

  if [ $DumpSourceFilesAndQuit == 1 ]; then
    SetSourceFiles
    echo "gdb --args bin/preprocessor_dev" $SOURCE_FILES
    exit 1
  fi

  if [ $CheckoutMetaOutput == 1 ]; then
    git checkout "src/metaprogramming/output"
  fi

  if [ $FirstPreprocessor == 1 ]; then
    PREPROCESSOR_EXECUTABLE="bin/preprocessor_current"
    RunPreprocessor
  fi

  if [ $BuildPreprocessor == 1 ]; then
    BuildPreprocessor
    [ ! -x $PREPROCESSOR_EXECUTABLE ] && echo -e "$Failed Couldn't find preprocessor, exiting." && exit 1
  fi

  if [ $SecondPreprocessor == 1 ]; then

    if [ "$Platform" == "Linux" ] ; then

      bin/preprocessor_dev                   \
        src/metaprogramming/preprocessor.cpp \
        -I src                               \
        -I /usr/include/x86_64-linux-gnu     \
        -I /usr/include

    elif [ "$Platform" == "Windows" ] ; then

      bin/preprocessor_dev                                                                                           \
        src/metaprogramming/preprocessor.cpp                                                                         \
        -I ./src                                                                                                     \
        -I "C:\Program Files\LLVM\lib\clang\11.0.0\include"                                                          \
        -I "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.28.29333\include"         \
        -I "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.28.29333\atlmfc\include"  \
        -I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.18362.0\ucrt"                                        \
        -I "C:\Program Files (x86)\Windows Kits\10\include\10.0.18362.0\shared"                                      \
        -I "C:\Program Files (x86)\Windows Kits\10\include\10.0.18362.0\um"                                          \
        -I "C:\Program Files (x86)\Windows Kits\10\include\10.0.18362.0\winrt"

    else
      echo "Unknown platform"
    fi
  fi

  if [ $EMCC == 1 ]; then
    BuildAllEMCC
  else
    BuildAllClang
  fi

  if [ $RunTests == 1 ]; then
    ./scripts/run_tests.sh
  fi

  if [ $FinalPreprocessor == 1 ]; then
    RunPreprocessor
  fi

}

DumpSourceFilesAndQuit=0

CheckoutMetaOutput=0

FirstPreprocessor=0
BuildPreprocessor=0
SecondPreprocessor=0

BuildExecutables=0
BuildDebugTests=0
BuildTests=1
BuildDebugSystem=0
BuildExamples=0

RunTests=0
FinalPreprocessor=0

time RunEntireBuild
