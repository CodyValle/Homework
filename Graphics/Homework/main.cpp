///////////////////////////////////////////////////////////////////////////////////////////
// circularAnnuluses.cpp
//
// This program draws three identical-looking circular annuluses in three different ways -
// see comments below.
//
// Interaction:
// Press the space bar to toggle between wirefrime and filled for the lower annulus.
//
// Sumanta Guha.
///////////////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <cmath>
#include <iostream>

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

const double PI = 3.14159265;
const int N = 8; // Number of vertices on the boundary of the disc.

using namespace std;

// Globals.
static bool isWire = false; // Is wireframe?
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Function to draw a disc with center at (X, Y, Z), radius R, parallel to the xy-plane.
void drawDisc(float R, float X, float Y, float Z)
{
   float t;
   int i;

   glBegin(GL_TRIANGLE_FAN);
      glVertex3f( X, Y, Z);
      for(i = 0; i <= N; ++i)
	  {
         t = 2 * PI * i / N;
         glVertex3f(X + cos(t) * R, Y + sin(t) * R, Z);
	  }
   glEnd();
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the buffers including
                                                       // the depth buffer.

   glPolygonMode(GL_FRONT, GL_FILL);

   glColor3f(1, 0, 0);

   glRotatef(180.0, 1.0, 0.0, 0.0);
    glutWireTeapot(1.0);
    glRotatef(90.0, 0.0, 0.0, 1.0);
    glTranslatef(4.0, 0.0, 0.0);
    glutWireSphere(2.0, 10, 8);

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
   glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key)
   {
      case ' ':
         isWire = !isWire;
         glutPostRedisplay();
         break;
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the space bar to toggle between wirefrime and filled for the lower annulus." << endl;
}

// Main routine.
int main(int argc, char **argv)
{
   printInteraction();
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3);
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("circularAnnuluses.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup();

   glutMainLoop();
}
