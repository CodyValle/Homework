//////////////////////////////////////////////////////
// main.cpp
//
// This program draws a wireframe vase.
//
//
// Sumanta Guha, modified by Cody Valle and Andrew Burke.
//////////////////////////////////////////////////////

#include <iostream>
#include <cmath>

#  include <GL/glew.h>
#  include <GL/freeglut.h>
#ifdef __APPLE__
#  include <OpenGL/glext.h>
#else
#  include <GL/glext.h>
#endif


const double PI = 3.14159265358979324;

using namespace std;

// Globals.
static int p = 10; // Number of grid columns for [-pi,pi]
static int q = 10; // Number of grid rows for [-1,1]
static float *vertices = nullptr; // Vertex array of the mapped sample on the cylinder.
static float Xangle = 90.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate the cylinder.

// Expects a value on [0,q].
float calculateRadius(int j)
{
    float radius = cos(PI * (7. / 4 * j / q + 5. / 6)); // Map j onto 5PI/6 thru 7PI/4
    radius += 2; // Offset for a minimum radius of 1.
    radius /= 5.5; // Scaling
    return radius;
}

// Functions to map the grid vertex (u_i,v_j) to the mesh vertex (f(u_i,v_j), g(u_i,v_j), h(u_i,v_j)) on the cylinder.
float f(int i, int j)
{
    return cos(PI * (2. * i / p - 1)) * calculateRadius(j);
}

float g(int i, int j)
{
    return sin(PI * (2. * i / p - 1)) * calculateRadius(j);
}

float h(int i, int j)
{
    return 2. * j / q - 1;
}

// Routine to fill the vertex array with co-ordinates of the mapped sample points.
void fillVertexArray(void)
{
   float* k = vertices;

   for (int j = 0; j <= q; ++j)
      for (int i = 0; i <= p; ++i)
      {
         *(k++) = f(i,j);
         *(k++) = g(i,j);
         *(k++) = h(i,j);
      }
}

// Initialization routine.
void setup(void)
{
   glEnableClientState(GL_VERTEX_ARRAY);

   glClearColor(1.0, 1.0, 1.0, 0.0);
}

// Drawing routine.
void drawScene(void)
{
   delete[] vertices; // Free memory!!

   vertices = new float[(p * q + p + q + 1) * 3]; // Dynamic array allocation with new value of p and q.

   glVertexPointer(3, GL_FLOAT, 0, vertices);
   glClear(GL_COLOR_BUFFER_BIT);

   glLoadIdentity();
   gluLookAt (0., 0., 3., 0., 0., 0., 0., 1., 0.);

   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   glColor3f(0.0, 0.0, 0.0);

   // Rotate scene.
   glRotatef(Zangle, 0.0, 0.0, 1.0);
   glRotatef(Yangle, 0.0, 1.0, 0.0);
   glRotatef(Xangle, 1.0, 0.0, 0.0);

   // Fill the vertex array.
   fillVertexArray();

   // Make the approximating triangular mesh.
   for (int j = 0; j < q; j++)
   {
      glBegin(GL_TRIANGLE_STRIP);
         for (int i = 0; i <= p; i++)
         {
            glArrayElement((j + 1) * (p + 1) + i);
            glArrayElement(   j    * (p + 1) + i);
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
   gluPerspective(60.0, float(w) / h, 1.0, 50.0);
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
        Xangle = fmod(Xangle + 5.0, 360.);
        break;

    case 'X':
        Xangle = fmod(Xangle - 5.0, 360.);
        break;

    case 'y':
        Yangle = fmod(Yangle + 5.0, 360.);
        break;

    case 'Y':
        Yangle = fmod(Yangle - 5.0, 360.);
        break;

    case 'z':
        Zangle = fmod(Zangle + 5.0, 360.);
        break;

    case 'Z':
        Zangle = fmod(Zangle - 5.0, 360.);
        break;

    default: break;
    }
    
    glutPostRedisplay();
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
   cout << "Interaction:" << endl
        << "Press left/right arrow keys to increase/decrease the number of grid columns." << endl
        << "Press up/down arrow keys to increase/decrease the number of grid rows." << endl
        << "Press x, X, y, Y, z, Z to turn the vase." << endl;
}

// Main routine.
int main(int argc, char **argv)
{
    printInteraction();
    glutInit(&argc, argv);

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("One Beautiful Vase");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

    glewExperimental = GL_TRUE;
    glewInit();

    setup();

    glutMainLoop();
}
