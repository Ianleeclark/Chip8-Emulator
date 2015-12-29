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
                    // 0x00E0 <-> Clears the display.
                    gfx_clear_screen();
                    break;
                }
                case(0x000E): {
                    // 0x00EE <-> Return from sub-routine.
                    // TODO(ian): Brush up on c and see if you can do --cpu->stack_pointer here.
                    cpu->counter = cpu->stack[cpu->stack_pointer - 1];
                    cpu->stack_pointer--;
                    break;
                }
            }
        }
        case(0x1000): {
            // 0x1NNN <-> Jumps to address NNN.
            cpu->counter = op & 0x0FFF;
            break;
        }
        case(0x2000): {
            // 0x2NNN <-> Calls Subroutine at address NNN.
            push_addr(cpu);
            cpu->counter = op & 0x0FFF;
            break;
        }
        case(0x3000): {
            // 0x3XKK <-> Skip next instruction if V[x] == KK
            unsigned int x = op & 0x0F00;
            if(cpu->V[x] == op & 0x00FF)
                cpu->counter += 2;
            break;
        }
        case(0x4000): {
            // 0x4XKK <-> Skip next instruction if V[x] != KK
            unsigned int x = op & 0x0F00;
            if(cpu->V[x] != op & 0x00FF) {
                cpu->counter += 2;
                break;
            }
            ++cpu->counter;
            break;
        }
        case(0x5000): {
            // 0x5xy0 <-> Skip Next Instruction if V[x] == V[y].
            unsigned int x = lookup_bit_values(op & 0x0F00);
            unsigned int y = lookup_bit_values(op & 0x00F0);
            if(cpu->V[x] != cpu->V[y]) {
                cpu->counter += 2;
                break;
            }
            ++cpu->counter;
            break;
        }
        case(0x6000): {
            // 0x6XKK -> Set V[X] to KK
            cpu->V[lookup_bit_values(op & 0x0F00)] = op & 0x00FF;
            ++cpu->counter;
            break;
        }
        case(0x7000): {
            // 0x7XKK -> Set V[X] to V[X] + KK
            cpu->V[lookup_bit_values(op & 0x0F00)] += op & 0x00FF;
            ++cpu->counter;
            break;
        }
        case(0x8000): {
            switch(op & 0x000F) {
                case(0x0000): {
                    // 0x8XY0 -> Set V[X] to V[Y]
                    cpu->V[lookup_bit_values(op & 0x0F00)] = cpu->V[lookup_bit_values(op & 0x00F0)];
                    ++cpu->counter;
                    break;
                }
                case(0x0001): {
                    // 0x8XY1 -> Set V[X] to V[X] OR V[Y]

                    ++cpu->counter;
                    break;
                }
                case(0x0002): {
                    // 0x8XY2 -> Set V[X] to V[X] AND V[Y]

                    ++cpu->counter;
                    break;
                }
                case(0x0003): {
                    // 0x8XY3 -> Set V[X] to V[X] XOR V[Y]

                    ++cpu->counter;
                    break;
                }
            }
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
            switch(op & 0x00FF) {
                case(0x0007): {
                    // 0xFX07 <-> Set V[x] to the value of the delay timer.
                    unsigned int x = lookup_bit_values(op & 0x0F00);
                    cpu->V[x] = cpu->delay_timer;
                    break;
                }
                case(0x000A): {
                    // 0xFX0A <-> Set V[x] to the value the next key press.
                    unsigned int x = lookup_bit_values(op & 0x0F00);
                    cpu->V[x] = 0; // TODO(ian): Implement key handling function to get keypress
                    break;
                }
                case(0x0015): {
                    // 0xFX15 <-> Sets the delay timer to V[X]
                    unsigned int x = lookup_bit_values(op & 0x0F00);
                    cpu->delay_timer = cpu->V[x];
                    break;
                }
                case(0x0018): {
                    // 0xFX18 <-> Sets the sound timer to V[X]
                    unsigned int x = lookup_bit_values(op & 0x0F00);
                    cpu->sound_timer = cpu->V[x];
                    break;
                }
                case(0x001E): {
                    // 0xFX1E <-> Adds V[x] to cpu->I
                    unsigned int x = lookup_bit_values(op & 0x0F00);
                    cpu->I += cpu->V[x];
                    break;
                }
                case(0x0029): {

                    // 0xFX29 <-> Set cpu->I to the location of sprite for digit cpu->V[x]
                    unsigned int x = lookup_bit_values(op & 0x0F00);
                    // TODO(ian): Implement this opcode, not sure what it means exactly.
                    break;
                }
                case(0x0033): {
                    // 0xFx33 <->
                    unsigned int x = lookup_bit_values(op & 0x0F00);
                    // TODO(ian): Implement this opcode, not sure what it means exactly.
                    break;
                }
                case(0x0055): {
                    // 0xFx55 <-> Write V[0->16] to memory[i->16]
                    unsigned int i = 0;
                    for(i; i <= 16; i++) {
                        cpu->memory[i] = cpu->V[i];
                    }
                    break;
                }
                case(0x0065): {
                    // 0xFx65 <-> Opposite of 0x0055
                    unsigned int i = 0;
                    for(i; i <= 16; i++) {
                        cpu->V[i] = cpu->memory[i];
                    }
                    break;
                }
                default:
                    break;
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

unsigned int lookup_bit_values(int bit_value) {
    int vals[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int bit_values[] = {0, 256, 512, 768, 1024, 1280, 1536, 1792, 2048, 2304};

    unsigned int i;

    for(i = 0; i <= 9; i++) {
        if(bit_values[i] == bit_value)
            return vals[i];
    }
    return -1;
}
