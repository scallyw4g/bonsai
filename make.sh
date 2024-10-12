#!/usr/bin/env bash

# TODO(Jesse)(build): Revisit this blog and see if there are some tidbits we can use.
# http://ptspts.blogspot.com/2013/12/how-to-make-smaller-c-and-c-binaries.html


# SANITIZER="-fsanitize=undefined"
# SANITIZER="-fsanitize=address"

BUILD_EVERYTHING=0

RunPoof=0

BuildExecutables=0
BuildExamples=0

BuildTests=0
BuildDebugOnlyTests=0

RunTests=0

stdlib_build_scripts='external/bonsai_stdlib/scripts'
source $stdlib_build_scripts/preamble.sh
source $stdlib_build_scripts/setup_for_cxx.sh


OPTIMIZATION_LEVEL=""
EMCC=0


# NOTE(Jesse): Can't do this cause it fucks with the paths poof outputs
# ROOT="$(pwd)"
ROOT="."
SRC="$ROOT/src"
INCLUDE="$ROOT/external"
EXAMPLES="$ROOT/examples"
TESTS="$SRC/tests"
BIN="$ROOT/bin"
BIN_TEST="$BIN/tests"
BIN_GAME_LIBS="$BIN/game_libs"

BONSAI_INTERNAL='-D BONSAI_INTERNAL=1'
# BONSAI_INTERNAL=''

# $EXAMPLES/tile_gen

EXAMPLES_TO_BUILD=""

BUNDLED_EXAMPLES="
  $EXAMPLES/blank_project
  $EXAMPLES/turn_based
  $EXAMPLES/the_wanderer
  $EXAMPLES/terrain_gen
  $EXAMPLES/transparency
  $EXAMPLES/tools/voxel_synthesis_rule_baker
  $EXAMPLES/project_and_level_picker
"

EXECUTABLES_TO_BUILD="
  $SRC/game_loader.cpp
  $SRC/font/ttf.cpp
"
  # $SRC/tools/asset_packer.cpp
  # $SRC/net/server.cpp


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
      $SANITIZER                                     \
      $OPTIMIZATION_LEVEL                            \
      $CXX_OPTIONS                                   \
      $BONSAI_INTERNAL                               \
      $PLATFORM_CXX_OPTIONS                          \
      $PLATFORM_LINKER_OPTIONS                       \
      $PLATFORM_DEFINES                              \
      $PLATFORM_INCLUDE_DIRS                         \
      -I "$ROOT"                                     \
      -I "$SRC"                                      \
      -I "$INCLUDE"                                  \
      -o "$output_basename""$PLATFORM_EXE_EXTENSION" \
      $executable &

    TrackPid "$executable" $!

  done
}

function BuildDebugOnlyTests
{
  echo ""
  ColorizeTitle "Debug Tests"
  for executable in $DEBUG_TESTS_TO_BUILD; do
    SetOutputBinaryPathBasename "$executable" "$BIN_TEST"
    echo -e "$Building $executable"
    clang++                                          \
      $CXX_OPTIONS                                   \
      $BONSAI_INTERNAL                               \
      $PLATFORM_CXX_OPTIONS                          \
      $PLATFORM_LINKER_OPTIONS                       \
      $PLATFORM_DEFINES                              \
      $PLATFORM_INCLUDE_DIRS                         \
      -I "$ROOT"                                     \
      -I "$SRC"                                      \
      -I "$INCLUDE"                                  \
      -o "$output_basename""$PLATFORM_EXE_EXTENSION" \
      $executable &

    TrackPid "$executable" $!

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
      $BONSAI_INTERNAL                               \
      $PLATFORM_CXX_OPTIONS                          \
      $PLATFORM_LINKER_OPTIONS                       \
      $PLATFORM_DEFINES                              \
      $PLATFORM_INCLUDE_DIRS                         \
      -I "$ROOT"                                     \
      -I "$SRC"                                      \
      -I "$INCLUDE"                                  \
      -o "$output_basename""$PLATFORM_EXE_EXTENSION" \
      $executable &
    TrackPid "$executable" $!
  done
}

function BuildExamples
{
  echo ""
  ColorizeTitle "Examples"
  for executable in $EXAMPLES_TO_BUILD; do
    echo -e "$Building $executable"
    SetOutputBinaryPathBasename "$executable" "$BIN_GAME_LIBS"
    clang++                     \
      $SANITIZER                \
      -D BONSAI_DEBUG_SYSTEM_API=1 \
      $OPTIMIZATION_LEVEL       \
      $CXX_OPTIONS              \
      $BONSAI_INTERNAL          \
      $PLATFORM_CXX_OPTIONS     \
      $PLATFORM_LINKER_OPTIONS  \
      $PLATFORM_DEFINES         \
      $PLATFORM_INCLUDE_DIRS    \
      $SHARED_LIBRARY_FLAGS     \
      -I "$ROOT"                \
      -I "$INCLUDE"             \
      -I "$SRC"                 \
      -I "$executable"          \
      -o "$output_basename"     \
      "$executable/game.cpp" && \
      mv "$output_basename" "$output_basename""_loadable""$PLATFORM_LIB_EXTENSION" &
    TrackPid "$executable" $!
  done
}

