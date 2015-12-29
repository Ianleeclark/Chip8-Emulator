//
// Created by GrappigPanda on 12/28/15.
//

#ifndef CHIP8EMU_GFX_H
#define CHIP8EMU_GFX_H

#define DISPLAY_WIDTH 500
#define DISPLAY_HEIGHT 250

typedef struct chip8_gfx {
    char* title;
} chip8_gfx;

chip8_gfx* gfx_init();
void gfx_create_window(int, char**, chip8_gfx*);
void gfx_clear_screen();
void gfx_draw();

#endif //CHIP8EMU_GFX_H
