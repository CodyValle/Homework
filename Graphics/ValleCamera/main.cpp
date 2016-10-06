/////////////////////////////////
// box.cpp
//
// This program demonstrates moving the camera.
//
// Sumanta Guha.
//
// Cody Valle
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

static const GLfloat PI = 3.14159265;

static uint16_t angle = 0;

// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    glLoadIdentity();

    // Modeling transformations.
    //glTranslatef((GLfloat)0, (GLfloat)0, (GLfloat)-15);
    //glRotatef((GLfloat)45, (GLfloat)0, (GLfloat)0, (GLfloat)1);
    //glRotatef((GLfloat)180, (GLfloat)0, (GLfloat)0, (GLfloat)1);


    gluLookAt(sin(PI * angle / 180) * 10, (GLfloat)0, cos(PI * angle / 180) * 10,
              (GLfloat)0, (GLfloat)0, (GLfloat)0,
              (GLfloat)0, (GLfloat)1, (GLfloat)0);

    glutWireTeacup(5.0); // Teapcup.

    glFlush();
}

// Initialization routine.
void setup(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 'a':
        angle += 10;
        if (angle >= 360)
            angle -= 360;
        glutPostRedisplay();
        break;

    case 27:
        exit(0);
        break;

    default: break;
    }
}

// Main routine.
int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("ValleCamera");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);

    glewExperimental = GL_TRUE;
    glewInit();

    setup();

    glutMainLoop();
}

