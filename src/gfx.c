//
// Created by GrappigPanda on 12/28/15.
//
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "GL/glut.h"

#include "gfx.h"

struct chip8_gfx* gfx_init() {
    struct chip8_gfx* tmp = malloc(2 * sizeof(struct chip8_gfx));

    tmp->title = "Testing Window";

    memset(tmp->display_buffer, 0, 2048);

    tmp->width = DISPLAY_WIDTH;
    tmp->height = DISPLAY_HEIGHT;

    tmp->draw_flag = false;

    return tmp;
}

void gfx_draw(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
		glVertex3f(-0.5f,-0.5f,0.0);
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
    /*Set-up glut functions for different events.*/
    glutDisplayFunc(gfx_draw);
    glutReshapeFunc(window_reshape);
    glutKeyboardFunc(kb_up);
    glutKeyboardFunc(kb_down);

    glutMainLoop();
}

void gfx_clear_screen() {
    return;
}

void window_reshape(GLsizei w, GLsizei h) {
    glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
}

void kb_up(unsigned char key, int x, int y)  {

}

void kb_down(unsigned char key, int x, int y)  {

}
