// DEBUGGING FUNCTIONS
void GFX_DEBUG(CHIP8 *chip8)
{
    printf("GFX: \n");
    for (int i = 0; i < HEIGHT*WIDTH; i++)
    {
        printf("%d ", chip8->gfx[i]);
        if (i % 64 == 0)
            printf("\n");
    }
    // enter to continue
    // getchar();
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

    // Print next 10 instructions
    printf(" next 10 instructions: \n");
    for (int i = 0; i < 20; i+=2)
    {
        printf("0x%04x\n", chip8->memory[chip8->pc + i] << 8 | chip8->memory[chip8->pc + i + 1]);
    }
    exit(1);
}