// This is a CHIP8 emulator written in C.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chip8_instructions.c"

// SDL2
#include <SDL2/SDL.h>

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

// DEBUGGING FUNCTIONS

void diagnose(CHIP8 *chip8, WORD opcode)
{
    printf("Opcode: 0x%04x\n", opcode);
    printf("  pc: 0x%x\n", chip8->pc);
    printf("  sp: 0x%x\n", chip8->stack.sp);
    printf("  I: 0x%04x\n", chip8->I);
    printf("  V0: 0x%x\n", chip8->V[0]);
    printf("  V1: 0x%x\n", chip8->V[1]);
    printf("  V2: 0x%x\n", chip8->V[2]);
    printf("  V3: 0x%x\n", chip8->V[3]);
    printf("  V4: 0x%x\n", chip8->V[4]);
    printf("  V5: 0x%x\n", chip8->V[5]);
    printf("  V6: 0x%x\n", chip8->V[6]);
    printf("  V7: 0x%x\n", chip8->V[7]);
    printf("  V8: 0x%x\n", chip8->V[8]);
    printf("  V9: 0x%x\n", chip8->V[9]);
    printf("  VA: 0x%x\n", chip8->V[10]);
    printf("  VB: 0x%x\n", chip8->V[11]);
    printf("  VC: 0x%x\n", chip8->V[12]);
    printf("  VD: 0x%x\n", chip8->V[13]);
    printf("  VE: 0x%x\n", chip8->V[14]);
    printf("  VF: 0x%x\n", chip8->V[15]);
    printf(" delay_timer: 0x%x\n", chip8->delay_timer);
    printf(" sound_timer: 0x%x\n", chip8->sound_timer);
    // print stack
    printf(" stack: \n");
    for (int i = 0; i < STACK; i++)
    {
        printf("0x%02x\n", chip8->stack.stack[i]);
    }

    // dump memory
    printf(" memory: \n");
    for (int i = 1; i < MEM_SIZE + 1; i++)
    {
        printf("0x%02x ", chip8->memory[i - 1]);
        if (i % 16 == 0)
            printf("\n");
    }
    exit(1);
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

    // load the fontset
    memset(&chip8->memory[0x50], 0, 80);
    memcpy(&chip8->memory[0x50], chip8_fontset, 80);

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
    // printf("opcode: 0x%04x\n", opcode);
    BYTE instruction = (opcode & 0xF000) >> 12; // get the first 4 bits of the opcode
    // Decode opcode
    switch (instruction)
    {
    case 0x0: // 0NNN
        switch (opcode)
        {
        case 0x00E0: // 00E0
            chip8_00E0(chip8);
            break;
        case 0x00EE: // 00EE
            chip8_00EE(chip8);
            break;
        default:
            printf("Unknown opcode: 0x%X\n", opcode);
            diagnose(chip8, opcode);
            break;
        }
        break;
    case 0x1: // 1NNN
        chip8_1NNN(chip8, opcode);
        break;
    case 0x2: // 2NNN
        chip8_2NNN(chip8, opcode);
        break;
    case 0x3: // 3XNN
        chip8_3XNN(chip8, opcode);
        break;
    case 0x4: // 4XNN
        chip8_4XNN(chip8, opcode);
        break;
    case 0x5: // 5XY0
        chip8_5XY0(chip8, opcode);
        break;
    case 0x6: // 6XNN
        chip8_6XNN(chip8, opcode);
        break;
    case 0x7: // 7XNN
        chip8_7XNN(chip8, opcode);
        break;
    case 0x8: // 8XYN
        switch (opcode & 0x000F)
        {
        case 0x0: // 8XY0
            chip8_8XY0(chip8, opcode);
            break;
        case 0x1: // 8XY1
            chip8_8XY1(chip8, opcode);
            break;
        case 0x2: // 8XY2
            chip8_8XY2(chip8, opcode);
            break;
        case 0x3: // 8XY3
            chip8_8XY3(chip8, opcode);
            break;
        case 0x4: // 8XY4
            chip8_8XY4(chip8, opcode);
            break;
        case 0x5: // 8XY5
            chip8_8XY5(chip8, opcode);
            break;
        case 0x6: // 8XY6
            chip8_8XY6(chip8, opcode);
            break;
        case 0x7: // 8XY7
            chip8_8XY7(chip8, opcode);
            break;
        case 0xE: // 8XYE
            chip8_8XYE(chip8, opcode);
            break;
        default:
            printf("Unknown opcode: 0x%X\n", opcode);
            diagnose(chip8, opcode);
            break;
        }
        break;
    case 0x9: // 9XY0
        chip8_9XY0(chip8, opcode);
        break;
    case 0xA: // ANNN
        chip8_ANNN(chip8, opcode);
        break;
    case 0xB: // BNNN
        chip8_BNNN(chip8, opcode);
        break;
    case 0xC: // CXNN
        chip8_CXNN(chip8, opcode);
        break;
    case 0xD: // DXYN
        chip8_DXYN(chip8, opcode);
        break;
    case 0xE: // EXNN
        switch (opcode & 0x00FF)
        {
        case 0x9E: // EX9E
            chip8_EX9E(chip8, opcode);
            break;
        case 0xA1: // EXA1
            chip8_EXA1(chip8, opcode);
            break;
        default:
            printf("Unknown opcode: 0x%X\n", opcode);
            diagnose(chip8, opcode);
            break;
        }
        break;
    case 0xF: // FXNN
        switch (opcode & 0x00FF)
        {
        case 0x07: // FX07
            chip8_FX07(chip8, opcode);
            break;
        case 0x0A: // FX0A
            chip8_FX0A(chip8, opcode);
            break;
        case 0x15: // FX15
            chip8_FX15(chip8, opcode);
            break;
        case 0x18: // FX18
            chip8_FX18(chip8, opcode);
            break;
        case 0x1E: // FX1E
            chip8_FX1E(chip8, opcode);
            break;
        case 0x29: // FX29
            chip8_FX29(chip8, opcode);
            break;
        case 0x33: // FX33
            chip8_FX33(chip8, opcode);
            break;
        case 0x55: // FX55
            chip8_FX55(chip8, opcode);
            break;
        case 0x65: // FX65
            chip8_FX65(chip8, opcode);
            break;
        default:
            printf("Unknown opcode: 0x%X\n", opcode);
            diagnose(chip8, opcode);
            break;
        }
        break;
    default:
        printf("Unknown opcode: 0x%X\n", opcode);
        diagnose(chip8, opcode);
        break;
    }

    // Update timers
    chip8->pc += 2;
}

// Set the keys
void chip8_set_keys(CHIP8 *chip8)
{
    // TODO
}

// Draw the screen
void chip8_draw_screen(CHIP8 *chip8, SDL_Renderer *renderer)
{
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw the pixels
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int y = 0; y < 32; y++)
    {
        for (int x = 0; x < 64; x++)
        {
            if (chip8->gfx[y * 64 + x] == 1)
            {
                printf("x: %d, y: %d" , x, y);
                // set the pixel
                SDL_Rect rect = {x * 10, y * 10, 10, 10};
                SDL_RenderFillRect(renderer, &rect);

            }
        }
    }

    // Update the screen
    SDL_RenderPresent(renderer);
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

    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 320, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);

    // Main loop
    int quit = 0;

    while (!quit)
    {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = 1;
                break;
            }
        }

        // Emulate cycle
        chip8_emulate_cycle(&chip8);

        // Draw the screen
        chip8_draw_screen(&chip8, renderer);
    }

    // Cleanup
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
