#ifndef TEXTUREDPLANE_H_INCLUDED
#define TEXTUREDPLANE_H_INCLUDED

/**
 * A texturedPlane is a plane object that renders an image.
**/

#include "plane.h"
#include "textureVertex.h"
#include "getbmp.h"

using namespace glm;

class TexturedPlane : public Plane
{
public:
    // Destructor
    ~TexturedPlane();

    // Inherited from Drawable
    void draw(mat4 modelViewMat);

protected:
    friend class TexturedDrawableFactory; // To access the constructor

    // Constructor
    TexturedPlane(const unsigned pid, std::string file);

    BitMapFile* image;
    unsigned texture, myTextureID;
    static unsigned textureID;

private:
    /// Methods to fill vertex data
    // Fill the vertex array with co-ordinates of the sample points.
    static void fillPlaneVertexArray(TextureVertex* vertices)
    {
        vertices[0].coords.x = 0.5;
        vertices[0].coords.y = -0.5;
        vertices[0].coords.z = 0.0;
        vertices[0].coords.w = 1.0;
        vertices[0].texCoords.x = 1.0;
        vertices[0].texCoords.y = 0.0;

        vertices[1].coords.x = 0.5;
        vertices[1].coords.y = 0.5;
        vertices[1].coords.z = 0.0;
        vertices[1].coords.w = 1.0;
        vertices[1].texCoords.x = 1.0;
        vertices[1].texCoords.y = 1.0;

        vertices[2].coords.x = -0.5;
        vertices[2].coords.y = -0.5;
        vertices[2].coords.z = 0.0;
        vertices[2].coords.w = 1.0;
        vertices[2].texCoords.x = 0.0;
        vertices[2].texCoords.y = 0.0;

        vertices[3].coords.x = -0.5;
        vertices[3].coords.y = 0.5;
        vertices[3].coords.z = 0.0;
        vertices[3].coords.w = 1.0;
        vertices[3].texCoords.x = 0.0;
        vertices[3].texCoords.y = 1.0;
    }
};

#endif // TEXTUREDPLANE_H_INCLUDED
