////////////////////////////////////////////////////////
// squareShaderized.cpp
//
// Forward-compatible core GL 4.3 version of square.cpp.
//
// Sumanta Guha, modified by Cody Valle
////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>

#  include <GL/glew.h>
#  include <GL/freeglut.h>
#ifdef __APPLE__
#  include <OpenGL/glext.h>
#else
#  include <GL/glext.h>
#endif

using namespace std;

struct Vertex
{
   float coords[4]; // xyzw
   float colors[4]; // rgba
};

struct Matrix4x4
{
   float entries[16]; // pipe
};

static const Matrix4x4 IDENTITY_MATRIX4x4 =
{
   {
      1.0, 0.0, 0.0, 0.0,
      0.0, 1.0, 0.0, 0.0,
      0.0, 0.0, 1.0, 0.0,
      0.0, 0.0, 0.0, 1.0
   }
};

enum buffer {SQUARE_VERTICES};
enum object {SQUARE};

// Globals
static Vertex squareVertices[] =
{
   { { 20.0, 20.0, 0.0, 1.0 }, { 1.0, 0.0, 0.0, 1.0 } },
   { { 80.0, 20.0, 0.0, 1.0 }, { 0.0, 1.0, 0.0, 1.0 } },
   { { 20.0, 80.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0, 1.0 } },
   { { 80.0, 80.0, 0.0, 1.0 }, { 1.0, 1.0, 0.0, 1.0 } }
};

static Matrix4x4
   modelViewMat = IDENTITY_MATRIX4x4,
   projMat = IDENTITY_MATRIX4x4;

static unsigned int
   programId,
   vertexShaderId,
   fragmentShaderId,
   modelViewMatLoc,
   projMatLoc,
   buffer[1],
   vao[1];

// Function to read text file.
char* readTextFile(char* aTextFile)
{
   FILE* filePointer = fopen(aTextFile, "rb");
   char* content = NULL;
   long numVal = 0;

   fseek(filePointer, 0L, SEEK_END);
   numVal = ftell(filePointer);
   fseek(filePointer, 0L, SEEK_SET);
   content = (char*) malloc((numVal + 1) * sizeof(char));
   fread(content, 1, numVal, filePointer);
   content[numVal] = '\0';
   fclose(filePointer);
   return content;
}

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0);

   // Create shader program executable.
   // FIRST read the vertexShader.glsl file into the character string vertexShader
   char* vertexShader = readTextFile("E:\\School\\2016Fall\\Homework\\Graphics\\ValleSquareShaderized\\vertexShader.glsl");
   // SECOND create an empty vertex shader object returning the non-zero id vertexShaderID
   vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
   // THIRD sets the source code of the shader with id vertexShaderId to the value of the character string vertexShader
   //     which is only one-null-terminated string.  WHERE DOES THAT HAPPEN in readTextFile?
   glShaderSource(vertexShaderId, 1, (const char**) &vertexShader, NULL);
   // FOURTH  compile the source code for shader id vertexShaderId. At the end, we have a compiled vertex shader objecgt
   glCompileShader(vertexShaderId);

   free(vertexShader);

   // Similarly we do the same four steps for the fragment shader
   char* fragmentShader = readTextFile("E:\\School\\2016Fall\\Homework\\Graphics\\ValleSquareShaderized\\fragmentShader.glsl");
   fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragmentShaderId, 1, (const char**) &fragmentShader, NULL);
   glCompileShader(fragmentShaderId);

   free(fragmentShader);

   // now greate an empty shader program object returning a non-zero id programID
   programId = glCreateProgram();
   // attach the two compiled shader objects to the program object identified with programId to create an executable shader program
   glAttachShader(programId, vertexShaderId);
   glAttachShader(programId, fragmentShaderId);
   glLinkProgram(programId);
   glUseProgram(programId);
   ///////////////////////////////////////

   // Create VAO and VBO and associate data with vertex shader.
   glGenVertexArrays(1, vao);
   glGenBuffers(1, buffer);
   glBindVertexArray(vao[SQUARE]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[SQUARE_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(squareVertices[0]), 0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(squareVertices[0]), (GLvoid*)sizeof(squareVertices[0].coords));
   glEnableVertexAttribArray(1);
   ///////////////////////////////////////

   // Obtain projection matrix uniform location and set value.
   Matrix4x4 projMat =
   {
      {
         0.02, 0.0,  0.0, -1.0,
         0.0,  0.02, 0.0, -1.0,
         0.0,  0.0, -1.0,  0.0,
         0.0,  0.0,  0.0,  1.0
      }
   };

   //the right hand side returns the location of the shader uniform variable names projMat with the program object programID
   // into the variable projMatLoc
   projMatLoc = glGetUniformLocation(programId,"projMat");
   //the update the uniform at the location projMatLoc with the values in the 4th parameter
   glUniformMatrix4fv(projMatLoc, 1, GL_TRUE, projMat.entries);
   ///////////////////////////////////////

   // Obtain modelview matrix uniform location and set value.
   Matrix4x4 modelViewMat = IDENTITY_MATRIX4x4;
   modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat");
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_TRUE, modelViewMat.entries);
   ///////////////////////////////////////
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

   glFlush();
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

// Main routine.
int main(int argc, char* argv[])
{
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3);
   glutInitContextProfile(GLUT_CORE_PROFILE);
   glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("squareShaderized.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup();

   glutMainLoop();
}
