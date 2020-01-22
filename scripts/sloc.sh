#! /bin/bash

find src -type f -print | xargs cat | wc -l
