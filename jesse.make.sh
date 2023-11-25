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
#     BuildDebugSystem

# OPT="-O2"

./make.sh $OPT \
  BuildExecutables \
  BuildSingleExample examples/terrain_gen \
  BuildSingleExample examples/tools/voxel_synthesis_rule_baker \
  BuildSingleExample examples/turn_based \
  BuildSingleExample examples/transparency \
  BuildSingleExample examples/blank_project \
  BuildDebugSystem \
  # BuildSingleExample examples/the_wanderer \
#   # BuildTests \
#   # BuildSingleExample examples/turn_based2

# ./make.sh $OPT BuildAll
