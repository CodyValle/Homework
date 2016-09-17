///////////////////////////////////////////////////////////////////////////
// main.cpp
//
// This program draws a capital letter A
// This program draws a number 8
//
// Cody Valle, based off code by Samanta Guha and Dr. Y
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

// Vertex co-ordinate vectors.
static float vertices[] =
{
	-50, -50, 0,
	-30, -50, 0,
	-10, 50, 0,
	0, 30, 0,
	10, 50, 0,
	30, -50, 0,
	50, -50, 0
};

// Triangle strip vertex indices in order.
const unsigned stripIndices[] = {0, 1, 2, 3, 4, 5, 6};

// Vertex color vectors.
static float colors[] =
{
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	1.0, 0.0, 0.0
};

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw the figure
	glDrawElements(GL_TRIANGLE_STRIP, sizeof(stripIndices) / sizeof(unsigned), GL_UNSIGNED_INT, 0);

	glutSwapBuffers();
}

// Initialization routine.
void setup(void)
{
	const unsigned VERTICES = 0;
	const unsigned INDICES = 1;
	
	unsigned buffer[2]; // Array of buffer ids.
	
	glClearColor(1.0, 1.0, 1.0, 0.0);

	glGenBuffers(2, buffer); // Generate buffer ids.

	// Enable two vertex arrays: co-ordinates and color.
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	// Bind vertex buffer and reserve space.
	glBindBuffer(GL_ARRAY_BUFFER, buffer[VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);

	// Copy vertex coordinates data into first half of vertex buffer.
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

	// Copy vertex color data into second half of vertex buffer.
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

	// Bind and fill indices buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[INDICES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(stripIndices), stripIndices, GL_STATIC_DRAW);

	// Specify vertex and color pointers to the start of the respective data.
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glColorPointer(3, GL_FLOAT, 0, (GLvoid*)(sizeof(vertices)));

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

	glutInitContextVersion(3, 0); //4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("AlphaNum");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}

