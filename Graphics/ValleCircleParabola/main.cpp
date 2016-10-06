// File: main.cpp
// Authors: Cody Valle
// Description: Draws a primitive circle.

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib")
#endif

#include <cmath>

const double PI = 3.14159265359;
unsigned char numVertices = 15;

void drawParabola()
{
    double vertex_x = 0,
           vertex_y = 0,
           a = 0.25,
           left_end = -20,
           right_end = 20,
           x,
           y;

    glColor3f(1,0,1);
    glLineWidth(2);

    glBegin(GL_LINE_STRIP);
    for (unsigned char i = 0; i < numVertices; ++i)
    {
        x = vertex_x + ((right_end - vertex_x) / numVertices) * i;
        y = vertex_y + a * std::pow(x - vertex_x, 2);
        glVertex3d(x, y, 0);
    }

    glEnd();

    glBegin(GL_LINE_STRIP);
    for (unsigned char i = 0; i < numVertices; ++i)
    {
        x = vertex_x - ((vertex_x - left_end) / numVertices) * i;
        y = vertex_y + a * std::pow(x - vertex_x, 2);
        glVertex3d(x, y, 0);
    }

    glEnd();

}

void drawCircle()
{
    glColor3f(0, 1, 1);

    double radius = 50,
           center_x = 0,
           center_y = 0;

    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
        for (unsigned char i = 0; i < numVertices; ++i)
            glVertex3d(std::cos(PI * i / numVertices * 2) * radius + center_x,
                       std::sin(PI * i / numVertices * 2) * radius + center_y,
                       0);
    glEnd();
}

// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawCircle();

    drawParabola();

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
   glOrtho(-100.0, 100.0, -100.0, 100.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 27:
        exit(0);
        break;

    case '+':
        if (numVertices + 1 < std::pow(2, sizeof(numVertices) * 8)) ++numVertices;
        glutPostRedisplay();
        break;

    case '-':
        if (numVertices > 3) --numVertices;
        glutPostRedisplay();
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
   glutCreateWindow("Circular Parabola");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup();

   glutMainLoop();
}
