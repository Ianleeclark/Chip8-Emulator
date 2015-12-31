#include <stdio.h>

#include "src/chip8_cpu.h"
#include "src/gfx.h"
#include "src/sound.h"

void game_loop(struct chip8_cpu* cpu, struct chip8_gfx* gfx) {
    for(unsigned int i = 0; i < sizeof(cpu->memory); i++) {
        run_cpu_cycle(cpu);

        if(cpu->delay_timer > 0)
            --cpu->delay_timer;
        if(cpu->sound_timer != 0)
           beep();
    }
}

int main(int argc, char **argv) {
    struct chip8_cpu* cpu = cpu_init();
    chip8_gfx* gfx = gfx_init();

    load_rom(cpu, "PONG");
    game_loop(cpu, gfx);

    return 0;
}