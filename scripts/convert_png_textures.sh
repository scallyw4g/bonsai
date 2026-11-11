#! /usr/bin/env bash

IMAGES=$(find assets/terrain_textures/**/*.png | grep -e png$ )

rm -Rf assets/terrain_textures/bmp
mkdir assets/terrain_textures/bmp

for FILE in $IMAGES; do
  echo "Converting $FILE"

  Filename="$(basename -- $FILE .png)"
  magick -density 300 -background None $FILE -resize 64x64 -bordercolor None ./assets/terrain_textures/bmp/$Filename.bmp
done

