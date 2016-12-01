#include <cstdlib>
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

// Function to read text file.
char* readTextFile(const char* aTextFile)
{
   FILE* filePointer = fopen(aTextFile, "rb");
   if (filePointer == NULL) return NULL;
   char* content = NULL;
   long numVal = 0;

   fseek(filePointer, 0L, SEEK_END);
   numVal = ftell(filePointer);
   fseek(filePointer, 0L, SEEK_SET);
   content = (char*) malloc((numVal+1) * sizeof(char));
   fread(content, 1, numVal, filePointer);
   content[numVal] = '\0';
   fclose(filePointer);
   return content;
}

// Function to initialize shaders.
int setShader(const char* shaderType, const char* shaderFile)
{
   int shaderId;
   char* shader = readTextFile(shaderFile);
   if (shader == NULL) return -1;

   if (strcmp(shaderType, "vertex") == 0) shaderId = glCreateShader(GL_VERTEX_SHADER);
   if (strcmp(shaderType, "tessControl") == 0) shaderId = glCreateShader(GL_TESS_CONTROL_SHADER);
   if (strcmp(shaderType, "tessEvaluation") == 0) shaderId = glCreateShader(GL_TESS_EVALUATION_SHADER);
   if (strcmp(shaderType, "geometry") == 0) shaderId = glCreateShader(GL_GEOMETRY_SHADER);
   if (strcmp(shaderType, "fragment") == 0) shaderId = glCreateShader(GL_FRAGMENT_SHADER);

   glShaderSource(shaderId, 1, (const char**) &shader, NULL);
   glCompileShader(shaderId);

   free(shader);

   return shaderId;
}

