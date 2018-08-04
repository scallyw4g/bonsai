#! /bin/bash

for file in $(find bin/test*); do
  echo "Running $file"
  $file
done
