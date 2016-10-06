// File: main.cpp
// Author: Sumanta Guha
// Modified by: Cody Valle
// Description: Draws a simple square using OpenGL.

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
/*
// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

   glColor3f(0.0, 0.0, 0.0);

   // Draw a polygon with specified vertices.
   glBegin(GL_POLYGON);
      glVertex3f(20.0, 20.0, 0.0);
      glVertex3f(80.0, 20.0, 0.0);
      glVertex3f(80.0, 80.0, 0.0);
      glVertex3f(20.0, 80.0, 0.0);
   glEnd();

   glFlush();
}
*/

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

   glColor3f(0.0, 0.0, 0.0);
   glBegin(GL_POLYGON);
      glVertex3f(10.0, 10.0, -0.5);
      glVertex3f(80.0, 20.0, 0.5);
      glVertex3f(80.0, 80.0, 0.5);
      glVertex3f(20.0, 80.0, 0.5);
   glEnd();

   glColor3f(1.0, 0.0, 0.0);
   glBegin(GL_POLYGON);
      glVertex3f(90.0, 10.0, -0.5);
      glVertex3f(80.0, 80.0, 0.5);
      glVertex3f(20.0, 80.0, 0.5);
      glVertex3f(20.0, 20.0, 0.5);
   glEnd();

   glColor3f(0.0, 1.0, 0.0);
   glBegin(GL_POLYGON);
      glVertex3f(90.0, 90.0, -0.5);
      glVertex3f(20.0, 80.0, 0.5);
      glVertex3f(20.0, 20.0, 0.5);
      glVertex3f(80.0, 20.0, 0.5);
   glEnd();

   glColor3f(0.0, 0.0, 1.0);
   glBegin(GL_POLYGON);
      glVertex3f(10.0, 90.0, -0.5);
      glVertex3f(20.0, 20.0, 0.5);
      glVertex3f(20.0, 80.0, 0.5);
      glVertex3f(80.0, 20.0, 0.5);
   glEnd();

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
   glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
   /*
    * left = 0.0
    * right = 100.0
    * bottom = 0.0
    * top = 100.0
    * near = -1.0
    * far = 1.0
    *
    * Corners of the box are at:
    * (left, bottom, -near) = (0.0, 0.0, 1.0)
    * (right, bottom, -near) = (100.0, 0.0, 1.0)
    * (left, top, -near) = (0.0, 100.0, 1.0)
    * (right, top, -near) = (100.0, 100.0, 1.0)
    * (left, bottom, -far) = (0.0, 0.0, -1.0)
    * (right, bottom, -far) = (100.0, 0.0, -1.0)
    * (left, top, -far) = (0.0, 100.0, -1.0)
    * (right, top, -far) = (100.0, 100.0, -1.0)
    */
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
      default:
         break;
   }
}

// Main routine.
int main(int argc, char **argv)
{
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3);
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
   glutInitWindowSize(200, 200);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Black Square");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup();

   glutMainLoop();
}
