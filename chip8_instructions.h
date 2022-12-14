#include "chip8.h"


void chip8_0NNN(CHIP8 *chip8, WORD opcode);
void chip8_00E0(CHIP8 *chip8);
void chip8_00EE(CHIP8 *chip8);
void chip8_1NNN(CHIP8 *chip8, WORD opcode);
void chip8_2NNN(CHIP8 *chip8, WORD opcode);
void chip8_3XNN(CHIP8 *chip8, WORD opcode);
void chip8_4XNN(CHIP8 *chip8, WORD opcode);
void chip8_5XY0(CHIP8 *chip8, WORD opcode);
void chip8_6XNN(CHIP8 *chip8, WORD opcode);
void chip8_7XNN(CHIP8 *chip8, WORD opcode);
void chip8_8XY0(CHIP8 *chip8, WORD opcode);
void chip8_8XY1(CHIP8 *chip8, WORD opcode);
void chip8_8XY2(CHIP8 *chip8, WORD opcode);
void chip8_8XY3(CHIP8 *chip8, WORD opcode);
void chip8_8XY4(CHIP8 *chip8, WORD opcode);
void chip8_8XY5(CHIP8 *chip8, WORD opcode);
void chip8_8XY6(CHIP8 *chip8, WORD opcode);
void chip8_8XY7(CHIP8 *chip8, WORD opcode);
void chip8_8XYE(CHIP8 *chip8, WORD opcode);
void chip8_9XY0(CHIP8 *chip8, WORD opcode);
void chip8_ANNN(CHIP8 *chip8, WORD opcode);
void chip8_BNNN(CHIP8 *chip8, WORD opcode);
void chip8_CXNN(CHIP8 *chip8, WORD opcode);
void chip8_DXYN(CHIP8 *chip8, WORD opcode);
void chip8_EX9E(CHIP8 *chip8, WORD opcode);
void chip8_EXA1(CHIP8 *chip8, WORD opcode);
void chip8_FX07(CHIP8 *chip8, WORD opcode);
void chip8_FX0A(CHIP8 *chip8, WORD opcode);
void chip8_FX15(CHIP8 *chip8, WORD opcode);
void chip8_FX18(CHIP8 *chip8, WORD opcode);
void chip8_FX1E(CHIP8 *chip8, WORD opcode);
void chip8_FX29(CHIP8 *chip8, WORD opcode);
void chip8_FX33(CHIP8 *chip8, WORD opcode);
void chip8_FX55(CHIP8 *chip8, WORD opcode);
void chip8_FX65(CHIP8 *chip8, WORD opcode);
