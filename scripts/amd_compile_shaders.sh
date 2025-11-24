#! /usr/bin/env bash

GLC=~/bin/rdna_tools/utils/glc.exe 

# $GLC -gfxip 1100 -set in.vert.glsl="../../work/bonsai/shaders/DepthRTT.vertexshader"


SHADERS=$(find shaders/ | grep vert )
HEADER_CODE="$(cat ./external/bonsai_stdlib/shaders/header.glsl)"

for FILE in $SHADERS; do
  echo "Compiling $FILE"


  echo "$HEADER_CODE" > tmp.txt
  cat "$FILE" >> tmp.txt

  $GLC -gfxip 1100 -set in.vert.glsl="tmp.txt"
  echo "-----  "
done

SHADERS=$(find shaders/ | grep frag )

for FILE in $SHADERS; do
  echo "Compiling $FILE"


  echo "$HEADER_CODE" > tmp.txt
  cat "$FILE" >> tmp.txt

  $GLC -gfxip 1100 -set in.frag.glsl="tmp.txt"
  echo "-----  "
done

rm tmp.txt
