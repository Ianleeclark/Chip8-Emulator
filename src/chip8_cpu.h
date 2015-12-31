#include "gfx.h"//
// Created by GrappigPanda on 12/28/15.
//

#ifndef CHIP8EMU_CHIP8_CPU_H
#define CHIP8EMU_CHIP8_CPU_H

#include "gfx.h"


struct chip8_cpu {
    unsigned char memory[4096]; // memory region -- 8 bits wide per mem location
    unsigned char V[16]; // 16 Registers -- 8 bits wide each register
    unsigned short I; // Address register -- 16 bit wide

    chip8_gfx* gfx;

    unsigned short counter;

    unsigned short stack[16];
    unsigned short stack_pointer;

    unsigned int delay_timer; // Times events of the game
    unsigned int sound_timer; // When non-zero, beep
};

struct chip8_cpu* cpu_init();
void cpu_memzero(struct chip8_cpu*);
void run_cpu_cycle(struct chip8_cpu*);
void load_rom(struct chip8_cpu*, const char*);


// Start of helper functions
void push_addr(struct chip8_cpu*);
unsigned int lookup_bit_values(unsigned int);
unsigned int find_significant_bit(int, unsigned short);
#endif //CHIP8EMU_CHIP8_CPU_H
