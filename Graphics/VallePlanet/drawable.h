#ifndef DRAWABLE_H_INCLUDED
#define DRAWABLE_H_INCLUDED

#include <GL/glew.h>
#include <GL/freeglut.h>
#ifdef __APPLE__
#  include <OpenGL/glext.h>
#else
#  include <GL/glext.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "vertex.h"

using namespace glm;

static unsigned numObjects = 0;

class Drawable
{
public:
    Drawable(const unsigned pid, float const color[4]):
        objectID(numObjects++),
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

    virtual ~Drawable()
    {
        delete[] vertices;
        delete[] indices;
        delete[] counts;
        delete[] offsets;
    }

    virtual void draw(mat4 modelViewMat) = 0;

    void setColor(float const color[4])
    {
        objectColor[0] = color[0];
        objectColor[1] = color[1];
        objectColor[2] = color[2];
        objectColor[3] = color[3];
    }

protected:
    unsigned objectID;

    float objectColor[4]; // Color of object

    // Vertex data
    Vertex* vertices;
    unsigned* indices;
    int* counts;
    void** offsets;

    // GL ids
    unsigned
        programId,
        vao,
        vbo_vertices,
        vbo_indices,
        matrixLoc,
        objectLoc;
};

#endif // DRAWABLE_H_INCLUDED
