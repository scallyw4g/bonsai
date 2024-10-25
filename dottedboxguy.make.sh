#! /usr/bin/env bash

OPT="-O2"

./make.sh $OPT                                                 \
  BuildSingleExample examples/terrain_gen                      \
  BuildExecutables                                             \
