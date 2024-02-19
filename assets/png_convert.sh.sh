

for filename in $(ls | grep png); do
  echo $filename
  magick $filename $(basename $filename .png).bmp
done
