#! /usr/bin/env bash

# OPT="-O2"

./make.sh RunPoof
[ $? -ne 0 ] && exit 1

# ./make.sh $OPT BuildAll


# ./make.sh RunTests

# ./make.sh BuildWithEMCC

./make.sh $OPT                                                 \
  BuildSingleExample examples/asset_editor                     \
  BuildExecutables                                             \
  BuildSingleExample examples/terrain_gen                      \
  BuildSingleExample examples/project_and_level_picker         \
  # BuildSingleExample examples/blank_project                    \
  # BuildSingleExample examples/character_controller             \
  # BuildTests                                                   \
  # BuildSingleExample examples/ui_test                          \
  # BuildSingleExample examples/transparency                     \
  # BuildSingleExample examples/turn_based                       \
  # BuildSingleExample examples/the_wanderer

  # BuildSingleExample examples/tools/voxel_synthesis_rule_baker \

# ./make.sh BuildAll BundleRelease
# ./make.sh BundleRelease

