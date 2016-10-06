// File: main.cpp
// Authors: Cody Valle, Erik Murrain
// Description: Draws a 3D Ring

#include <GL/glew.h>
#include <GL/freeglut.h>
#ifdef __APPLE__
#  include <OpenGL/glext.h>
#else
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib")
#endif

#include <cmath>

const double PI = 3.14159265359;
static unsigned char numVertices = 15;
static bool isWire = false;
static unsigned angle = 0;
static unsigned color = 0;
static const unsigned colors[] = {0,0,0,1,0,0,0,1,0,0,0,1,1,1,0,1,0,1,0,1,1};
static const unsigned total_colors = 7;

void drawRing(const double r, const double w, const double cx, const double cy, const double z = 0)
{
    glBegin(GL_TRIANGLE_STRIP);
    for (unsigned char i = 0; i <= numVertices; ++i)
    {
        double t = PI * i / numVertices * 2;
        double x = std::cos(t) * r;
        double y = std::sin(t) * r;
        glColor3f(colors[color * 3], colors[color * 3 + 1], colors[color * 3 + 2]);
        glVertex3d(x + cx, y + cy, z - w);
        glColor3f(colors[color * 3 + 3], colors[color * 3 + 4], colors[color * 3 + 5]);
        glVertex3d(x + cx, y + cy, z + w);
    }
    glEnd();
}

// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, isWire ? GL_LINE : GL_FILL);

    glEnable(GL_DEPTH_TEST); // Enable depth testing.
    drawRing(40, 10, std::cos(angle * PI / 180) * 70, std::sin(angle * PI / 180) * 70, -20);
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

    case 'q':
        angle = ++angle % 360;
        glutPostRedisplay();
        break;

    case 'c':
        color = ++color % (total_colors - 1);
        glutPostRedisplay();
        break;

    case 'w':
        isWire = !isWire;
        glutPostRedisplay();
        break;

    case 'g':
        if (numVertices + 1 < std::pow(2, sizeof(numVertices) * 8)) ++numVertices;
        glutPostRedisplay();
        break;

    case 'h':
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

   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize(720, 720);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Ring around the Z-Axis (press and hold q)");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup();

   glutMainLoop();
}
