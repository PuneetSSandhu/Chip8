// This is a CHIP8 emulator written in C.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chip8_instructions.c"
#include <time.h>
#include <unistd.h>
#include <math.h>
// SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

Mix_Chunk *beep;

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

void cap_fps(Uint32 start)
{
    //     printf("1000/fps = %d\n", 1000/fps);
    //     printf("SDL_GetTicks() - start = %d\n", SDL_GetTicks() - start);
    //     SDL_Delay(100);
    if (200/ fps > SDL_GetTicks() - start)
    {
        SDL_Delay(200/ fps - (SDL_GetTicks() - start));
    }
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
    memset(&chip8->memory[0], 0, 80);
    memcpy(&chip8->memory[0], chip8_fontset, 80);
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
    WORD opcode = (chip8->memory[chip8->pc] << 8) | chip8->memory[chip8->pc + 1];
    chip8->pc += 2;
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
            break;
        }
        break;
    default:
        printf("Unknown opcode: 0x%X\n", opcode);
        break;
    }

    // Update timers
    if (chip8->delay_timer > 0)
    {
        chip8->delay_timer--;
    }
    if (chip8->sound_timer > 0)
    {
        if (chip8->sound_timer == 1)
        {
            Mix_PlayChannel(-1, beep, 0);
        }
        chip8->sound_timer--;
    }
}

// Draw the screen
void chip8_draw_screen(CHIP8 *chip8, SDL_Renderer *renderer, SDL_Texture *texture)
{
    SDL_RenderClear(renderer);
    // wait for the renderer to be ready

    uint32_t pixels[WIDTH * HEIGHT];
    for (int i = 0; i < WIDTH * HEIGHT; i++)
    {
        pixels[i] = (0x00FFFFFF * chip8->gfx[i]) | 0xFF000000;
    }

    // Update SDL texture
    SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(Uint32));
    SDL_RenderCopy(renderer, texture, NULL, NULL);
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
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_Window *window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH * 10, HEIGHT * 10, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
    
    // Initialize audio
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 1, 4096);
    beep = Mix_LoadWAV("beep.wav");
    Mix_VolumeChunk(beep, MIX_MAX_VOLUME/5);
    // Main loop
    int quit = 0;
    int start;
    // get the current time and strore it
    while (!quit)
    {
        // add time for 1/60th of a second
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            // key pressed
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_1:
                    chip8.key[0x1] = 1;
                    break;
                case SDLK_2:
                    chip8.key[0x2] = 1;
                    break;
                case SDLK_3:
                    chip8.key[0x3] = 1;
                    break;
                case SDLK_4:
                    chip8.key[0xC] = 1;
                    break;
                case SDLK_q:
                    chip8.key[0x4] = 1;
                    break;
                case SDLK_w:
                    chip8.key[0x5] = 1;
                    break;
                case SDLK_e:
                    chip8.key[0x6] = 1;
                    break;
                case SDLK_r:
                    chip8.key[0xD] = 1;
                    break;
                case SDLK_a:
                    chip8.key[0x7] = 1;
                    break;
                case SDLK_s:
                    chip8.key[0x8] = 1;
                    break;
                case SDLK_d:
                    chip8.key[0x9] = 1;
                    break;
                case SDLK_f:
                    chip8.key[0xE] = 1;
                    break;
                case SDLK_z:
                    chip8.key[0xA] = 1;
                    break;
                case SDLK_x:
                    chip8.key[0x0] = 1;
                    break;
                case SDLK_c:
                    chip8.key[0xB] = 1;
                    break;
                case SDLK_v:
                    chip8.key[0xF] = 1;
                    break;
                }
                break;
            // key released
            case SDL_KEYUP:
                switch (event.key.keysym.sym)
                {
                case SDLK_1:
                    chip8.key[0x1] = 0;
                    break;
                case SDLK_2:
                    chip8.key[0x2] = 0;
                    break;
                case SDLK_3:
                    chip8.key[0x3] = 0;
                    break;
                case SDLK_4:
                    chip8.key[0xC] = 0;
                    break;
                case SDLK_q:
                    chip8.key[0x4] = 0;
                    break;
                case SDLK_w:
                    chip8.key[0x5] = 0;
                    break;
                case SDLK_e:
                    chip8.key[0x6] = 0;
                    break;
                case SDLK_r:
                    chip8.key[0xD] = 0;
                    break;
                case SDLK_a:
                    chip8.key[0x7] = 0;
                    break;
                case SDLK_s:
                    chip8.key[0x8] = 0;
                    break;
                case SDLK_d:
                    chip8.key[0x9] = 0;
                    break;
                case SDLK_f:
                    chip8.key[0xE] = 0;
                    break;
                case SDLK_z:
                    chip8.key[0xA] = 0;
                    break;
                case SDLK_x:
                    chip8.key[0x0] = 0;
                    break;
                case SDLK_c:
                    chip8.key[0xB] = 0;
                    break;
                case SDLK_v:
                    chip8.key[0xF] = 0;
                    break;
                }
                break;
            case SDL_QUIT:
                quit = 1;
                break;
            }
        }

        Uint32 start = SDL_GetTicks();

        // Emulate cycle
        chip8_emulate_cycle(&chip8);

        cap_fps(start);
        if (chip8.draw_flag)
        {

            chip8.draw_flag = 0;
            chip8_draw_screen(&chip8, renderer, texture);
        }
    }

    // Cleanup
    Mix_FreeChunk(beep);
    Mix_CloseAudio();
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
