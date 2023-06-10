#!/usr/bin/env bash

# ./make.sh -O2 BuildSingleExample examples/blank_project
# ./make.sh -O2 BuildSingleExample examples/the_wanderer

# ./make.sh BuildTests

./make.sh              \
  -O2 \
  BuildExecutables     \
  BuildBundledExamples \
  BuildDebugSystem

