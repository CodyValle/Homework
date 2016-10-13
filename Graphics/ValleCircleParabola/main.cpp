// File: main.cpp
// Authors: Cody Valle
// Description: Draws a primitive circle.

#include <GL/glew.h>
#include <GL/freeglut.h>
#ifdef __APPLE__
#  include <OpenGL/glext.h>
#else
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib")
#endif

#include <cmath>
#include <iostream>

const double PI = 3.14159265359;
static unsigned char numVertices = 15;
static bool isWire = false;

void drawParabola(const double cx, const double cy)
{
    double a = 0.25,
           left_end = -20,
           right_end = 20,
           x, y;

    glColor3f(1,0,1);
    glLineWidth(2);

    glBegin(GL_LINE_STRIP);
    for (unsigned char i = 0; i < numVertices; ++i)
    {
        x = cx + ((right_end - cx) / numVertices) * i;
        y = cy + a * std::pow(x - cx, 2);
        glVertex3d(x, y, 0);
    }

    glEnd();

    glBegin(GL_LINE_STRIP);
    for (unsigned char i = 0; i < numVertices; ++i)
    {
        x = cx - ((cx - left_end) / numVertices) * i;
        y = cy + a * std::pow(x - cx, 2);
        glVertex3d(x, y, 0);
    }

    glEnd();

}

void drawCircle(const double r, const double cx, const double cy)
{
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
        for (unsigned char i = 0; i < numVertices; ++i)
        {
            double t = PI * i / numVertices * 2;
            glVertex3d(std::cos(t) * r + cx,
                       std::sin(t) * r + cy,
                       0);
        }
    glEnd();
}

void drawDisk(const double cx, const double cy, const double z, const double r)
{
    glBegin(GL_TRIANGLE_FAN);
        glVertex3d(cx, cy, z);
        for (unsigned char i = 0; i <= numVertices; ++i)
        {
            double t = PI * i / numVertices * 2;
            glVertex3d(std::cos(t) * r + cx,
                       std::sin(t) * r + cy,
                       z);
        }
    glEnd();
}

void drawTorus(const double r, const double w, const double cx, const double cy, const double z = 0)
{
    glBegin(GL_TRIANGLE_STRIP);
    for (unsigned char i = 0; i <= numVertices; ++i)
    {
        double t = PI * i / numVertices * 2;
        glVertex3d(std::cos(t) * (r + w) + cx,
                   std::sin(t) * (r + w) + cy,
                   z);
        glVertex3d(std::cos(t) * (r - w) + cx,
                   std::sin(t) * (r - w) + cy,
                   z);
    }
    glEnd();
}

void drawRing(const double r, const double w, const double cx, const double cy, const double z = 0)
{
    glBegin(GL_TRIANGLE_STRIP);
    for (unsigned char i = 0; i <= numVertices; ++i)
    {
        double t = PI * i / numVertices * 2;
        double x = std::cos(t) * r;
        double y = std::sin(t) * r;
        glColor3f(1,0,0);
        glVertex3d(x + cx, y + cy, z - w);
        glColor3f(0,0,1);
        glVertex3d(x + cx, y + cy, z + w);
    }
    glEnd();
}

void drawCone(const double h, const double r, const double cx, const double cy, const double cz = 0)
{
    glBegin(GL_LINE_STRIP);
        glColor3f(0,0,1);
        //glVertex3d(cx, cy, cz);
        glColor3f(1,0,0);
    for (unsigned char i = 0; i <= numVertices; ++i)
    {
        double t = PI * i / numVertices * 2;
        double x = std::cos(t) * r;
        double z = std::sin(t) * r;
        glVertex3d(x, h, z + cz);
    }
    glEnd();
}


// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, isWire ? GL_LINE : GL_FILL);

    glColor3f(1, 0, 0);
    /*drawDisk(0, 0, 1, 50);

    glColor3f(1, 1, 1);
    drawDisk(0, 0, 1, 30);*/


    //drawTorus(40, 0.4, 0, 0);

    static unsigned angle = 0;
    glEnable(GL_DEPTH_TEST); // Enable depth testing.
    //drawRing(40, 10, std::cos(angle * PI / 180) * 70, std::sin(angle * PI / 180) * 70, -20);
    drawCone(50, 20, std::cos(angle * PI / 180) * 70, std::sin(angle * PI / 180) * 70, -50);
    angle = ++angle % 360;
    glDisable(GL_DEPTH_TEST);
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
   glFrustum(-100.0, 100.0, -100.0, 100.0, 5, 100);
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

    case ' ':
        isWire = !isWire;
        glutPostRedisplay();
        break;

    case 'r':
        glutPostRedisplay();
        break;


    case GLUT_KEY_UP:
        break;

    case GLUT_KEY_DOWN:
        break;

    case GLUT_KEY_LEFT:
        break;

    case GLUT_KEY_RIGHT:
        break;

    case '+':
        if (numVertices + 1 < std::pow(2, sizeof(numVertices) * 8)) ++numVertices;
        glutPostRedisplay();
        break;

    case '-':
        if (numVertices > 3) --numVertices;
        glutPostRedisplay();
        break;

    case '2':
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

   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
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
