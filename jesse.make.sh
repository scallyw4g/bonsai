#!/usr/bin/env bash

# ./make.sh -O2 BuildSingleExample examples/blank_project
# ./make.sh -O2 BuildSingleExample examples/the_wanderer
# ./make.sh BuildSingleExample examples/the_wanderer
#
# ./make.sh BuildSingleExample examples/perlin_terrain_gen

./make.sh BuildTests

# ./make.sh RunPoof

# ./make.sh


# ./make.sh MakeDebugLibRelease
# ./make.sh BuildExecutables

# ./make.sh BuildAll -O0
# ./make.sh BuildAll -O2

# OPT="-O2"
# ./make.sh $OPT \
#     BuildSingleExample examples/terrain_gen \
#     BuildDebugSystem

# ./make.sh BuildDebugSystem

# OPT="-O2"
# ./make.sh $OPT \
#     BuildSingleExample examples/turn_based2

