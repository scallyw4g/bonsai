#! /bin/bash

FILES=$(cpp_and_headers.sh)

for file in $FILES; do
  TODOS=$(cat "$file" | grep -n "TODO") # | sed -e 's/^[[:space:]]*\/\/[[:space:]]*TODO(//g' -e 's/)//' -e 's/[[:space:]]*$//g')

  if [ ! -z "$TODOS" ]; then
    while read -r todo; do
        echo "$file:$todo"
    done <<< "$TODOS"
  fi

done
