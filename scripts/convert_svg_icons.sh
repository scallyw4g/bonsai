#! /usr/bin/env bash

PNG_IMAGES=$(find assets/icons/svg/ | grep -e svg$ )

rm -Rf assets/icons/bmp
mkdir assets/icons/bmp

for FILE in $PNG_IMAGES; do
  echo "Converting $FILE"

  Filename="$(basename -- $FILE .svg)"
  magick -density 300  -background None  $FILE -resize 64x64 ./assets/icons/bmp/$Filename.bmp
done

