#! /usr/bin/env bash

PNG_IMAGES=$(find assets/icons/png/ | grep -e png$ )

for FILE in $PNG_IMAGES; do
  echo "Converting $FILE"

  Filename="$(basename -- $FILE .png)"
  magick $FILE assets/icons/bmp/$Filename.bmp
done

