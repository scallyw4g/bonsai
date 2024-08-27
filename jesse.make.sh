#!/usr/bin/env bash

# OPT="-O2"

./make.sh RunPoof
[ $? -ne 0 ] && exit 1

# ./make.sh $OPT BuildAll

./make.sh $OPT                                                 \
  BuildExecutables                                             \
  BuildSingleExample examples/blank_project                    \
  BuildSingleExample examples/tools/voxel_synthesis_rule_baker \
#   BuildSingleExample examples/turn_based                       \
#   BuildSingleExample examples/project_and_level_picker         \
#   BuildSingleExample examples/terrain_gen                      \
#   BuildSingleExample examples/transparency                     \
#   BuildSingleExample examples/the_wanderer                     \
#   BuildTests                                                   \

# ./make.sh BundleRelease

# exit 0

# ./make.sh -O2 BuildSingleExample examples/blank_project
# ./make.sh -O2 BuildSingleExample examples/the_wanderer
# ./make.sh     BuildSingleExample examples/the_wanderer

# ./make.sh BuildTests



# ./make.sh MakeDebugLibRelease

# ./make.sh BuildTests
# ./make.sh BuildExecutables
# ./make.sh BuildBundledExamples

# ./make.sh BuildAll -O0
# ./make.sh BuildAll -O2

# ./make.sh BuildTests
