///////////////////////////////////
// square.cpp
//
// OpenGL program to draw a square.
//
// Sumanta Guha.
///////////////////////////////////

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

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

   glColor3f(0.0, 0.0, 0.0);

   glPolygonMode(GL_FRONT, GL_LINE);
   glPolygonMode(GL_BACK, GL_FILL);

   gluLookAt(0.0, 7.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); // for Question 3


  // Question 1
  //Is this a triangulation?
  //If yes, is this a consistently oriented triangulation?
/*
   glBegin(GL_TRIANGLES);
      glVertex3f(20.0, 80.0, 0.0);
      glVertex3f(20.0, 20.0, 0.0);
      glVertex3f(50.0, 80.0, 0.0);

      glVertex3f(50.0, 80.0, 0.0);
      glVertex3f(20.0, 20.0, 0.0);
      glVertex3f(50.0, 20.0, 0.0);

      glVertex3f(50.0, 20.0, 0.0);
      glVertex3f(50.0, 80.0, 0.0);
      glVertex3f(80.0, 80.0, 0.0);

      glVertex3f(80.0, 80.0, 0.0);
      glVertex3f(50.0, 20.0, 0.0);
      glVertex3f(80.0, 20.0, 0.0);
   glEnd();
*/

   // Question 2
   // Is this a triangulation?
   // if yes, is it consistently oriented? Need page 40
   /*
   glBegin(GL_TRIANGLE_STRIP);
       glVertex3f(20.0, 80.0, 0.0);
       glVertex3f(20.0, 20.0, 0.0);
       glVertex3f(50.0, 80.0, 0.0);
       glVertex3f(50.0, 20.0, 0.0);
       glVertex3f(80.0, 20.0, 0.0);
       glVertex3f(80.0, 80.0, 0.0);
   glEnd();
   */

   // Question 3
   //Change to   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);

   glBegin(GL_TRIANGLE_STRIP);
      glVertex3f(-5.0, 1.0, 5.0);
      glVertex3f(-5.0, -1.0, 5.0);
      glVertex3f(5.0, 1.0, 5.0);
      glVertex3f(5.0, -1.0, 5.0);
      glVertex3f(5.0, 1.0, -5.0);
      glVertex3f(5.0, -1.0, -5.0);
      glVertex3f(-5.0, 1.0, -5.0);
      glVertex3f(-5.0, -1.0, -5.0);
      glVertex3f(-5.0, 1.0, 5.0);
      glVertex3f(-5.0, -1.0, 5.0);
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
   //glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
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
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("square.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup();

   glutMainLoop();
}