#!/bin/sh

set -eu

URL=https://raw.githubusercontent.com/ncw/rclone/master/graphics/rclone.png

[ -f "rclone.png" ] || curl -o rclone.png -L $URL

SIZES=(512 256 128 64 32 16)
for s in "${SIZES[@]}"
do
  convert rclone.png -resize $s rclone_$s.png
  optipng -o7 -strip all rclone_$s.png
done

if [ `uname` == "Linux" ]
then
  ./images2ico.py -o ../src/icon.ico rclone_256.png rclone_128.png rclone_64.png rclone_32.png rclone_16.png
else
  # brew install makeicns
  makeicns -out ../src/icon.icns -512 rclone_512.png -256 rclone_256.png -128 rclone_128.png -64 rclone_64.png -32 rclone_32.png -16 rclone_16.png
fi
