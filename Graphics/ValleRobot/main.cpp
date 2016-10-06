/////////////////////////////////
// main.cpp
//
// This program draws a wire box.
//
// Sumanta Guha, modified by Cody Valle.
/////////////////////////////////

#include <iostream>
#include <cmath>

#  include <GL/glew.h>
#  include <GL/freeglut.h>

#ifdef __APPLE__
#  include <OpenGL/glext.h>
#else
#  include <GL/glext.h>
#endif

using namespace std;

// Translate globals
float xoffset = 0;
float yoffset = 0;
float zoffset = -10;

const double PI = 3.14159265359;
static bool isAnimating = false;
static const int animationPeriod = 100;
float angle = 45;

void animate(int value)
{
    static bool goingUp = false;
    if (isAnimating)
        angle = float(value);
    glutPostRedisplay();

    if (value == -45)
        goingUp = true;
    else if (value == 45)
        goingUp = false;

    if (isAnimating)
        glutTimerFunc(animationPeriod, animate, goingUp ? ++value : --value);
}

// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0, 0, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(xoffset, yoffset, -10 + zoffset);

    // Robot
    glEnable(GL_DEPTH_TEST); // Enable depth testing.

    // Draw the head
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glScalef(1, 2, 1);
    glutSolidCube(5);
    glPopMatrix();

    // Draw the head
    glPushMatrix();
    glColor3f(0, 1, 1);
    glTranslatef(0, 7.5, 0);
    glutSolidSphere(2.5, 10, 12);
    glPopMatrix();

    // Draw the arms
    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslatef(5, 1.5, 0);
    glRotatef(-angle, 0, 0, 1);
    glScalef(2, 1, 1);
    glutSolidTeapot(2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-5, 1.5, 0);
    glRotatef(angle, 0, 0, 1);
    glScalef(-2, 1, 1);
    glutSolidTeapot(2);
    glPopMatrix();

    glDisable(GL_DEPTH_TEST); // Disable depth testing.

    glFlush();
}

// Initialization routine.
void setup(void)
{
    glClearColor(1, 1, 1, 0);
}

// OpenGL window reshape rouconst double PI = 3.14159265359;tine.
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-10, 10, -10, 10, 5, 20);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch(key)
    {
    case ' ':
        isAnimating = !isAnimating;
        animate(45);
        break;

    case 'z':
        ++zoffset;
        break;

    case 'a':
        --zoffset;
        break;

    case 'y':
        ++yoffset;
        break;

    case 'u':
        --yoffset;
        break;

    case 'x':
        ++xoffset;
        break;

    case 'c':
        --xoffset;
        break;

    case 27:
        exit(0);
        break;

    default: break;
    }

    glutPostRedisplay();
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

