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
   if [ $1 = "t" ]; then
      make
      ./chip8 roms/TEST
   fi
   # run ./chip8 with roms/$1
   if [ $1 != "d" ] && [ $1 != "t" ]; then
      make
      ./chip8 roms/$1
   fi
fi