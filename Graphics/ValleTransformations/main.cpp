/////////////////////////////////
// main.cpp
//
// This program draws a wire box.
//
// Sumanta Guha, modified by Cody Valle.
/////////////////////////////////

#include <iostream>

#  include <GL/glew.h>
#  include <GL/freeglut.h>

#ifdef __APPLE__
#  include <OpenGL/glext.h>
#else
#  include <GL/glext.h>
#endif

// Translate globals
float xoffset = 0;
float yoffset = 0;
float zoffset = -10;

// Scale globals
float xscale = 1;
float yscale = 1;
float zscale = 1;

// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0, 0, 0);
    glLoadIdentity();

    // Modeling transformations.
    //glTranslatef(xoffset, yoffset, zoffset);
    //glScalef(xscale, yscale, zscale);

    /*glTranslatef(0, 0, -15);
    glRotatef(45, 0, 0, 1);
    glScalef(2, 1, 0.5);

    glutWireTeapot(5); // Teapot*/

    // Robot
    glEnable(GL_DEPTH_TEST); // Enable depth testing.
    glLoadIdentity();
    glTranslatef(0, 0, -20);
    glScalef(1, 2, 1);
    glutSolidCube(5);
    glColor3f(0, 1, 1);
    glLoadIdentity();
    glTranslatef(0, 7.5, -20);
    glutSolidSphere(2.5, 20, 20);
    glDisable(GL_DEPTH_TEST); // Disable depth testing.

    glFlush();
}

// Initialization routine.
void setup(void)
{
    glClearColor(1, 1, 1, 0);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5, 5, -5, 5, 5, 100);

    glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 'q':
        ++zoffset;
        break;

    case 'e':
        --zoffset;
        break;

    case 's':
        --yoffset;
        break;

    case 'w':
        ++yoffset;
        break;

    case 'a':
        --xoffset;
        break;

    case 'i':
        yscale *= 2;
        break;

    case 'k':
        yscale *= 0.5;
        break;

    case 'j':
        xscale *= 2;
        break;

    case 'l':
        xscale *= 0.5;
        break;

    case 'u':
        zscale *= 2;
        break;

    case 'o':
        zscale *= 0.5;
        break;

    case 27:
        exit(0);
        break;

    default: break;
    }

    drawScene();
}

// Main routine.
int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("box.cpp");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);

    glewExperimental = GL_TRUE;
    glewInit();

    setup();

    glutMainLoop();
}

