//
// Created by GrappigPanda on 12/28/15.
//

#ifndef CHIP8EMU_CHIP8_CPU_H
#define CHIP8EMU_CHIP8_CPU_H

#include "gfx.h"

struct chip8_cpu {
    unsigned char memory[4096]; // memory region -- 8 bits wide per mem location
    unsigned char V[16]; // 16 Registers -- 8 bits wide each register
    unsigned short I; // Address register -- 16 bit wide

    unsigned short counter;

    unsigned short stack[16];
    unsigned short stack_pointer;

    unsigned int delay_timer; // Times events of the game
    unsigned int sound_timer; // When non-zero, beep
};

struct chip8_cpu* cpu_init();
void cpu_memzero(struct chip8_cpu*);
void run_cpu_cycle(struct chip8_cpu*);
unsigned char* load_rom();


// Start of helper functions
void push_addr(struct chip8_cpu*);

#endif //CHIP8EMU_CHIP8_CPU_H
