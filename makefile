chip8: chip8.o
	gcc -o chip8 chip8.o

chip8.o: chip8.c chip8.h
	gcc -c chip8.c

clean:
	rm -f chip8 *.o

