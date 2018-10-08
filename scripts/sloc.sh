#! /bin/bash

find src -path src/GL -prune -o -type f -print | xargs cat | wc -l
