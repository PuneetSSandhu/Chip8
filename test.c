#include "chip8_instructions.c"
#include <assert.h>
#include <stdio.h>
// Unit Testing

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

void runTests(CHIP8 *chip8)
{
    printf("Running tests...");
    // Test 1: 0x00E0
    chip8_00E0(chip8);
    assert(chip8->draw_flag == 1);
    assert(chip8->gfx[0] == 0);

    // Test 2: 0x00EE
    chip8->stack.stack[0] = 0x202;
    chip8->stack.sp = 1;
    chip8->pc = 0x200;
    chip8_00EE(chip8);
    assert(chip8->pc == 0x202);
    assert(chip8->stack.sp == 0);

    // Test 3: 0x1NNN
    chip8->pc = 0x200;
    chip8_1NNN(chip8, 0x1FFF);
    assert(chip8->pc == 0xFFF);

    // Test 4: 0x2NNN
    chip8->pc = 0x200;
    chip8_2NNN(chip8, 0x2FFF);
    assert(chip8->pc == 0xFFF);
    assert(chip8->stack.stack[0] == 0x200);
    assert(chip8->stack.sp == 1);

    // Test 5: 0x3XNN
    chip8->pc = 0x200;
    chip8->V[0] = 0x10;
    chip8_3XNN(chip8, 0x3010);
    assert(chip8->pc == 0x202);

    // Test 5.1: 0x3XNN
    chip8->pc = 0x200;
    chip8->V[0] = 0x10;
    chip8_3XNN(chip8, 0x3011);
    assert(chip8->pc == 0x200);

    // Test 6: 0x4XNN
    chip8->pc = 0x200;
    chip8->V[0] = 0x00;
    chip8_4XNN(chip8, 0x4010);
    assert(chip8->pc == 0x202);

    // Test 6.1: 0x4XNN
    chip8->pc = 0x200;
    chip8->V[0] = 0x10;
    chip8_4XNN(chip8, 0x4010);
    assert(chip8->pc == 0x200);

    // Test 7: 0x5XY0
    chip8->pc = 0x200;
    chip8->V[0] = 0x10;
    chip8->V[1] = 0x10;
    chip8_5XY0(chip8, 0x5010);
    assert(chip8->pc == 0x202);

    // Test 7.1: 0x5XY0
    chip8->pc = 0x200;
    chip8->V[0] = 0x10;
    chip8->V[1] = 0x11;
    chip8_5XY0(chip8, 0x5010);
    assert(chip8->pc == 0x200);

    // Test 8: 0x6XNN
    chip8->pc = 0x200;
    chip8_6XNN(chip8, 0x6010);
    assert(chip8->V[0] == 0x10);

    // Test 9: 0x7XNN
    chip8->pc = 0x200;
    chip8->V[0] = 0x00;
    chip8_7XNN(chip8, 0x7010);
    assert(chip8->V[0] == 0x10);

    // Test 10: 0x8XY0
    chip8->pc = 0x200;
    chip8->V[0] = 0x10;
    chip8->V[1] = 0x11;
    chip8_8XY0(chip8, 0x8010);
    assert(chip8->V[0] == 0x11);

    // Test 11: 0x8XY1
    chip8->pc = 0x200;
    chip8->V[0] = 0x10;
    chip8->V[1] = 0x11;
    chip8_8XY1(chip8, 0x8011);
    assert(chip8->V[0] == 0x11);

    // Test 12: 0x8XY2
    chip8->pc = 0x200;
    chip8->V[0] = 0x10;
    chip8->V[1] = 0x11;
    chip8_8XY2(chip8, 0x8012);
    assert(chip8->V[0] == 0x10);

    // Test 13: 0x8XY3
    chip8->pc = 0x200;
    chip8->V[0] = 0x10;
    chip8->V[1] = 0x11;
    chip8_8XY3(chip8, 0x8013);
    assert(chip8->V[0] == 0x01);

    // Test 14: 0x8XY4
    chip8->pc = 0x200;
    chip8->V[0] = 0x10;
    chip8->V[1] = 0x11;
    chip8_8XY4(chip8, 0x8014);
    assert(chip8->V[0] == 0x21);
    assert(chip8->V[0xF] == 0x00);

    // Test 14.1: 0x8XY4
    chip8->pc = 0x200;
    chip8->V[0] = 0xFF;        // X
    chip8->V[1] = 0x01;        // Y
    chip8_8XY4(chip8, 0x8014); // X = X + Y (0xFF + 0x01) = 0x00
    assert(chip8->V[0] == 0x00);
    assert(chip8->V[0xF] == 0x01);

    // Test 15: 0x8XY5
    chip8->pc = 0x200;
    chip8->V[0] = 0x10;        // X
    chip8->V[1] = 0x11;        // Y
    chip8_8XY5(chip8, 0x8015); // X = X - Y (0x10 - 0x11) = 0xFF
    assert(chip8->V[0] == 0xFF);
    assert(chip8->V[0xF] == 0x00);

    // Test 15.1: 0x8XY5
    chip8->pc = 0x200;
    chip8->V[0] = 0x01;
    chip8->V[1] = 0xFF;
    chip8_8XY5(chip8, 0x8015);
    assert(chip8->V[0] == 0x02);
    assert(chip8->V[0xF] == 0x01);

    // Test 16: 0x8XY6
    chip8->pc = 0x200;
    chip8->V[0] = 0x10;
    chip8_8XY6(chip8, 0x8016);
    assert(chip8->V[0] == 0x08);
    assert(chip8->V[0xF] == 0x00);

    // Test 16.1: 0x8XY6
    chip8->pc = 0x200;
    chip8->V[0] = 0x11;
    chip8_8XY6(chip8, 0x8016);
    assert(chip8->V[0] == 0x08);
    assert(chip8->V[0xF] == 0x01);

    // Test 17: 0x8XY7
    chip8->pc = 0x200;
    chip8->V[0] = 0x10;        // X
    chip8->V[1] = 0x11;        // Y
    chip8_8XY7(chip8, 0x8017); // X = Y - X (0x11 - 0x10) = 0x01
    assert(chip8->V[0] == 0x01);
    assert(chip8->V[0xF] == 0x01);

    // Test 17.1: 0x8XY7
    chip8->pc = 0x200;
    chip8->V[0] = 0x11;        // X
    chip8->V[1] = 0x10;        // Y
    chip8_8XY7(chip8, 0x8017); // X = Y - X (0x10 - 0x11) = 0xFF
    assert(chip8->V[0] == 0xFF);
    assert(chip8->V[0xF] == 0x00);

    // Test 18: 0x8XYE
    chip8->pc = 0x200;
    chip8->V[0] = 0x10;
    chip8_8XYE(chip8, 0x801E);
    assert(chip8->V[0] == 0x20);
    assert(chip8->V[0xF] == 0x00);

    // Test 18.1: 0x8XYE
    chip8->pc = 0x200;
    chip8->V[0] = 0x80;
    chip8_8XYE(chip8, 0x801E);
    assert(chip8->V[0] == 0x00);
    assert(chip8->V[0xF] == 0x01);

    // Test 19: 0x9XY0
    chip8->pc = 0x200;
    chip8->V[0] = 0x10;
    chip8->V[1] = 0x11;
    chip8_9XY0(chip8, 0x9010);
    assert(chip8->pc == 0x202);

    // Test 19.1: 0x9XY0
    chip8->pc = 0x200;
    chip8->V[0] = 0x10;
    chip8->V[1] = 0x10;
    chip8_9XY0(chip8, 0x9010);
    assert(chip8->pc == 0x200);

    // Test 20: 0xANNN
    chip8->pc = 0x200;
    chip8_ANNN(chip8, 0xA123);
    assert(chip8->I == 0x123);

    // Test 21: 0xBNNN
    chip8->pc = 0x200;
    chip8->V[0] = 0x10;
    chip8_BNNN(chip8, 0xB123);
    assert(chip8->pc == 0x133);

    // Test 22: 0xCXNN
    chip8->pc = 0x200;
    chip8_CXNN(chip8, 0xC123);
    assert((chip8->V[1] | 0x23) == 0x23);

    // Test 23: 0xDXYN
    chip8->pc = 0x200;
    chip8->V[0] = 0x00;
    chip8->V[1] = 0x00;
    chip8->I = 0x200;
    chip8->memory[0x200] = 0xFF;
    chip8->memory[0x201] = 0x00;
    chip8_DXYN(chip8, 0xD001);

    // Test 23.1: 0xDXYN
    chip8->pc = 0x200;
    chip8->V[0] = 0x00;
    chip8->V[1] = 0x00;
    chip8->I = 0x200;
    chip8->memory[0x200] = 0xFF;
    chip8->memory[0x201] = 0x00;
    chip8_DXYN(chip8, 0xD001);

    // Test 24: 0xEX9E
    chip8->pc = 0x200;
    chip8->V[0] = 0x01;
    chip8->key[0x01] = 1;
    chip8_EX9E(chip8, 0xE09E);
    assert(chip8->pc == 0x202);

    // Test 24.1: 0xEX9E
    chip8->pc = 0x200;
    chip8->V[0] = 0x01;
    chip8->key[0x01] = 0;
    chip8_EX9E(chip8, 0xE09E);
    assert(chip8->pc == 0x200);

    // Test 25: 0xEXA1
    chip8->pc = 0x200;
    chip8->V[0] = 0x01;
    chip8->key[0x01] = 0;
    chip8_EXA1(chip8, 0xE0A1);
    assert(chip8->pc == 0x202);

    // Test 25.1: 0xEXA1
    chip8->pc = 0x200;
    chip8->V[0] = 0x01;
    chip8->key[0x01] = 1;
    chip8_EXA1(chip8, 0xE0A1);
    assert(chip8->pc == 0x200);

    // Test 26: 0xFX07
    chip8->pc = 0x200;
    chip8->delay_timer = 0x10;
    chip8_FX07(chip8, 0xF007);
    assert(chip8->V[0] == 0x10);

    // Test 27: 0xFX0A
    chip8->pc = 0x200;
    chip8->key[0x01] = 1;
    chip8_FX0A(chip8, 0xF00A);
    assert(chip8->V[0] == 0x01);

    // Test 28: 0xFX15
    chip8->pc = 0x200;
    chip8->V[0] = 0x10;
    chip8_FX15(chip8, 0xF015);
    assert(chip8->delay_timer == 0x10);

    // Test 29: 0xFX18
    chip8->pc = 0x200;
    chip8->V[0] = 0x10;
    chip8_FX18(chip8, 0xF018);
    assert(chip8->sound_timer == 0x10);

    // Test 30: 0xFX1E
    chip8->pc = 0x200;
    chip8->V[0] = 0x10;
    chip8->I = 0x10;
    chip8_FX1E(chip8, 0xF01E);
    assert(chip8->I == 0x20);

    // Test 31: 0xFX29
    chip8->pc = 0x200;
    chip8->V[0] = 0x00;
    chip8_FX29(chip8, 0xF029);
    assert(chip8->I == 0x00);

    // Test 32: 0xFX33
    chip8->pc = 0x200;
    chip8->V[0] = 0x23;
    chip8_FX33(chip8, 0xF033);

    // Test 33: 0xFX55
    chip8->pc = 0x200;
    chip8->V[0] = 0x01;
    chip8->V[1] = 0x02;
    chip8->V[2] = 0x03;
    chip8->V[3] = 0x04;
    chip8->V[4] = 0x05;
    chip8->V[5] = 0x06;

    chip8_FX55(chip8, 0xF555);
    assert(chip8->memory[chip8->I] == 0x01);
    assert(chip8->memory[chip8->I + 1] == 0x02);
    assert(chip8->memory[chip8->I + 2] == 0x03);
    assert(chip8->memory[chip8->I + 3] == 0x04);
    assert(chip8->memory[chip8->I + 4] == 0x05);
    assert(chip8->memory[chip8->I + 5] == 0x06);

    // Test 34: 0xFX65
    chip8->pc = 0x200;
    chip8->memory[chip8->I] = 0x07;
    chip8->memory[chip8->I + 1] = 0x06;
    chip8->memory[chip8->I + 2] = 0x05;
    chip8->memory[chip8->I + 3] = 0x04;
    chip8->memory[chip8->I + 4] = 0x03;
    chip8->memory[chip8->I + 5] = 0x02;

    chip8_FX65(chip8, 0xF565);

    assert(chip8->V[0] == 0x07);
    assert(chip8->V[1] == 0x06);
    assert(chip8->V[2] == 0x05);
    assert(chip8->V[3] == 0x04);
    assert(chip8->V[4] == 0x03);
    assert(chip8->V[5] == 0x02);
}

int main()
{
    CHIP8 chip8;
    runTests(&chip8);
    printf("All tests passed\n");
    return 0;
}