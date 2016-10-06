///////////////////////////////////////////////////////////////////////////
// squareAnnulusVBO.cpp
//
// This program modifies squareAnnulus4.cpp to store the vertex and color
// arrays in one vertex buffer object (VBO) and the index array in another.
//
// Sumanta Guha with some changes by Dr. Y
///////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <iostream>

#  include <GL/glew.h>
#  include <GL/freeglut.h>

#ifdef __APPLE__
#  include <OpenGL/glext.h>
#else
#  include <GL/glext.h>
#endif

const double PI = 3.14159265359;
const int VERTICES = 0;
const int INDICES = 1;
const int DONUT = 0;
const int DISK = 1;

using namespace std;

// Begin globals.
// Vertex co-ordinate vectors.
static float donutVertices[] =
{
   30.0, 30.0, 0.0,
   10.0, 10.0, 0.0,
   70.0, 30.0, 0.0,
   90.0, 10.0, 0.0,
   70.0, 70.0, 0.0,
   90.0, 90.0, 0.0,
   30.0, 70.0, 0.0,
   10.0, 90.0, 0.0
};

// Vertex color vectors.
static float donutColors[] =
{
   0.0, 0.0, 0.0,
   1.0, 0.0, 0.0,
   0.0, 1.0, 0.0,
   0.0, 0.0, 1.0,
   1.0, 1.0, 0.0,
   1.0, 0.0, 1.0,
   0.0, 1.0, 1.0,
   1.0, 0.0, 0.0
};

const unsigned char NUMVERTICES = 28;
float diskVertices[(NUMVERTICES + 2) * 3];

// Vertex color vectors.
float diskColors[(NUMVERTICES + 2) * 3];

// Triangle strip vertex indices in order.
unsigned int stripIndices[] = {0, 1, 2, 3, 4, 5, 6, 7, 0, 1};

unsigned int vao[2];

unsigned int buffer[2]; // Array of buffer ids.
// End globals.

void fillDiskVertices(float* const arr, const unsigned numVertices, const double r, const double cx, const double cy, const double cz = 0)
{
    float* at = arr;
    *at++ = cx;
    *at++ = cy;
    *at++ = cz;
    for (unsigned i = 0; i <= numVertices; ++i)
    {
        double t = PI * i / numVertices * 2;
        *at++ = std::cos(t) * r + cx;
        *at++ = std::sin(t) * r + cy;
        *at++ = cz;
    }
}

void fillDiskColors(float* const arr, const unsigned numVertices)
{
    float* at = arr;
    *at++ = 1;
    *at++ = 0;
    *at++ = 0;
    for (unsigned i = 0; i <= numVertices; ++i)
    {
        *at++ = 0;
        *at++ = i % 2 == 0 ? 0 : 1;
        *at++ = i % 2 == 1 ? 0 : 1;
    }
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

     // Draw square donut.
   glBindVertexArray(vao[DONUT]);
   glDrawElements(GL_TRIANGLE_STRIP, 10, GL_UNSIGNED_INT, 0);

     // Draw square donut.
   glBindVertexArray(vao[DISK]);
   glDrawArrays(GL_TRIANGLE_FAN, 0, NUMVERTICES + 2);

   glutSwapBuffers();
}

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0);

   fillDiskVertices(diskVertices, NUMVERTICES, 15, 50, 50);
   fillDiskColors(diskColors, NUMVERTICES);

   glGenVertexArrays(2, vao);
   glBindVertexArray(vao[DONUT]);

   // Draw donut
   glGenBuffers(2, buffer); // Generate buffer ids.

   // Enable two vertex arrays: co-ordinates and color.
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_COLOR_ARRAY);

      // Bind vertex buffer and reserve space.
   glBindBuffer(GL_ARRAY_BUFFER, buffer[VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(donutVertices) + sizeof(donutColors), NULL, GL_STATIC_DRAW);

   // Copy vertex coordinates data into first half of vertex buffer.
   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(donutVertices), donutVertices);

   // Copy vertex color data into second half of vertex buffer.
   glBufferSubData(GL_ARRAY_BUFFER, sizeof(donutVertices), sizeof(donutColors), donutColors);

   // Bind and fill indices buffer.
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[INDICES]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(stripIndices), stripIndices, GL_STATIC_DRAW);

   // Specify vertex and diskColorscolor pointers to the start of the respective data.
   glVertexPointer(3, GL_FLOAT, 0, 0);
   glColorPointer(3, GL_FLOAT, 0, (GLvoid*)(sizeof(donutVertices)));



   // Draw disk
   glBindVertexArray(vao[DISK]);

   glGenBuffers(1, buffer); // Generate buffer ids.

   // Enable two vertex arrays: co-ordinates and color.
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_COLOR_ARRAY);

      // Bind vertex buffer and reserve space.
   glBindBuffer(GL_ARRAY_BUFFER, buffer[VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(diskVertices) + sizeof(diskColors), NULL, GL_STATIC_DRAW);

   // Copy vertex coordinates data into first half of vertex buffer.
   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(diskVertices), diskVertices);

   // Copy vertex color data into second half of vertex buffer.
   glBufferSubData(GL_ARRAY_BUFFER, sizeof(diskVertices), sizeof(diskColors), diskColors);

   // Specify vertex and color pointers to the start of the respective data.
   glVertexPointer(3, GL_FLOAT, 0, 0);
   glColorPointer(3, GL_FLOAT, 0, (GLvoid*)(sizeof(diskVertices)));
 }

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
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

   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Still a Square Donut");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup();

   glutMainLoop();
}

