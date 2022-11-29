// This is a CHIP8 emulator written in C.

typedef unsigned char BYTE;
typedef unsigned short int WORD; 

// CHIP8 has 4K of memory
#define MEM_SIZE 4096

// CHIP8 has 16 8-bit registers
#define REGISTERS 16

// CHIP8 has 16 keys
#define KEYS 16

// CHIP8 has 16 levels of stack
#define STACK 16

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
    BYTE gfx[64 * 32];
    BYTE draw_flag;
} CHIP8;


// Function prototypes
void chip8_init(struct CHIP8 *chip8);
int chip8_load(struct CHIP8 *chip8, char *filename);
void chip8_emulate_cycle(struct CHIP8 *chip8);
void chip8_set_keys(struct CHIP8 *chip8);


// Stack functions 
void stack_push(Stack *stack, WORD value);
WORD stack_pop(Stack *stack);