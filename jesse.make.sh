#!/usr/bin/env bash

# OPT="-O2"

# ./make.sh RunPoof
# [ $? -ne 0 ] && exit 1

./make.sh $OPT BuildAll

./make.sh RunTests

# ./make.sh $OPT                                                 \
#   BuildSingleExample examples/blank_project                    \
#   BuildExecutables                                             \
#   BuildSingleExample examples/tools/voxel_synthesis_rule_baker \
#   BuildSingleExample examples/turn_based                       \
#   BuildSingleExample examples/project_and_level_picker         \
#   BuildSingleExample examples/terrain_gen                      \
#   BuildSingleExample examples/transparency                     \
#   BuildSingleExample examples/the_wanderer                     \
#   BuildTests                                                   \

# ./make.sh BundleRelease

