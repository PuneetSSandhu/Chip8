# Link SDL2
CC = gcc
CFLAGS = -I/opt/homebrew/include
LDFLAGS = -lSDL2 

chip8: chip8.o
	$(CC) $(CFLAGS) -o chip8 chip8.o $(LDFLAGS)

chip8.o: chip8.c chip8.h chip8_instructions.h chip8_instructions.c
	$(CC) $(CFLAGS) -c chip8.c


TEST: test.c chip8.h chip8_instructions.h chip8_instructions.c
	$(CC) $(CFLAGS) -c test.c -o test
clean:
	rm -f *.o chip8
