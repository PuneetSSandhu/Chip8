// This is a CHIP8 emulator written in C.
// For debugging purposes
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 


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

// CHIP8 System
typedef struct chip8 {
    BYTE memory[MEM_SIZE];
    BYTE V[REGISTERS];
    WORD I;
    WORD pc;
    BYTE delay_timer;
    BYTE sound_timer;
    WORD stack[STACK];
    WORD sp;
    BYTE key[KEYS];
    BYTE gfx[64 * 32];
    BYTE draw_flag;
}chip8;


// Function prototypes
void chip8_init(struct chip8 *chip8);
int chip8_load(struct chip8 *chip8, char *filename);
void chip8_emulate_cycle(struct chip8 *chip8);
void chip8_set_keys(struct chip8 *chip8);
