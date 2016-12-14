/////////////////////////////////////////////////////////////////////////////////
// fieldAndSkyFilteredShaderized.cpp
//
// Forward-compatible core GL 4.3 version of fieldAndSkyFiltered.cpp (except only
// one filter is implemented).

// Comparing different filtering techniques
//
// Interaction:
// Press the up and down arrow keys to move the viewpoint over the field.
//
// Sumanta Guha
//
// Texture Credits: See ExperimenterSource/Textures/TEXTURE_CREDITS.txt
/////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <iostream>
#include <fstream>

#  include <GL/glew.h>
#  include <GL/freeglut.h>
#ifdef __APPLE__
#  include <OpenGL/glext.h>
#else
#  include <GL/glext.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "shader.h"
#include "getbmp.h"
#include "vertex.h"

using namespace std;
using namespace glm;

enum object {FIELD, SKY}; // VAO ids.
enum buffer {FIELD_VERTICES, SKY_VERTICES}; // VBO ids.

static Vertex skyVertices[4] =
{
	{vec4(0.5, -0.5, 0.0, 1.0), vec2(1.0, 0.0)},
	{vec4(0.5, 0.5, .0, 1.0), vec2(1.0, 1.0)},
	{vec4(-0.5, -0.5, 0.0, 1.0), vec2(0.0, 0.0)},
	{vec4(-0.5, 0.5, 0.0, 1.0), vec2(0.0, 1.0)}
	/*{vec4(100.0, 0.0, -70.0, 1.0), vec2(1.0, 0.0)},
	{vec4(100.0, 120.0, -70.0, 1.0), vec2(1.0, 1.0)},
	{vec4(-100.0, 0.0, -70.0, 1.0), vec2(0.0, 0.0)},
	{vec4(-100.0, 120.0, -70.0, 1.0), vec2(0.0, 1.0)}*/
};

static mat4 modelViewMat = mat4(1.0);
static mat4 projMat = mat4(1.0);

static unsigned int
   programId,
   programId2,
   vertexShaderId,
   fragmentShaderId,
   matrixLoc,
   projMatLoc,
   skyTexLoc,
   objectLoc,
   buffer,
   vao,
   texture;

static BitMapFile *image; // Local storage for bmp image data.

static float d = 0.0; // Distance parameter in gluLookAt().

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0);
   glEnable(GL_DEPTH_TEST);

   // Create shader program executable.
   programId = glCreateProgram();
   glAttachShader(programId, setShader("vertex", "vertexShader.glsl"));
   glAttachShader(programId, setShader("fragment", "fragmentShader.glsl"));
   glLinkProgram(programId);

   // Obtain projection matrix uniform location and set value.
   projMat = frustum(-5.0, 5.0, -5.0, 5.0, 5.0, 300.0);
   glUniformMatrix4fv(glGetUniformLocation(programId,"projMat"), 1, GL_FALSE, value_ptr(projMat));

   // Obtain modelview matrix uniform and object uniform locations.
   matrixLoc = glGetUniformLocation(programId,"modelViewMat");


   /**/
   // Create VAOs and VBOs...
   glGenVertexArrays(1, &vao);
   glGenBuffers(1, &buffer);

   // ...and associate data with vertex shader.
   glBindVertexArray(vao);
   glBindBuffer(GL_ARRAY_BUFFER, buffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(skyVertices), skyVertices, GL_STATIC_DRAW);

   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));
   glEnableVertexAttribArray(1);

   // Load the images.
   image = getbmp("MechaYerion.bmp");

   // Create texture ids.
   glGenTextures(1, &texture);

   // Bind sky image.
   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, texture);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->sizeX, image->sizeY, 0,
	            GL_RGBA, GL_UNSIGNED_BYTE, image->data);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glGenerateMipmap(GL_TEXTURE_2D);
   glUniform1i(glGetUniformLocation(programId, "Tex"), 1);
   /**/
}

// Drawing routine.
void drawScene(void)
{
   glUseProgram(programId);// Create shader program executable.
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Calculate and update modelview matrix.
   modelViewMat = mat4(1.0);
   //modelViewMat = lookAt(vec3(0.0, 10.0, 15.0 + d), vec3(0.0, 10.0, -70.0), vec3(0.0, 1.0, 0.0));
   modelViewMat = translate(modelViewMat, vec3(0,0,-50));
   //modelViewMat = rotate(modelViewMat, float(180), vec3(0,1,0));
   modelViewMat = scale(modelViewMat, vec3(30, 30, 30));

   /**/
   glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, value_ptr(modelViewMat));

   // Draw sky.
   glBindVertexArray(vao);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   /**/


   glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
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

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_UP)
   {
      if (d > -69.5) d -= 0.5;
   }
   if (key == GLUT_KEY_DOWN)
   {
      if (d < 14.5) d += 0.5;
   }
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the up and down arrow keys to move the viewpoint over the field." << endl;
}

// Main routine.
int main(int argc, char **argv)
{
   printInteraction();
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3);
   glutInitContextProfile(GLUT_CORE_PROFILE);
   glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("a field and a sky");
   glutFullScreen();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup();

   glutMainLoop();
}

