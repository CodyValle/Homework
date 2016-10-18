////////////////////////////////////////////////////////////////////////////////////
// cylinder.cpp
//
// A cylinder is approximated using a triangular mesh whose vertices are the images
// of a grid of sample points in parameter space.  chapter 10
//
// Interaction:
// Press left/right arrow keys to increase/decrease the number of grid columns.
// Press up/down arrow keys to increase/decrease the number of grid rows.
// Press x, X, y, Y, z, Z to turn the cylinder.
//
// Sumanta Guha.
////////////////////////////////////////////////////////////////////////////////////

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
#endif

const double PI = 3.14159265358979324;

using namespace std;

// Globals.
static int p = 6; // Number of grid columns for [-pi,pi]
static int q = 4; // Number of grid rows for [-1,1]
static float *vertices = NULL; // Vertex array of the mapped sample on the cylinder.
static float Xangle = 150.0, Yangle = 60.0, Zangle = 0.0; // Angles to rotate the cylinder.

// Functions to map the grid vertex (u_i,v_j) to the mesh vertex (f(u_i,v_j), g(u_i,v_j), h(u_i,v_j)) on the cylinder.
float f(int i, int j)
{
   return ( cos(PI * (-1.0 + (2.0 * (float)i/p))));
}

float g(int i, int j)
{
   return ( sin(PI * (-1.0 + (2.0 * (float)i/p) ) ));
}

float h(int i, int j)
{
   return ( -1.0 + ((float)j*2.0/q));
}

// Routine to fill the vertex array with co-ordinates of the mapped sample points.
void fillVertexArray(void)
{
   int i, j, k;

   k = 0;
   for (j = 0; j <= q; j++)
      for (i = 0; i <= p; i++)
      {
         vertices[k++] = f(i,j);
         vertices[k++] = g(i,j);
         vertices[k++] = h(i,j);
      }
}

// Initialization routine.
void setup(void)
{
   glEnableClientState(GL_VERTEX_ARRAY);  //Find in class document for Sept 13

   glClearColor(1.0, 1.0, 1.0, 0.0);
}

// Drawing routine.
void drawScene(void)
{
   int  i, j;
   if(vertices != NULL)delete [] vertices;
   vertices = new float[3 * (p+1) * (q + 1)]; // Dynamic array allocation with new value of p and q.

   glVertexPointer(3, GL_FLOAT, 0, vertices); //Find in class document for sept 13
   glClear(GL_COLOR_BUFFER_BIT);

   glLoadIdentity();
   gluLookAt (0.0, 0.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   glColor3f(0.0, 0.0, 0.0);

   // Rotate scene.
   glRotatef(Zangle, 0.0, 0.0, 1.0);
   glRotatef(Yangle, 0.0, 1.0, 0.0);
   glRotatef(Xangle, 1.0, 0.0, 0.0);

   // Fill the vertex array.
   fillVertexArray();

   // Make the approximating triangular mesh.
   for(j = 0; j < q; j++)
   {
      glBegin(GL_TRIANGLE_STRIP);
         for(i = 0; i <= p; i++)
         {
            glArrayElement( (j+1) * (p + 1) + i );
            glArrayElement(  j    * (p + 1) + i );
         }
      glEnd();
   }

   glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0, (float)w / (float)h, 1.0, 50.0);
   glMatrixMode(GL_MODELVIEW);

}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key)
   {
      case 27:
         exit(0);
         break;
      case 'x':
         Xangle += 5.0;
         if (Xangle > 360.0) Xangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'X':
         Xangle -= 5.0;
         if (Xangle < 0.0) Xangle += 360.0;
         glutPostRedisplay();
         break;
      case 'y':
         Yangle += 5.0;
         if (Yangle > 360.0) Yangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'Y':
         Yangle -= 5.0;
         if (Yangle < 0.0) Yangle += 360.0;
         glutPostRedisplay();
         break;
      case 'z':
         Zangle += 5.0;
         if (Zangle > 360.0) Zangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'Z':
         Zangle -= 5.0;
         if (Zangle < 0.0) Zangle += 360.0;
         glutPostRedisplay();
         break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_LEFT) if (p > 3) p -= 1;
   if (key == GLUT_KEY_RIGHT) p += 1;
   if (key == GLUT_KEY_DOWN) if (q > 3) q -= 1;
   if (key == GLUT_KEY_UP) q += 1;

   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press left/right arrow keys to increase/decrease the number of grid columns." << endl
        << "Press up/down arrow keys to increase/decrease the number of grid rows." << endl
        << "Press x, X, y, Y, z, Z to turn the cylinder." << endl;
}

// Main routine.
int main(int argc, char **argv)
{
   printInteraction();
   glutInit(&argc, argv);

   glutInitContextVersion(3, 3);
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Cylinder");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup();

   glutMainLoop();
}
