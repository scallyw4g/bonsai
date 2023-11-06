#!/usr/bin/env bash

./make.sh RunPoof
# exit 1
[ $? -ne 0 ] && exit 1

# exit 0

# ./make.sh -O2 BuildSingleExample examples/blank_project
# ./make.sh -O2 BuildSingleExample examples/the_wanderer
# ./make.sh     BuildSingleExample examples/the_wanderer

# ./make.sh BuildTests



# ./make.sh MakeDebugLibRelease

# ./make.sh BuildTests
# ./make.sh BuildExecutables
# ./make.sh BuildDebugSystem
# ./make.sh BuildBundledExamples

# ./make.sh BuildAll -O0
# ./make.sh BuildAll -O2

# OPT="-O2"
# ./make.sh $OPT \
#     BuildSingleExample examples/terrain_gen \
#     BuildDebugSystem

# OPT="-O2"

./make.sh $OPT \
  BuildSingleExample examples/transparency \
  BuildSingleExample examples/turn_based \
  BuildSingleExample examples/the_wanderer \
  BuildExecutables \
  BuildDebugSystem \
  # BuildSingleExample examples/blank_project \
#   # BuildTests \
#   # BuildSingleExample examples/turn_based2

# ./make.sh $OPT BuildAll
