#ifndef DRAWABLE_H_INCLUDED
#define DRAWABLE_H_INCLUDED

/**
 * Parent class for all drawble objects.
**/

#include "vertex.h"
#include "material.h"

using namespace glm;

class Drawable
{
public:
    Drawable(const unsigned pid);

    virtual ~Drawable();

    virtual void draw(mat4 modelViewMat) = 0;

    virtual void setAmbient(const float r, const float g, const float b);
    virtual void setDiffuse(const float r, const float g, const float b);
    virtual void setSpecular(const float r, const float g, const float b);
    virtual void setEmittance(const float r, const float g, const float b);
    virtual void setShininess(const float shiny);

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
        normalMatLoc;

    // Object material properties.
    Material objFront;
};

#endif // DRAWABLE_H_INCLUDED
