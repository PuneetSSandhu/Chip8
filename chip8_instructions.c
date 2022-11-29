// Instructions for the Chip-8 CPU
#include "chip8.h"

// 0NNN - call routine at NNN
void chip8_0NNN(CHIP8 *chip8, WORD opcode)
{
    // not implemented
}

// 00E0 - clear the screen
void chip8_00E0(CHIP8 *chip8)
{
    memset(chip8->gfx, 0, sizeof(chip8->gfx));
    chip8->draw_flag = 1;
}

// 00EE - return from subroutine
void chip8_00EE(CHIP8 *chip8)
{
    // The address of the caller of the subroutine is stored in the stack
    chip8->pc = stack_pop(&chip8->stack);
}

// 1NNN - jump to address NNN
void chip8_1NNN(CHIP8 *chip8, WORD opcode)
{
    chip8->pc = opcode & 0x0FFF;
}

// 2NNN - call subroutine at NNN
void chip8_2NNN(CHIP8 *chip8, WORD opcode)
{
    // The address of the next instruction is stored in the stack
    stack_push(&chip8->stack, chip8->pc);
    chip8->pc = opcode & 0x0FFF;
}

// 3XNN - skip next instruction if VX == NN
void chip8_3XNN(CHIP8 *chip8, WORD opcode)
{
    BYTE X = opcode & 0x0F00 >> 8;
    WORD NN = opcode & 0x00FF;
    if (X == NN)
    {
        // Skip the next instruction (Add 2 since the execute will increment it by 2 more)
        chip8->pc += 2;
    }
}

// 4XNN - skip next instruction if XV != NN
void chip8_4XNN(CHIP8 *chip8, WORD opcode)
{
    BYTE X = opcode & 0x0F00 >> 8;
    WORD NN = opcode & 0x00FF;
    if (X != NN)
    {
        // Skip the next instruction (Add 2 since the execute will increment it by 2 more)
        chip8->pc += 2;
    }
}

// 5XY0 - skip next instruction if Vx == Vy
void chip8_5XY0(CHIP8 *chip8, WORD opcode)
{
    BYTE X = opcode & 0x0F00 >> 8;
    BYTE Y = opcode & 0x00F0 >> 4;
    if (chip8->V[(int)X] == chip8->V[(int)Y])
    {
        // Skip the next instruction (Add 2 since the execute will increment it by 2 more)
        chip8->pc += 2;
    }
}

// 6XNN - set Vx = NN
void chip8_6XNN(CHIP8 *chip8, WORD opcode)
{
    BYTE X = opcode & 0x0F00 >> 8;
    WORD NN = opcode & 0x00FF;
    chip8->V[(int)X] = NN;
}

// 7XNN - set Vx = Vx + NN
void chip8_7XNN(CHIP8 *chip8, WORD opcode)
{
    BYTE X = opcode & 0x0F00 >> 8;
    WORD NN = opcode & 0x00FF;
    chip8->V[(int)X] += NN;
}

// 8XY0 - set Vx to Vy
void chip8_8XY0(CHIP8 *chip8, WORD opcode){
    BYTE X = opcode & 0x0F00 >> 8;
    BYTE Y = opcode & 0x00F0 >> 4;
    chip8->V[(int)X] = chip8->V[(int)Y];
}

// 8XY1 - set Vx to Vx | Vy
void chip8_8XY1(CHIP8 *chip8, WORD opcode){
    BYTE X = opcode & 0x0F00 >> 8;
    BYTE Y = opcode & 0x00F0 >> 4;
    chip8->V[(int)X] |= chip8->V[(int)Y];
}

// 8XY2 - set Vx to Vx & Vy
void chip8_8XY2(CHIP8 *chip8, WORD opcode){
    BYTE X = opcode & 0x0F00 >> 8;
    BYTE Y = opcode & 0x00F0 >> 4;
    chip8->V[(int)X] &= chip8->V[(int)Y];
}

// 8XY3 - set Vx to Vx ^ Vy
void chip8_8XY3(CHIP8 *chip8, WORD opcode){
    BYTE X = opcode & 0x0F00 >> 8;
    BYTE Y = opcode & 0x00F0 >> 4;
    chip8->V[(int)X] ^= chip8->V[(int)Y];
}

// 8XY4 - add Vy to Vx, set VF 
void chip8_8XY1(CHIP8 *chip8, WORD opcode){
    BYTE X = opcode & 0x0F00 >> 8;
    BYTE Y = opcode & 0x00F0 >> 4;

    chip8->V[(int)X] += chip8->V[(int)Y];
    int sum = X + Y;

    if(sum > sum & 0xFFFF){ // there is a carry
        chip8->V[0xF] = 1;
    } else {
        chip8->V[0xF] = 0;
    }
}

// 8XY5 - subtract Vy from Vx, set VF
void chip8_8XY5(CHIP8 *chip8, WORD opcode){
    BYTE X = opcode & 0x0F00 >> 8;
    BYTE Y = opcode & 0x00F0 >> 4;

    if(chip8->V[(int)X] > chip8->V[(int)Y]){
        chip8->V[0xF] = 1;
    } else {
        chip8->V[0xF] = 0;
    }

    chip8->V[(int)X] -= chip8->V[(int)Y];
}

// 8XY6 - store the least significant bit of Vx on Vf and shift Vx left by one
void chip8_8XY6(CHIP8 * chip8, WORD opcode){
    BYTE X = opcode & 0x0F00 >> 8;
    // Y not needed
    chip8->V[0xF] = chip8->V[X] & 0x01;
    chip8->V[X] = chip8->V[X] >> 1;
}

// 8XY7 - Set Vx to Vy - Vx and Vf to 0 is there is a borrow else 1
void chip_8XY7(CHIP8 *chip8, WORD opcode){
    BYTE X = opcode & 0x0F00 >> 8;
    BYTE Y = opcode & 0x00F0 >> 4;

    chip8->V[X] = chip8->V[Y] - chip8->V[X];
    if(chip8->V[X] > chip8->V[Y]){
        chip8->V[0xF] = 0;
    } else {
        chip8->V[0xF] = 1;
    }
}

// 8XYE - store the most significant bit of Vx on Vf and shift Vx right by one
void chip8_8XYE(CHIP8 *chip8, WORD opcode){
    BYTE X = opcode & 0x0F00 >> 8;
    // Y not needed
    chip8->V[0xF] = chip8->V[X] & 0x10;
    chip8->V[X] = chip8->V[X] << 1;
}

// 9XY0 - skip next instruction if Vx != Vy
// ANNN - set I to NNN
// BNNN - jump to NNN + V0
// CXNN - set Vx to a random number & NN
// DXYN - draw sprite at (Vx, Vy) with width 8 and height N
// EX9E - skip next instruction if key with the value of Vx is pressed
// EXA1 - skip next instruction if key with the value of Vx is not pressed
// FX07 - set Vx to the value of the delay timer
// FX0A - wait for a key press, store the value of the key in Vx
// FX15 - set the delay timer to Vx
// FX18 - set the sound timer to Vx
// FX1E - add Vx to I
// FX29 - set I to the location of the sprite for the character in Vx
// FX33 - store the binary-coded decimal representation of Vx at I, I+1, I+2
// FX55 - store V0 to Vx in memory starting at address I
// FX65 - fill V0 to Vx with values from memory starting at address I