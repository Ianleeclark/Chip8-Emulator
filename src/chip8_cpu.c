//
// Created by GrappigPanda on 12/28/15.
//
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
                    cpu->counter = cpu->stack[--cpu->stack_pointer - 1];
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
            unsigned int x = op & 0x0F00 >> 8;
            if(cpu->V[x] == op & 0x00FF)
                cpu->counter += 4;
            else
                cpu->counter += 2;
            break;
        }
        case(0x4000): {
            // 0x4XKK <-> Skip next instruction if V[x] != KK
            unsigned int x = op & 0x0F00;
            if(cpu->V[x] != op & 0x00FF)
                cpu->counter += 4;
            else
                cpu->counter += 2;
            break;
        }
        case(0x5000): {
            // 0x5xy0 <-> Skip Next Instruction if V[x] == V[y].
            unsigned int x = lookup_bit_values(op & 0x0F00);
            unsigned int y = lookup_bit_values(op & 0x00F0);
            if(cpu->V[x] != cpu->V[y])
                cpu->counter += 4;
            else
                cpu->counter += 2;
            break;
        }
        case(0x6000): {
            // 0x6XKK -> Set V[X] to KK
            cpu->V[lookup_bit_values(op & 0x0F00)] = op & 0x00FF;
            cpu->counter += 2;
            break;
        }
        case(0x7000): {
            // 0x7XKK -> Set V[X] to V[X] + KK
            cpu->V[lookup_bit_values(op & 0x0F00)] += op & 0x00FF;
            cpu->counter += 2;
            break;
        }
        case(0x8000): {
            switch(op & 0x000F) {
                case(0x0000): {
                    // 0x8XY0 -> Set V[X] to V[Y]
                    cpu->V[lookup_bit_values(op & 0x0F00)] = cpu->V[lookup_bit_values(op & 0x00F0)];
                    cpu->counter += 2;
                    break;
                }
                case(0x0001): {
                    // 0x8XY1 -> Set V[X] to V[X] OR V[Y]
                    unsigned int x = lookup_bit_values(op & 0x0F00);
                    cpu->V[x] |= cpu->V[lookup_bit_values(op & 0x00F0)];
                    cpu->counter += 2;
                    break;
                }
                case(0x0002): {
                    // 0x8XY2 -> Set V[X] to V[X] AND V[Y]
                    unsigned int x = lookup_bit_values(op & 0x0F00);
                    cpu->V[x] &= cpu->V[lookup_bit_values(op & 0x00F0)];
                    cpu->counter += 2;
                    break;
                }
                case(0x0003): {
                    // 0x8XY3 -> Set V[X] to V[X] XOR V[Y]
                    unsigned int x = lookup_bit_values(op & 0x0F00);
                    cpu->V[x] ^= cpu->V[lookup_bit_values(op & 0x00F0)];
                    cpu->counter += 2;
                    break;
                }
                case(0x0004): {
                    // 0x8XY4 -> Set V[X] += V[y], if V[x] > V[y], v[0xF] = 1
                    unsigned int x = lookup_bit_values(op & 0x0F00);
                    unsigned int y = lookup_bit_values(op & 0x00F0);

                    if(((int)cpu->V[x] + (int)cpu->V[y]) > 0)
                        cpu->V[0xF] = 1;
                    else
                        cpu->V[0xF] = 0;

                    cpu->V[x] += cpu->V[y];

                    cpu->counter += 2;
                    break;
                }
                case(0x0005): {
                    // 0x8XY5 -> Set V[X] -= V[y], if V[x] > V[y], v[0xF] = 1
                    unsigned int x = lookup_bit_values(op & 0x0F00);
                    unsigned int y = lookup_bit_values(op & 0x00F0);

                    if(cpu->V[x] > cpu->V[y])
                        cpu->V[0xF] = 1;
                    else
                        cpu->V[0xF] = 0;

                    cpu->V[x] -= cpu->V[y];

                    cpu->counter += 2;
                    break;
                }
                case(0x0006): {
                    // 0x8XY6 ->

                    break;
                }
                case(0x0007): {
                    // 0x8XY5 -> Set V[X] -= V[y], if V[x] > V[y], v[0xF] = 1
                    unsigned int x = lookup_bit_values(op & 0x0F00);
                    unsigned int y = lookup_bit_values(op & 0x00F0);

                    if(cpu->V[y] > cpu->V[x])
                        cpu->V[0xF] = 1;
                    else
                        cpu->V[0xF] = 0;

                    cpu->V[x] = cpu->V[y] - cpu->V[x];

                    cpu->counter += 2;
                    break;
                }
                case(0x000E): {
                    // 0x8XYE ->
                    unsigned int x = lookup_bit_values(op & 0x0F00);
                    cpu->V[x] &= cpu->V[lookup_bit_values(op & 0x00F0)];
                    cpu->counter += 2;
                    break;
                }
            }
        }
        case(0x9000): {
            // 0x9XY0 <-> Skips net instruction if VX != VY
            unsigned int x = lookup_bit_values(op & 0x0F00);
            unsigned int y = lookup_bit_values(op & 0x00F0);
            if(cpu->V[x] != cpu->V[y])
                cpu->counter += 4;
            else
                cpu->counter += 2;
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
            cpu->counter = (op & 0x0FFF) + cpu->V[0];
            break;
        }
        case(0xC000): {
            // 0xCNNN <-> Sets V[X] to rand() AND NN
            unsigned int x = op & 0x0F00 >> 8;
            unsigned int nn = op & 0x00FF;

            cpu->V[x] = rand() & nn;

            cpu->counter += 2;
            break;
        }
        case(0xD000): {
            // 0x1NNN <-> Jumps to address NNN.
            break;
        }
        case(0xF000): {
            switch(op & 0x00FF) {
                case(0x0007): {
                    // 0xFX07 <-> Set V[x] to the value of the delay timer.
                    unsigned int x = lookup_bit_values(op & 0x0F00);
                    cpu->V[x] = cpu->delay_timer;
                    cpu->counter += 2;
                    break;
                }
                case(0x000A): {
                    // 0xFX0A <-> Set V[x] to the value the next key press.
                    unsigned int x = lookup_bit_values(op & 0x0F00);
                    cpu->V[x] = 0;
                    // TODO(ian): Implement key handling function to get key
                    cpu->counter += 2;
                    break;
                }
                case(0x0015): {
                    // 0xFX15 <-> Sets the delay timer to V[X]
                    unsigned int x = lookup_bit_values(op & 0x0F00);
                    cpu->delay_timer = cpu->V[x];
                    cpu->counter += 2;
                    break;
                }
                case(0x0018): {
                    // 0xFX18 <-> Sets the sound timer to V[X]
                    unsigned int x = lookup_bit_values(op & 0x0F00 >> 8);
                    cpu->sound_timer = cpu->V[x];
                    cpu->counter += 2;
                    break;
                }
                case(0x001E): {
                    // 0xFX1E <-> Adds V[x] to cpu->I
                    unsigned int x = lookup_bit_values(op & 0x0F00);
                    cpu->I += cpu->V[x];
                    cpu->counter += 2;
                    break;
                }
                case(0x0029): {
                    // 0xFX29 <-> Set cpu->I to the location of sprite for digit cpu->V[x]
                    unsigned int x = lookup_bit_values(op & 0x0F00);
                    // TODO(ian): Implement this opcode, not sure what it means exactly.
                    cpu->counter += 2;
                    break;
                }
                case(0x0033): {
                    // 0xFx33 <->
                    unsigned int x = lookup_bit_values(op & 0x0F00);
                    // TODO(ian): Implement this opcode, not sure what it means exactly.
                    cpu->counter += 2;
                    break;
                }
                case(0x0055): {
                    // 0xFx55 <-> Write V[0->16] to memory[i->16]
                    unsigned int i = 0;
                    for(i; i <= 16; i++) {
                        cpu->memory[i] = cpu->V[i];
                    }
                    cpu->counter += 2;
                    break;
                }
                case(0x0065): {
                    // 0xFx65 <-> Opposite of 0x0055
                    unsigned int i = 0;
                    for(i; i <= 16; i++) {
                        cpu->V[i] = cpu->memory[i];
                    }
                    cpu->counter += 2;
                    break;
                }
                default:
                    printf("I'm not entirely sure how were here: %x", op);
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

unsigned int lookup_bit_values(unsigned int bit_value) {
    unsigned int i = 0;
    unsigned int value_cliffs[] = {16, 256, 4096};
    unsigned int return_val = 0;

    if(bit_value == 0)
        return_val = 0;
    else if(bit_value < 16)
        return_val = bit_value;


    for(i = 0; i <= 2; ++i) {
        if(bit_value >= value_cliffs[i])
            return_val = bit_value / value_cliffs[i];
    }
    return return_val;
}
