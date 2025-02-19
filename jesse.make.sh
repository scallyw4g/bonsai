#! /usr/bin/env bash

# OPT="-O2"

./make.sh RunPoof
[ $? -ne 0 ] && exit 1

# ./make.sh $OPT BuildAll

# ./make.sh RunTests

./make.sh $OPT                                                 \
  BuildSingleExample examples/terrain_gen                      \
  BuildExecutables                                             \
  # BuildTests                                                   \
  # BuildSingleExample examples/blank_project                    \
  # BuildSingleExample examples/project_and_level_picker         \
  # BuildSingleExample examples/transparency                     \
  # BuildSingleExample examples/tools/voxel_synthesis_rule_baker \
  # BuildSingleExample examples/turn_based                       \
  # BuildSingleExample examples/the_wanderer                     \

# ./make.sh BundleRelease

