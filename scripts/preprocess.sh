#! /bin/bash

SOURCE_FILES=$(find src -type f -not -wholename "src/metaprogramming/defines.h")

GREEN="\x1b[32m"
WHITE="\x1b[37m"
RED="\x1b[31m"

RETURN=0

for file in $SOURCE_FILES; do
  output=$(bin/preprocessor "$file")
  if [ "$?" -eq "1" ]; then
    if [ ! -z "$output" ]; then
      echo -e "$GREEN"" ✔""$WHITE"" $file"
      echo "$output" > "src/metaprogramming/output/$(basename $file)"
    fi

  else
    echo -e "$RED"" ✗""$WHITE"" $file"
    RETURN=1
  fi

done

exit $RETURN