function BuildWithClang
{
  which clang++ > /dev/null
  [ $? -ne 0 ] && echo -e "Please install clang++" && exit 1

  echo -e ""
  echo -e "$Delimeter"

  [[ $BuildExecutables == 1     || $BUILD_EVERYTHING == 1 ]] && BuildExecutables
  [[ $BuildDebugOnlyTests == 1  || $BUILD_EVERYTHING == 1 ]] && BuildDebugOnlyTests
  [[ $BuildTests == 1           || $BUILD_EVERYTHING == 1 ]] && BuildTests
  [[ $BuildExamples == 1        || $BUILD_EVERYTHING == 1 ]] && BuildExamples

  echo -e ""
  echo -e "$Delimeter"
  echo -e ""
  ColorizeTitle "Complete"

  WaitForTrackedPids
  sync

  echo -e ""
  echo -e "$Delimeter"
  echo -e ""

  echo -e ""
}

function BuildWithEMCC {
  which emcc > /dev/null 2&> 1
  [ $? -ne 0 ] && echo -e "$Error Please install emcc" && exit 1

  emcc                       \
    -s WASM=1                \
    -s LLD_REPORT_UNDEFINED  \
    -s FULL_ES3=1            \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s ASSERTIONS=1          \
    -s DEMANGLE_SUPPORT=1    \
    -std=c++17               \
    -Wno-c99-designator      \
    -Wno-reorder-init-list   \
    -ferror-limit=2000       \
    -fno-exceptions          \
    -O2                      \
    -g4                      \
    --source-map-base /      \
    --emrun                  \
    -msse                    \
    -msimd128                \
    -DEMCC=1                 \
    -DWASM=1                 \
    $BONSAI_INTERNAL         \
    -I src                   \
    -I examples              \
    src/game_loader.cpp      \
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

if [ ! -d "$BIN_GAME_LIBS" ]; then
  mkdir "$BIN_GAME_LIBS"
fi

if [ ! -d "$BIN_TEST" ]; then
  mkdir "$BIN_TEST"
fi

if [ ! -d "$BIN_TEST" ]; then
  mkdir "$BIN_TEST"
fi

function RunEntireBuild {

  if [ $RunPoof == 1 ]; then
    RunPoof
    [ $? -ne 0 ] && exit 1
  fi

  if [ $EMCC == 1 ]; then
    BuildWithEMCC
    [ $? -ne 0 ] && exit 1
  else
    BuildWithClang
    [ $? -ne 0 ] && exit 1
  fi

  if [ $RunTests == 1 ]; then
    $stdlib_build_scripts/run_tests.sh
    [ $? -ne 0 ] && exit 1
  fi

}


# --log-level LogLevel_Debug -I src/ -I external/ -I "C:/Program Files/LLVM/lib/clang/11.0.0/include" -I "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.28.29333/include" -I "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.28.29333/atlmfc/include" -I "C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/ucrt" -I "C:/Program Files (x86)/Windows Kits/10/include/10.0.18362.0/shared" -I "C:/Program Files (x86)/Windows Kits/10/include/10.0.18362.0/um" -I "C:/Program Files (x86)/Windows Kits/10/include/10.0.18362.0/winrt" -D BONSAI_WIN32 -D BONSAI_INTERNAL -o generated src/game_loader.cpp

function RunPoofHelper {

   # -D _M_CEE_PURE \

   # -I "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.28.29333/include"        \
   # -I "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.28.29333/atlmfc/include" \

   # -I "C:/Program Files/LLVM/lib/clang/11.0.0/include"                                                         \
   # -I "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.28.29333/include"        \
   # -I "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.28.29333/atlmfc/include" \
   # -I "C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/ucrt"                                       \
   # -I "C:/Program Files (x86)/Windows Kits/10/include/10.0.18362.0/shared"                                     \
   # -I "C:/Program Files (x86)/Windows Kits/10/include/10.0.18362.0/um"                                         \
   # -I "C:/Program Files (x86)/Windows Kits/10/include/10.0.18362.0/winrt"                                      \




   # --log-level LogLevel_Debug                                                                                  \
  poof                    \
   $COLOR_FLAG \
   -D POOF_PREPROCESSOR   \
   -D BONSAI_PREPROCESSOR \
   -I src/                \
   -I external/           \
   $PLATFORM_DEFINES      \
   $BONSAI_INTERNAL       \
   -o generated           \
   $1

}

function RunPoof
{
  echo -e ""
  echo -e "$Delimeter"
  echo -e ""

  ColorizeTitle "Poofing"

  # [ -d src/generated ] && rm -Rf src/generated
  # [ -d generated ] && rm -Rf generated

  # RunPoofHelper src/game_loader.cpp && echo -e "$Success poofed src/game_loader.cpp" &
  # TrackPid "" $!

  # RunPoofHelper examples/turn_based/game.cpp && echo -e "$Success poofed examples/turn_based/game.cpp" &
  # TrackPid "" $!

  RunPoofHelper examples/terrain_gen/game.cpp && echo -e "$Success poofed examples/terrain_gen/game.cpp" &
  TrackPid "" $!

  # RunPoofHelper examples/the_wanderer/game.cpp && echo -e "$Success poofed examples/the_wanderer/game.cpp" &
  # TrackPid "" $!

  # RunPoofHelper examples/tools/voxel_synthesis_rule_baker/game.cpp && echo -e "$Success poofed examples/tools/voxel_synthesis_rule_baker/game.cpp" &
  # TrackPid "" $!

  # RunPoofHelper src/tools/asset_packer.cpp && echo -e "$Success poofed src/tools/asset_packer.cpp" &
  # TrackPid "" $!

  WaitForTrackedPids
  sync

  [ -d tmp ] && rmdir tmp
}


  # $TESTS/chunk.cpp
  # $TESTS/ui_command_buffer.cpp
  # $TESTS/m4.cpp
  # $TESTS/colladaloader.cpp
  # $TESTS/test_bitmap.cpp
  # $TESTS/bonsai_string.cpp
  # $TESTS/objloader.cpp
  # $TESTS/callgraph.cpp
  # $TESTS/heap_allocation.cpp
  # $TESTS/rng.cpp
  # $TESTS/file.cpp
  # $TESTS/sort.cpp
  # $TESTS/containers/block_array.cpp
TESTS_TO_BUILD="
  $TESTS/perlin_perf.cpp
"

BuildAll() {

  BuildExamples=1
  BuildExecutables=1
  BuildTests=1

  # NOTE(Jesse): These only build on linux.  I'm honestly not sure if it's
  # worth getting them to build on Windows
  if [ $Platform == "Linux" ]; then
    BuildDebugOnlyTests=1
  fi

  for ex in $BUNDLED_EXAMPLES; do
    EXAMPLES_TO_BUILD="$EXAMPLES_TO_BUILD $ex"
  done
}

if [ $# -eq 0 ]; then
  OPTIMIZATION_LEVEL="-O2"
  BuildAll
fi

BundleRelease=0
while (( "$#" )); do
  CliArg=$1
  echo $CliArg

  case $CliArg in

    "BuildAll")
      BuildAll
    ;;

    "BuildExecutables")
      BuildExecutables=1
    ;;

    "BuildDebugSystem")
      echo "BuildDebugSystem has been deprecated and will be removed in a future version."
    ;;

    "BuildBundledExamples")
      BuildExamples=1
      for ex in $BUNDLED_EXAMPLES; do
        EXAMPLES_TO_BUILD="$EXAMPLES_TO_BUILD $ex"
      done
    ;;

    "BuildTests")
      BuildTests=1
    ;;

    "BuildDebugOnlyTests")
      BuildDebugOnlyTests=1
    ;;

    "RunTests")
      RunTests=1
    ;;

    "RunPoof")
      RunPoof=1
    ;;

    "BuildWithEMCC")
      BuildWithEMCC=1
    ;;

    "BuildSingleExample")
      BuildExamples=1
      EXAMPLES_TO_BUILD="$EXAMPLES_TO_BUILD $2"
      shift
    ;;

    "BundleRelease")
      BundleRelease=1
      OPTIMIZATION_LEVEL="-O2"
      BONSAI_INTERNAL="-D BONSAI_INTERNAL=0"
      # BuildAll
    ;;

    "-Od")
      OPTIMIZATION_LEVEL="-Od"
    ;;

    "-O0")
      OPTIMIZATION_LEVEL="-O0"
    ;;

    "-O1")
      OPTIMIZATION_LEVEL="-O1"
    ;;

    "-O2")
      OPTIMIZATION_LEVEL="-O2"
    ;;

    "-O3")
      OPTIMIZATION_LEVEL="-O3"
    ;;


    *)
      echo "Unrecognized Build Option ($CliArg), exiting." && exit 1
    ;;
  esac

  shift
done

time RunEntireBuild

if [ $BundleRelease -eq 1 ]; then
  echo -e ""
  echo -e "$Delimeter"
  echo -e ""

  ColorizeTitle "Bundling"

  tar -cz                                                                  \
    bin/game_loader$PLATFORM_EXE_EXTENSION                                 \
    bin/game_libs/blank_project_loadable$PLATFORM_LIB_EXTENSION            \
    bin/game_libs/turn_based_loadable$PLATFORM_LIB_EXTENSION               \
    bin/game_libs/the_wanderer_loadable$PLATFORM_LIB_EXTENSION             \
    bin/game_libs/terrain_gen_loadable$PLATFORM_LIB_EXTENSION              \
    bin/game_libs/transparency_loadable$PLATFORM_LIB_EXTENSION             \
    bin/game_libs/project_and_level_picker_loadable$PLATFORM_LIB_EXTENSION \
    \
    shaders/*                                                              \
    external/bonsai_stdlib/shaders/*                                       \
    assets/*                                                               \
    models/*                                                               \
    brushes/*                                                              \
    \
    .root_marker                                                           \
    settings.init                                                          \
    texture_atlas_0.bmp > "$Platform""_x86_64_release.tar.gz"
fi
