//
// Created by GrappigPanda on 12/28/15.
//
#include <string.h>
#include <stdlib.h>

#include "GL/glut.h"

#include "gfx.h"

struct chip8_gfx* gfx_init() {
    struct chip8_gfx* tmp = malloc(2 * sizeof(struct chip8_gfx));

    tmp->title = "Testing Window";

    return tmp;
}

void gfx_draw(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
		glVertex3f(-0.5,-0.5,0.0);
		glVertex3f(0.5,0.0,0.0);
		glVertex3f(0.0,0.5,0.0);
    glEnd();
    glFlush();
}

void gfx_create_window(int argc, char** argv, chip8_gfx* gfx) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(50,25);
    glutInitWindowSize(DISPLAY_WIDTH, DISPLAY_HEIGHT);

    glutCreateWindow(gfx->title);
    glutDisplayFunc(gfx_draw);
    glutMainLoop();
}

void gfx_clear_screen() {

}