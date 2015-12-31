//
// Created by GrappigPanda on 12/28/15.
//

#ifndef CHIP8EMU_GFX_H
#define CHIP8EMU_GFX_H

#include "GL/glut.h"

#define DISPLAY_WIDTH 500
#define DISPLAY_HEIGHT 250

typedef int bool;
enum {
    false, true
};

typedef struct chip8_gfx {
    char* title;
    unsigned char display_buffer[2048];

    unsigned int width;
    unsigned int height;

    bool draw_flag;

} chip8_gfx;

chip8_gfx* gfx_init();

void gfx_create_window(int, char**, chip8_gfx*);
void gfx_clear_screen();
void gfx_draw();

void window_reshape(GLsizei, GLsizei);
void kb_up(unsigned char, int, int);
void kb_down(unsigned char, int, int);

#endif //CHIP8EMU_GFX_H
