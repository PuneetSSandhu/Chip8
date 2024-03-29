// Instructions for the Chip-8 CPU
#include "chip8_instructions.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// 0NNN - call routine at NNN
void chip8_0NNN(CHIP8 *chip8, WORD opcode)
{
    // not implemented
}

// 00E0 - clear the screen
void chip8_00E0(CHIP8 *chip8)
{
    memset(chip8->gfx, 0, 2048);
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

// 3XNN - skip next instruction if Vx == NN
void chip8_3XNN(CHIP8 *chip8, WORD opcode)
{
    BYTE X = (opcode & 0x0F00) >> 8;
    WORD NN = opcode & 0x00FF;
    if (chip8->V[X] == NN)
    {
        // Skip the next instruction (Add 2 since the execute will increment it by 2 more)
        chip8->pc += 2;
    }
}

// 4XNN - skip next instruction if Vx != NN
void chip8_4XNN(CHIP8 *chip8, WORD opcode)
{
    BYTE X = (opcode & 0x0F00) >> 8;
    WORD NN = opcode & 0x00FF;
    if (chip8->V[X] != NN)
    {
        // Skip the next instruction (Add 2 since the execute will increment it by 2 more)
        chip8->pc += 2;
    }
}

// 5XY0 - skip next instruction if Vx == Vy
void chip8_5XY0(CHIP8 *chip8, WORD opcode)
{
    BYTE X = (opcode & 0x0F00) >> 8;
    BYTE Y = (opcode & 0x00F0) >> 4;
    if (chip8->V[X] == chip8->V[Y])
    {
        // Skip the next instruction (Add 2 since the execute will increment it by 2 more)
        chip8->pc += 2;
    }
}

// 6XNN - set Vx = NN
void chip8_6XNN(CHIP8 *chip8, WORD opcode)
{
    BYTE X = (opcode & 0x0F00) >> 8;
    WORD NN = opcode & 0x00FF;
    chip8->V[X] = NN;
}

// 7XNN - set Vx = Vx + NN
void chip8_7XNN(CHIP8 *chip8, WORD opcode)
{
    BYTE X = (opcode & 0x0F00) >> 8;
    BYTE NN = opcode & 0x00FF;
    chip8->V[X] += NN;
}

// 8XY0 - set Vx to Vy
void chip8_8XY0(CHIP8 *chip8, WORD opcode)
{
    BYTE X = (opcode & 0x0F00) >> 8;
    BYTE Y = (opcode & 0x00F0) >> 4;
    chip8->V[X] = chip8->V[Y];
}

// 8XY1 - set Vx to Vx | Vy
void chip8_8XY1(CHIP8 *chip8, WORD opcode)
{
    BYTE X = (opcode & 0x0F00) >> 8;
    BYTE Y = (opcode & 0x00F0) >> 4;
    chip8->V[X] |= chip8->V[Y];
    chip8->V[0xF] = 0;
}

// 8XY2 - set Vx to Vx & Vy
void chip8_8XY2(CHIP8 *chip8, WORD opcode)
{
    BYTE X = (opcode & 0x0F00) >> 8;
    BYTE Y = (opcode & 0x00F0) >> 4;
    chip8->V[X] &= chip8->V[Y];
    chip8->V[0xF] = 0;
}

// 8XY3 - set Vx to Vx ^ Vy
void chip8_8XY3(CHIP8 *chip8, WORD opcode)
{
    BYTE X = (opcode & 0x0F00) >> 8;
    BYTE Y = (opcode & 0x00F0) >> 4;
    chip8->V[X] ^= chip8->V[Y];
    chip8->V[0xF] = 0;
}

// 8XY4 - add Vy to Vx, set VF
void chip8_8XY4(CHIP8 *chip8, WORD opcode)
{
    BYTE X = (opcode & 0x0F00) >> 8;
    BYTE Y = (opcode & 0x00F0) >> 4;

    chip8->V[X] += chip8->V[Y];
    if (chip8->V[X] < chip8->V[Y])
    {
        chip8->V[0xF] = 1;
    }
    else
    {
        chip8->V[0xF] = 0;
    }
}

// 8XY5 - subtract Vy from Vx, set VF
void chip8_8XY5(CHIP8 *chip8, WORD opcode)
{
    BYTE X = (opcode & 0x0F00) >> 8;
    BYTE Y = (opcode & 0x00F0) >> 4;

    chip8->V[X] -= chip8->V[Y];

    if (chip8->V[X] < chip8->V[Y])
    {
        chip8->V[0xF] = 1;
    }
    else
    {
        chip8->V[0xF] = 0;
    }
}

// 8XY6 - store the least significant bit of Vx on Vf and shift Vx left by one
void chip8_8XY6(CHIP8 *chip8, WORD opcode)
{
    BYTE X = (opcode & 0x0F00) >> 8;
    BYTE Y = (opcode & 0x00F0) >> 4;
    BYTE digit = chip8->V[X] & 0x1;

    chip8->V[X] >>= 1;
    chip8->V[0xF] = digit;
}

// 8XY7 - Set Vx to Vy - Vx and Vf to 0 is there is a borrow else 1
void chip8_8XY7(CHIP8 *chip8, WORD opcode)
{
    BYTE X = (opcode & 0x0F00) >> 8;
    BYTE Y = (opcode & 0x00F0) >> 4;
    chip8->V[X] = chip8->V[Y] - chip8->V[X];

    if (chip8->V[X] > chip8->V[Y])
    {
        chip8->V[0xF] = 0;
    }
    else
    {
        chip8->V[0xF] = 1;
    }
}

// 8XYE - store the most significant bit of Vx on Vf and shift Vx right by one
void chip8_8XYE(CHIP8 *chip8, WORD opcode)
{
    BYTE X = (opcode & 0x0F00) >> 8;
    BYTE Y = (opcode & 0x00F0) >> 4;
    BYTE digit = (chip8->V[X] & 0x80) >> 7;

    chip8->V[X] <<= 1;
    chip8->V[0xF] = digit;
}

// 9XY0 - skip next instruction if Vx != Vy
void chip8_9XY0(CHIP8 *chip8, WORD opcode)
{
    BYTE X = (opcode & 0x0F00) >> 8;
    BYTE Y = (opcode & 0x00F0) >> 4;
    chip8->pc += (chip8->V[X] != chip8->V[Y]) * 2; // no need to branch
}

// ANNN - set I to NNN
void chip8_ANNN(CHIP8 *chip8, WORD opcode)
{
    chip8->I = opcode & 0x0FFF;
}

// BNNN - jump to NNN + V0
void chip8_BNNN(CHIP8 *chip8, WORD opcode)
{
    chip8->pc = (opcode & 0x0FFF) + chip8->V[0];
}

// CXNN - set Vx to a random number & NN
void chip8_CXNN(CHIP8 *chip8, WORD opcode)
{
    WORD NN = opcode & 0x00FF;
    BYTE X = (opcode & 0x0F00) >> 8;
    chip8->V[X] = (rand() & NN) & 0xFF;
}

// DXYN - draw sprite at (Vx, Vy) with width 8 and height N, sprite located at I
// TODO: Verify
void chip8_DXYN(CHIP8 *chip8, WORD opcode)
{
    BYTE X = (opcode & 0x0F00) >> 8;
    BYTE Y = (opcode & 0x00F0) >> 4;
    BYTE Vx = chip8->V[X] % WIDTH;
    BYTE Vy = chip8->V[Y] % HEIGHT;
    BYTE N = opcode & 0x000F;
    chip8->V[0xF] = 0;
    BYTE newVal, spriteLine;
    int pixelIndex;
    for (int i = 0; i < N; i++)
    {
        spriteLine = chip8->memory[chip8->I + i];
        // printf("0x%02x, ", spriteLine);
        for (int j = 0; j < 8; j++)
        {
            if (Vx + j > WIDTH - 1 || Vy + i > HEIGHT - 1 || Vx + j < 0 || Vy + i < 0)
            {
                continue;
            }

            pixelIndex = (Vx + j) + ((Vy + i) * WIDTH);
            newVal =((spriteLine & (0x80 >> j)) >> (7 - j));
            if(newVal != 0){
                chip8->gfx[pixelIndex] ^= newVal;
                if(chip8->gfx[pixelIndex] == 0){
                    chip8->V[0xF] = 1;
                }
            }
        }
    }
    chip8->draw_flag = 1;
    // GFX_DEBUG(chip8);
    // diagnose(chip8, opcode);
}

// EX9E - skip next instruction if key with the value of Vx is pressed
void chip8_EX9E(CHIP8 *chip8, WORD opcode)
{
    BYTE X = (opcode & 0x0F00) >> 8;
    chip8->pc += (chip8->key[chip8->V[X]] == 1) * 2;
}

// EXA1 - skip next instruction if key with the value of Vx is not pressed
void chip8_EXA1(CHIP8 *chip8, WORD opcode)
{
    BYTE X = (opcode & 0x0F00) >> 8;
    chip8->pc += (chip8->key[chip8->V[X]] != 1) * 2;
}

// FX07 - set Vx to the value of the delay timer
void chip8_FX07(CHIP8 *chip8, WORD opcode)
{
    BYTE X = (opcode & 0x0F00) >> 8;
    chip8->V[X] = chip8->delay_timer;
}

// FX0A - wait for a key press, store the value of the key in Vx
void chip8_FX0A(CHIP8 *chip8, WORD opcode)
{
    BYTE X = (opcode & 0x0F00) >> 8;
    for (int i = 0; i < 16; i++)
    {
        if (chip8->key[i] == 1)
        {
            chip8->V[X] = i;
            return;
        }
    }
    chip8->pc -= 2; // wait for key press
}
// FX15 - set the delay timer to Vx
void chip8_FX15(CHIP8 *chip8, WORD opcode)
{
    BYTE X = (opcode & 0x0F00) >> 8;
    chip8->delay_timer = chip8->V[X];
}

// FX18 - set the sound timer to Vx
void chip8_FX18(CHIP8 *chip8, WORD opcode)
{
    BYTE X = (opcode & 0x0F00) >> 8;
    chip8->sound_timer = chip8->V[X];
}

// FX1E - add Vx to I
void chip8_FX1E(CHIP8 *chip8, WORD opcode)
{
    BYTE X = (opcode & 0x0F00) >> 8;
    chip8->I += chip8->V[X];
}

// FX29 - set I to the location of the sprite for the character in Vx
void chip8_FX29(CHIP8 *chip8, WORD opcode)
{
    BYTE X = (opcode & 0x0F00) >> 8;
    chip8->I = chip8->V[X] * 5;

}

// FX33 - store the binary-coded decimal representation of Vx at I, I+1, I+2
void chip8_FX33(CHIP8 *chip8, WORD opcode)
{
    BYTE X = (opcode & 0x0F00) >> 8;
    BYTE Vx = chip8->V[X];
    chip8->memory[chip8->I] = Vx / 100;
    chip8->memory[chip8->I + 1] = (Vx / 10) % 10;
    chip8->memory[chip8->I + 2] = (Vx % 100) % 10;
}

// FX55 - store V0 to Vx in memory starting at address I
void chip8_FX55(CHIP8 *chip8, WORD opcode)
{
    BYTE X = (opcode & 0x0F00) >> 8;
    for (int i = 0; i < X + 1; i++)
    {
        chip8->memory[chip8->I + i] = chip8->V[i];
    }
    chip8->I += 1;
}

// FX65 - fill V0 to Vx with values from memory starting at address I
void chip8_FX65(CHIP8 *chip8, WORD opcode)
{
    BYTE X = (opcode & 0x0F00) >> 8;
    for (int i = 0; i < X + 1; i++)
    {
        chip8->V[i] = chip8->memory[chip8->I + i];
    }
    chip8->I += 1;
}
