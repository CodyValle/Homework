#include "drawable.h"

#include <GL/glew.h>

using namespace glm;

Drawable::Drawable(const unsigned pid) :
    vertices('\0'),
    indices('\0'),
    counts('\0'),
    offsets('\0'),
    programId(pid),
    vao(0),
    vbo_vertices(0),
    vbo_indices(0),
    matrixLoc(0)
{
    objectColor[0] = 0.;
    objectColor[1] = 0.;
    objectColor[2] = 0.;
    objectColor[3] = 1.;

    // Create VAOs and VBOs...
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo_indices);
    glGenBuffers(1, &vbo_vertices);

    // Obtain modelview matrix uniform and object uniform locations.
    matrixLoc = glGetUniformLocation(programId, "modelViewMat");
    normalMatLoc = glGetUniformLocation(programId, "normalMat");

    // Front material properties.
    objFront.ambRefl  = vec4(0.0, 0.0, 0.0, 1.0);
    objFront.difRefl  = vec4(1.0, 1.0, 1.0, 1.0);
    objFront.specRefl = vec4(1.0, 1.0, 1.0, 1.0);
    objFront.emitCols = vec4(0.0, 0.0, 0.0, 1.0);
    objFront.shininess = 50.0;
}

Drawable::~Drawable()
{
    delete[] vertices;
    delete[] indices;
    delete[] counts;
    delete[] offsets;
}

void Drawable::setAmbient(const float r, const float g, const float b)
{
    objFront.ambRefl[0] = r;
    objFront.ambRefl[1] = g;
    objFront.ambRefl[2] = b;
}

void Drawable::setDiffuse(const float r, const float g, const float b)
{
    objFront.difRefl[0] = r;
    objFront.difRefl[1] = g;
    objFront.difRefl[2] = b;
}

void Drawable::setSpecular(const float r, const float g, const float b)
{
    objFront.specRefl[0] = r;
    objFront.specRefl[1] = g;
    objFront.specRefl[2] = b;
}

void Drawable::setEmittance(const float r, const float g, const float b)
{
    objFront.emitCols[0] = r;
    objFront.emitCols[1] = g;
    objFront.emitCols[2] = b;
}

void Drawable::setShininess(const float shiny)
{
    objFront.shininess = shiny;
}
