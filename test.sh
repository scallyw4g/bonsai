#! /bin/bash

for test_executable in $(find bin/test*); do
  echo "Running $test_executable"
  $test_executable
done
