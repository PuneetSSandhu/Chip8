// This is a CHIP8 emulator written in C.

#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Stack functions
void stack_push(Stack *stack, WORD value)
{
    stack->stack[stack->sp] = value;
    stack->sp++;
}

WORD stack_pop(Stack *stack)
{
    stack->sp--;
    return stack->stack[stack->sp];
}

// Get the size of a file
long get_file_size(FILE *file)
{
    long size;
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);
    return size;
}

/////////////////////////////////////////

// Initialize CHIP8
void chip8_init(CHIP8 *chip8)
{

    // set default values for all registers
    chip8->pc = 0x200; // the program counter starts at 0x200
    chip8->I = 0;
    chip8->stack.sp = 0;

    // clear the display
    memset(chip8->gfx, 0, sizeof(chip8->gfx));

    // clear the stack
    memset(chip8->stack.stack, 0, sizeof(chip8->stack));

    // clear the registers
    memset(chip8->V, 0, sizeof(chip8->V));

    // clear the memory
    memset(chip8->memory, 0, sizeof(chip8->memory));

    // clear the keys
    memset(chip8->key, 0, sizeof(chip8->key));

    // Print initialization message
    printf("CHIP8 initialized\n");
}

// Load the ROM into memory
int chip8_load(CHIP8 *chip8, char *filename)
{
    // open the file
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        return 1;
    }

    // read the file into memory
    size_t f = fread(chip8->memory + 0x200, 1, get_file_size(file), file);
    if (f == 0)
    {
        return 1;
    }
    return 0;
}

// Emulate a single cycle
void chip8_emulate_cycle(CHIP8 *chip8)
{
    // Fetch opcode (OPCODES are 2 bytes so we need to shift the first byte by 8 and then or the second byte with the shifted first byte)
    WORD opcode = chip8->memory[chip8->pc] << 8 | chip8->memory[chip8->pc + 1];
    chip8->pc += 2;
    if (chip8->pc >= 0xFFF || opcode == 0x0000)
    {
        exit(1);
    }
    printf("opcode: %04x\n", opcode);

    // Decode opcode

    // Execute opcode

    // Update timers
}

// Set the keys
void chip8_set_keys(CHIP8 *chip8)
{
    // TODO
}

// Main game loop
int main(int argc, char *argv[])
{
    CHIP8 chip8;
    chip8_init(&chip8);
    int load = chip8_load(&chip8, argv[1]);
    if (load == 1)
    {
        fprintf(stderr, "Error: Could not load file %s\n", argv[1]);
        return 1;
    }

    while (1)
    {
        chip8_emulate_cycle(&chip8);
        chip8_set_keys(&chip8);
    }
    return 0;
}
