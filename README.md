# Chip8 Emulator in C
I have taken many classes on low level hardware functions and felt it was time to implement something that would give me a practical understanding of the subject. I originally wanted to make a Gameboy emulator but this is where I am starting, as recommended by this emulator [guide](http://www.codeslinger.co.uk/pages/projects/chip8/mychip8.html).

## CPU Emulation
The emulation of the cpu involves recreating the structure of the specific cpu. My "cpu" is in the chip8.h with all registers and ram bundled into a single struct. There are also instruction which need to preform similar operations to the original cpu. These instruction implementations are located in the chip8_instructions.c file in the format chip8_instruction(...). The main loop of the emulator is located in the chip8.c file and is responsible for fetching, decoding, and executing instructions. All in all this is a very simple cpu emulation since the cpu is very simple.

## Graphics and Audio
This chip8 emulator is written is C and uses SDL2 for graphics and sound. The renderer is setup as a 64x32 pixel grid. The audio is a beep.wav file I found online and is played using the SDL2 mixer library. The graphics and audio are handled in the chip8.c file.
 
## Keyboard
The keyboard is handled in the chip8.c file. The keyboard is mapped to the following keys:
```
1 2 3 4     1 2 3 F
q w e r     4 5 6 E
a s d f     7 8 9 D
z x c v     A 0 B C
```

## Usage
To compile the emulator you will need SDL2 and SDL2_mixer installed. Then run the included makefile. The emulator can be run with the following command:
```
./chip8 <rom>
```
Where rom is the path to the rom you want to run. The roms I have tested with are located in the roms folder.


## Challenges
While being simple it also gave me quiet a bit of trouble to debug. Thankfully [this](https://github.com/Timendus/chip8-test-suite) repo has an all in one test for checking functionality. Big thanks to Timendus for compiling this test suite. There were quiet a few bugs that required fixing. 

I also faced a weird problem with collision detection. I was able to solve it by flipping the flag setting in instruction 8XY4. I'm not sure why this fixed it but it did :grin:. I'm sure this a bug in my code but I'm not sure where.

## Resources
* [Guide](http://www.codeslinger.co.uk/pages/projects/chip8/mychip8.html)
* [Test Suite](https://github.com/Timendus/chip8-test-suite)
