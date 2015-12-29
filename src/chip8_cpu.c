//
// Created by GrappigPanda on 12/28/15.
//
#include <string.h>
#include <stdlib.h>

#include "chip8_cpu.h"

void cpu_memzero(struct chip8_cpu* cpu) {
    memset(cpu->memory, 0, 4096);
    memset(cpu->V, 0, 16);
    memset(cpu->stack, 0 , 16);
}

struct chip8_cpu* cpu_init() {
    struct chip8_cpu* tmp = malloc(2 * sizeof(struct chip8_cpu));

    cpu_memzero(tmp);

    tmp->I = 0;
    tmp->stack_pointer = 0;
    tmp->counter = 0;

    tmp->delay_timer = 0;
    tmp->sound_timer = 0;

    return tmp;
}

void run_cpu_cycle(struct chip8_cpu* cpu) {
    unsigned short op = cpu->memory[cpu->counter] << 8 |
                        cpu->memory[cpu->counter + 1];

    switch(op & 0xF000) {
        case(0x0000): {
            switch(op & 0x000F) {
                case(0x0000): {
                    // 0x1NNN <-> Jumps to address NNN.
                    gfx_clear_screen();
                    break;
                }
                case(0x000E): {
                    // 0x1NNN <-> Jumps to address NNN.
                    break;
                }
            }
        }
        case(0x1000): {
            // 0x1NNN <-> Jumps to address NNN.
            push_addr(cpu);
            cpu->counter = op & 0x0FFF;
            break;
        }
        case(0x2000): {
            // 0x2NNN <-> Calls Subroutine at address NNN.
            push_addr(cpu);
            cpu->counter = op & 0x0FFF;
            break;
        }
        case(0xA000): {
            // 0x1NNN <-> Jumps to address NNN.
            cpu->I = op & 0x0FFF;
            cpu->counter += 2;
            break;
        }
        case(0xB000): {
            // 0xBNNN <-> Jumps to address NNN plus V0
            push_addr(cpu);
            cpu->counter = (op & 0x0FFF) + cpu->V[0];
            break;
        }
        case(0xC000): {
            // 0x1NNN <-> Jumps to address NNN.
            break;
        }
        case(0xD000): {
            // 0x1NNN <-> Jumps to address NNN.
            break;
        }
        case(0xE000): {
            // 0x1NNN <-> Jumps to address NNN.
            break;
        }
        case(0xF000): {
            switch(op & 0x000F) {
                 case(0x0005): {
                     // 0xFX15 <-> Sets the delay timer to V[X]
                     int x = op & 0x0F00;
                     break;
                }
            }
            break;
        }
        default:
            // Should we do anything here?
            break;
    }
}

void push_addr(struct chip8_cpu* cpu) {
    // Store the current address (cpu->counter) into the stack and update the
    // stack pointer
    cpu->stack[cpu->stack_pointer] = cpu->counter;
    cpu->stack_pointer++;
}
