#ifndef HEMISPHERE_H
#define HEMISPHERE_H

#include "drawable.h"

using namespace glm;

static const double PI = 3.14159265358979;

class Hemisphere : public Drawable
{
public:
    void draw(mat4 modelViewMat);

    ~Hemisphere()
    {
        delete[] vertices;
        delete[] indices;
        delete[] counts;
        delete[] offsets;
    }

protected:
    friend class DrawableFactory;

    Hemisphere(const unsigned pid, float const color[4], float r = 2.0, unsigned longs = 10, unsigned lats = 5);

    // Shape data
    float radius; // Hemisphere radius.
    unsigned longitudes, // Number of longitudinal slices.
             latitudes; // Number of latitudinal slices.

    /// Methods to fill vertex data
    // Fill the vertex array with co-ordinates of the sample points.
    static void fillHemVertexArray(Vertex* hemVertices, float radius, unsigned lats, unsigned longs)
    {
        int k = 0;
        for (unsigned j = 0; j <= lats; j++)
            for (unsigned i = 0; i <= longs; i++)
            {
                hemVertices[k].coords.x = radius * cos( (float)j/lats * PI/2.0 ) * cos( 2.0 * (float)i/longs * PI );
                hemVertices[k].coords.y = radius * sin( (float)j/lats * PI/2.0 );
                hemVertices[k].coords.z = radius * cos( (float)j/lats * PI/2.0 ) * sin( 2.0 * (float)i/longs * PI );
                hemVertices[k].coords.w = 1.0;
                k++;
            }
    }

    // Fill the array of index arrays.
    static void fillHemIndices(unsigned* hemIndices, unsigned lats, unsigned longs)
    {
        for(unsigned j = 0; j < lats; j++)
            for (unsigned i = 0; i <= longs; i++)
            {
                hemIndices[2*(j*(longs+1)+i)]   = (j+1)*(longs + 1) + i;
                hemIndices[2*(j*(longs+1)+i)+1] =   j  *(longs + 1) + i;
            }
    }

    // Fill the array of counts.
    static void fillHemCounts(int hemCounts[], unsigned lats, unsigned longs)
    {
        for(unsigned j = 0; j < lats; j++)
            hemCounts[j] = 2*(longs + 1);
    }

    // Fill the array of buffer offsets.
    static void fillHemOffsets(void* hemOffsets[], unsigned lats, unsigned longs)
    {
        for(unsigned j = 0; j < lats; j++)
            hemOffsets[j] = (GLvoid*)(2*(longs+1)*j*sizeof(unsigned));
    }
};

#endif
