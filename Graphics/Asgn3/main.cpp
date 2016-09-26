///////////////////////////////////////////////////////////////////////////
// main.cpp
//
// This program draws a capital letter A
//
// Cody Valle and Andrew Burke, based off code by Samanta Guha and Dr. Y
///////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#ifdef __APPLE__
#  include <OpenGL/glext.h>
#else
#  include <GL/glext.h>
#endif

// Objects to draw
unsigned int vao[2];
const unsigned BAR = 0;
const unsigned MAIN = 1;

// Vertex co-ordinate vectors.
float mainVertices[21];

float barVertices[12];

// Triangle strip vertex indices in order.
const unsigned mainIndices[] = {0, 1, 2, 3, 4, 5, 6};
const unsigned barIndices[] = {0, 1, 2, 3};

// Vertex color vectors.
float mainColors[] =
{
	1, 0, 0, // Red 2
	0, 1, 0, // Green 3
	0, 1, 0, // Green 3
	0, 0, 1, // Blue 4
	0, 1, 0, // Green 3
	0, 1, 0, // Green 3
	1, 0, 0  // Red 2
};

float barColors[] =
{
    0, 0, 0, // Black 0
    1, 1, 1, // White 1
    0, 0, 0, // Black 0
    1, 1, 1  // White 1
};

void fillMainVertices(float* const arr, const int x, const int y, const int z = 0)
{
    float* at = arr;
    *at++ = -50 + x; *at++ = -50 + y; *at++ = z;
    *at++ = -30 + x; *at++ = -50 + y; *at++ = z;
    *at++ = -10 + x; *at++ = 50 + y; *at++ = z;
    *at++ = 0 + x; *at++ = 30 + y; *at++ = z;
    *at++ = 10 + x; *at++ = 50 + y; *at++ = z;
    *at++ = 30 + x; *at++ = -50 + y; *at++ = z;
    *at++ = 50 + x; *at++ = -50 + y; *at++ = z;
}
void fillBarVertices(float* const arr, const int x, const int y, const int z = 0)
{
    float* at = arr;
    *at++ = 20 + x; *at++ = -15 + y; *at++ = z;
    *at++ = 20 + x; *at++ = 0 + y; *at++ = z;
    *at++ = -20 + x; *at++ = -15 + y; *at++ = z;
    *at++ = -20 + x; *at++ = 0 + y; *at++ = z;
}

// Drawing routine.
void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw the figure
    glBindVertexArray(vao[BAR]);
	glDrawElements(GL_TRIANGLE_STRIP, sizeof(mainIndices) / sizeof(unsigned), GL_UNSIGNED_INT, 0);
    glBindVertexArray(vao[MAIN]);
	glDrawElements(GL_TRIANGLE_STRIP, sizeof(mainIndices) / sizeof(unsigned), GL_UNSIGNED_INT, 0);

	glutSwapBuffers();
}

// Initialization routine.
void setup()
{
	const unsigned VERTICES = 0;
	const unsigned INDICES = 1;

	unsigned buffer[2]; // Array of buffer ids.

	glClearColor(1, 1, 1, 0);

	// Fill buffers with vertex data
	fillMainVertices(mainVertices, 0, 0);
	fillBarVertices(barVertices, 0, 0);

	// Generate two vertex arrays
    glGenVertexArrays(2, vao);

    /// Prepare the main
    glBindVertexArray(vao[MAIN]);
	glGenBuffers(2, buffer); // Generate buffer ids.

	// Enable two vertex arrays: co-ordinates and color.
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	// Bind vertex buffer and reserve space.
	glBindBuffer(GL_ARRAY_BUFFER, buffer[VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mainVertices) + sizeof(mainColors), NULL, GL_STATIC_DRAW);

	// Copy vertex coordinates data into first half of vertex buffer.
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(mainVertices), mainVertices);

	// Copy vertex color data into second half of vertex buffer.
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(mainVertices), sizeof(mainColors), mainColors);

	// Bind and fill indices buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[INDICES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mainIndices), mainIndices, GL_STATIC_DRAW);

	// Specify vertex and color pointers to the start of the respective data.
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glColorPointer(3, GL_FLOAT, 0, (GLvoid*)(sizeof(mainVertices)));

	/// Prepare the bar
    glBindVertexArray(vao[BAR]);
	glGenBuffers(2, buffer); // Generate buffer ids.

	// Enable two vertex arrays: co-ordinates and color.
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	// Bind vertex buffer and reserve space.
	glBindBuffer(GL_ARRAY_BUFFER, buffer[VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(barVertices) + sizeof(barColors), NULL, GL_STATIC_DRAW);

	// Copy vertex coordinates data into first half of vertex buffer.
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(barVertices), barVertices);

	// Copy vertex color data into second half of vertex buffer.
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(barVertices), sizeof(barColors), barColors);

	// Bind and fill indices buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[INDICES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(barIndices), barIndices, GL_STATIC_DRAW);

	// Specify vertex and color pointers to the start of the respective data.
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glColorPointer(3, GL_FLOAT, 0, (GLvoid*)(sizeof(barVertices)));
 }

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-100, 100, -100, 100, -1, 1);
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

	default: break;
	}
}

// Main routine.
int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(720, 720);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Grand Letter A");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}

