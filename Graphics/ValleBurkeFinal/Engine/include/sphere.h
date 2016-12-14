#ifndef SPHERE_H_INCLUDED
#define SPHERE_H_INCLUDED

#include "hemisphere.h"

/**
 * The Sphere class is poorly named. It actually creates a teardrop shape.
**/

class Sphere : public Drawable
{
public:
    // Destructor
    ~Sphere();

    // Inherited from Drawable
    void draw(mat4 modelViewMat);

protected:
    friend class SolidDrawableFactory; // To access the constructor

    // Constructor
    Sphere(const unsigned pid, const unsigned longs = 10, const unsigned lats = 5);

    // Shape data
    unsigned longitudes, // Number of longitudinal slices.
             latitudes; // Number of latitudinal slices.

    /// Methods to fill vertex data
    // Fill the vertex array with co-ordinates of the sample points.
    static void fillSphereVertexArray(Vertex* sphereVertices, const unsigned lats, const unsigned longs)
    {
        int k = 0;
        for (unsigned j = 0; j <= lats; ++j)
            for (unsigned i = 0; i <= longs; ++i)
            {
                float x = cos( (-1 + 2 * (float)i / longs) * PI ) * sin(PI * (float)j / lats);
                float y = sin( (-1 + 2 * (float)i / longs) * PI ) * sin(PI * (float)j / lats);
                float z = -cos(PI / 2 * (float)j / lats);

                sphereVertices[k].coords.x = x;
                sphereVertices[k].coords.y = y;
                sphereVertices[k].coords.z = z;
                sphereVertices[k].coords.w = 1.0;

                sphereVertices[k].normal.x = x;
                sphereVertices[k].normal.y = y;
                sphereVertices[k].normal.z = z;

                ++k;
            }
    }

    // Fill the array of index arrays.
    static void fillSphereIndices(unsigned* sphereIndices, const unsigned lats, const unsigned longs)
    {
        for(unsigned j = 0; j < lats; j++)
            for (unsigned i = 0; i <= longs; i++)
            {
                sphereIndices[2*(j*(longs+1)+i)]   = (j+1)*(longs + 1) + i;
                sphereIndices[2*(j*(longs+1)+i)+1] =   j  *(longs + 1) + i;
            }
    }

    // Fill the array of counts.
    static void fillSphereCounts(int sphereCounts[], const unsigned lats, const unsigned longs)
    {
        for(unsigned j = 0; j < lats; j++)
            sphereCounts[j] = 2*(longs + 1);
    }


    // Fill the array of buffer offsets.
    static void fillSphereOffsets(void* sphereOffsets[], const unsigned lats, const unsigned longs)
    {
        for(unsigned j = 0; j < lats; j++)
            sphereOffsets[j] = (void*)(2*(longs+1)*j*sizeof(unsigned));
    }
};

#endif // SPHERE_H_INCLUDED
