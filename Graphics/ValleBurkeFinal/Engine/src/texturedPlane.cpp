#include "texturedPlane.h"

#include <cmath>
#include <iostream>

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

using namespace std;

unsigned TexturedPlane::textureID = 0;

TexturedPlane::TexturedPlane(const unsigned pid, std::string file) :
    Plane(pid),
    image(0),
    texture(0),
    myTextureID(textureID)
{
    glUseProgram(programId);

    TextureVertex tVertices[4];

    // Initialize plane
    fillPlaneVertexArray(tVertices);

    // ...and associate data with vertex shader.
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TextureVertex) * 4, tVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(TextureVertex), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextureVertex), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(1);

    // Load the image
    image = getbmp(file.c_str());

    // Create texture ids.
    glGenTextures(1, &texture);

    // Bind the image.
    switch (myTextureID)
    {
    case 0:
        glActiveTexture(GL_TEXTURE0);
        break;

    case 1:
        glActiveTexture(GL_TEXTURE1);
        break;

    default: break;
    }
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->sizeX, image->sizeY, 0,
                  GL_RGBA, GL_UNSIGNED_BYTE, image->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glUniform1i(glGetUniformLocation(programId, "Tex"), myTextureID);

    textureID++;
}

TexturedPlane::~TexturedPlane()
{
}

void TexturedPlane::draw(mat4 modelViewMat)
{
    glUseProgram(programId);

    glUniform1i(glGetUniformLocation(programId, "Tex"), myTextureID);

    // Update the modelViewMat
    glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, value_ptr(modelViewMat));

    // Draw textured plane
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
