#! /usr/bin/env bash

# IMAGES=$(find assets/terrain_textures -type f -print0)

rm -Rf assets/terrain_textures/bmp
mkdir assets/terrain_textures/bmp

find assets/terrain_textures -type f -print0 | while IFS= read -r -d '' FILE; do
  echo "Converting $FILE"

  Filename="$(basename -- $FILE .png)"
  magick -density 300 -background None $FILE -resize 128x128 -bordercolor None "./assets/terrain_textures/bmp/$Filename.bmp"
done

