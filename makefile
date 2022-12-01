chip8: chip8.o
	gcc -o chip8 chip8.o

chip8.o: chip8.c chip8.h chip8_instructions.h chip8_instructions.c
	gcc -g -c chip8.c -lSDL2main -lSDL2 

clean:
	rm -f chip8 *.o

