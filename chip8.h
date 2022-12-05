// This is a CHIP8 emulator written in C.
typedef unsigned char BYTE; // 8 bits/1 byte
typedef unsigned short int WORD; // 16 bits/2 bytes

// CHIP8 has 4K of memory
#define MEM_SIZE 4096

// CHIP8 has 16 8-bit registers
#define REGISTERS 16

// CHIP8 has 16 keys
#define KEYS 16

// CHIP8 has 16 levels of stack
#define STACK 16

// CHIP8 has 64x32 pixels
#define WIDTH 64
#define HEIGHT 32

#define fps 60

// Stack
typedef struct Stack{
    WORD stack[STACK];
    WORD sp;
} Stack;

// CHIP8 System
typedef struct CHIP8 {
    BYTE memory[MEM_SIZE];
    BYTE V[REGISTERS];
    WORD I;
    WORD pc;
    BYTE delay_timer;
    BYTE sound_timer;
    Stack stack;
    BYTE key[KEYS];
    BYTE gfx[HEIGHT * WIDTH];
    BYTE draw_flag;
} CHIP8;

// Function prototypes
void chip8_init(struct CHIP8 *chip8);
int chip8_load(struct CHIP8 *chip8, char *filename);
void chip8_emulate_cycle(struct CHIP8 *chip8);


// Stack functions 
void stack_push(Stack *stack, WORD value);
WORD stack_pop(Stack *stack);

// fontset
const BYTE chip8_fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

// DEBUGGING FUNCTIONS
void GFX_DEBUG(CHIP8 *chip8)
{
    printf("GFX: \n");
    for (int i = 1; i < HEIGHT*WIDTH+1; i++)
    {
        printf("%d  ", chip8->gfx[i-1]);
        if (i % 64 == 0)
            printf("\n");
    }
    // enter to continue
    // getchar();
}