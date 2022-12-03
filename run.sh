#!/bin/sh
# no parameter
if [ $# -eq 0 ]; then
    make 
    ./chip8 roms/INVADERS
fi

# debug parameter
if [ $# -eq 1 ]; then
   if [ $1 = "d" ]; then
      make
      gdb --args ./chip8 roms/INVADERS
   fi
fi