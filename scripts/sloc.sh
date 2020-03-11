#! /bin/bash

find src -name \*.cpp -print -o -name \*.h -print | xargs cat | wc -l
