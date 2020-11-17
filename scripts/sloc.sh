#! /bin/bash

find src -name \*.cpp -print -o -name \*.h -print | xargs sed -i 's/\bfunction\b/bonsai_function/g'

# echo "Meta output : $(find src/metaprogramming/output -name \*.cpp -print -o -name \*.h -print | xargs cat | wc -l)"
# echo "Total SLOC  : $(find src -name \*.cpp -print -o -name \*.h -print | xargs cat | wc -l)"
