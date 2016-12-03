#include "drawable.h"

#include <GL/glew.h>

Drawable::Drawable(const unsigned pid) :
    vertices('\0'),
    indices('\0'),
    counts('\0'),
    offsets('\0'),
    programId(pid),
    vao(0),
    vbo_vertices(0),
    vbo_indices(0),
    matrixLoc(0),
    objectLoc(0)
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
    objectLoc = glGetUniformLocation(programId, "object");
}

Drawable::Drawable(const unsigned pid, float const color[4]) :
    vertices('\0'),
    indices('\0'),
    counts('\0'),
    offsets('\0'),
    programId(pid),
    vao(0),
    vbo_vertices(0),
    vbo_indices(0),
    matrixLoc(0),
    objectLoc(0)
{
    objectColor[0] = color[0];
    objectColor[1] = color[1];
    objectColor[2] = color[2];
    objectColor[3] = color[3];

    // Create VAOs and VBOs...
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo_indices);
    glGenBuffers(1, &vbo_vertices);

    // Obtain modelview matrix uniform and object uniform locations.
    matrixLoc = glGetUniformLocation(programId, "modelViewMat");
    objectLoc = glGetUniformLocation(programId, "object");
}

Drawable::~Drawable()
{
    delete[] vertices;
    delete[] indices;
    delete[] counts;
    delete[] offsets;
}

void Drawable::setColor(float const color[4])
{
    objectColor[0] = color[0];
    objectColor[1] = color[1];
    objectColor[2] = color[2];
    objectColor[3] = color[3];
}
