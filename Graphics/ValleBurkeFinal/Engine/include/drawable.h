#ifndef DRAWABLE_H_INCLUDED
#define DRAWABLE_H_INCLUDED

#include "vertex.h"

class Drawable
{
public:
    Drawable(const unsigned pid);
    Drawable(const unsigned pid, float const color[4]);

    virtual ~Drawable();

    virtual void draw(glm::mat4 modelViewMat) = 0;

    virtual void setColor(float const color[4]);

protected:
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
