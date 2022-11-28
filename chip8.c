// This is a CHIP8 emulator written in C.

#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


long get_file_size(FILE *file) {
    long size;
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);
    return size;
}

// Initialize CHIP8
void chip8_init(chip8* chip8){
    // set default values for all registers
    chip8->pc = 0x200; // the program counter starts at 0x200
    chip8->I = 0;
    chip8->sp = 0;

    // clear the display
    memset(chip8->gfx, 0, sizeof(chip8->gfx));

    // clear the stack
    memset(chip8->stack, 0, sizeof(chip8->stack));

    // clear the registers
    memset(chip8->V, 0, sizeof(chip8->V));

    // clear the memory
    memset(chip8->memory, 0, sizeof(chip8->memory));

    // clear the keys
    memset(chip8->key, 0, sizeof(chip8->key));
}

// Load the ROM into memory
int chip8_load(chip8* chip8, char* filename){
    // open the file
    FILE* file = fopen(filename, "rb");
    if(file == NULL){
        return 1;
    }
    
    // read the file into memory
    fread(chip8->memory + 0x200, 1, get_file_size(file), file);
}

// Emulate a single cycle
void chip8_emulate_cycle(chip8* chip8){
    // Fetch opdode
    WORD opcode = chip8->memory[chip8->pc];

    // Decode opcode

    // Execute opcode

    // Update timers

}


// Set the keys
void chip8_set_keys(chip8* chip8){
    // TODO
}

// Main game loop
int main (int argc, char *argv[]) {
    struct chip8 chip8;
    chip8_init(&chip8);
    int load = chip8_load(&chip8, argv[1]);
    if(load == 1){
        fprintf(stderr ,"Error: Could not load file %s\n", argv[1]);
        return 1;
    }
    // while (1) {
        chip8_emulate_cycle(&chip8);
        chip8_set_keys(&chip8);
    // }
    return 0;
}

