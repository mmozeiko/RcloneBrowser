#!/usr/bin/env python3

# packs multiple images (bmp/png/...) into ico file
# width and height of images must be <= 256
# pixel format of images must be 32-bit RGBA

import argparse
import struct
import os
from PIL import Image # https://python-pillow.org/

def pack(output, inp):
  count = len(inp)

  with open(output, "wb") as f:
    f.write(struct.pack("HHH", 0, 1, count))
    offset = struct.calcsize("HHH") + struct.calcsize("BBBBHHII")*count

    for i in inp:
      size = os.stat(i).st_size
      img = Image.open(i)
      w = 0 if img.width == 256 else img.width
      h = 0 if img.height == 256 else img.height
      f.write(struct.pack("BBBBHHII", w, h, 0, 0, 1, 32, size, offset))
      offset += size

    for i in inp:
      f.write(open(i, "rb").read())

if __name__ == "__main__":
  ap = argparse.ArgumentParser(description="pack multiple images into ico file")
  ap.add_argument("-o", "--out", help="output file")
  ap.add_argument("input", type=str, nargs='+', help="input images")
  args = ap.parse_args()
  pack(args.out, args.input)
