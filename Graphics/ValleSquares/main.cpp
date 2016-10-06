///////////////////////////////////
// square.cpp
//
// OpenGL program to draw a square.
//
// Sumanta Guha.
// Revised by: Cody Valle
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

   // Draw a red square with specified vertices.
   glColor3f(1, 0, 0);
   glBegin(GL_POLYGON);
      glVertex3f(20, 20, 0);
      glVertex3f(80, 20, 0);
      glVertex3f(80, 80, 0);
      glVertex3f(20, 80, 0);
   glEnd();

   // Draw a blue square with specified vertices.
   glColor3f(0, 0, 1);
   glBegin(GL_POLYGON);
      glVertex3f(50, 25, 0);
      glVertex3f(75, 50, 0);
      glVertex3f(50, 75, 0);
      glVertex3f(25, 50, 0);
   glEnd();

   // Draw a colorful triangle with specified vertices.
   glBegin(GL_POLYGON);
      glColor3f(1, 0, 0); // Red
      glVertex3f(25, 100, 0);
      glColor3f(0, 1, 0); // Green
      glVertex3f(175, 100, 0);
      glColor3f(0, 0, 1); // Blue
      glVertex3f(100, 200, 0);
   glEnd();

   // Draw a couple magenta points
    glPointSize(6);
    glBegin(GL_POINTS);
      glColor3f(1, 0, 1);
      glVertex3f(140, 20, 0);
      glVertex3f(160, 20, 0);
    glEnd();

   // Draw a cyan line
   glLineWidth(6);
    glBegin(GL_LINE_STRIP);
      glColor3f(0, 1, 1);
      glVertex3f(120, 40, 0);
      glVertex3f(140, 60, 0);
      glVertex3f(160, 60, 0);
      glVertex3f(180, 40, 0);
    glEnd();

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
   glOrtho(0, 200, 0, 200, -1, 1);
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
   glutCreateWindow("Shapes, Gradients, Points, and Lines");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup();

   glutMainLoop();
}
