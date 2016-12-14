#ifndef SOLIDPLANE_H_INCLUDED
#define SOLIDPLANE_H_INCLUDED

/**
 * The solidplane is a one color plane
**/

#include "plane.h"

class SolidPlane: Plane
{
    // Destructor
    ~SolidPlane();

    // Inherited from Drawable
    virtual void draw(mat4 modelViewMat);

protected:
    friend class SolidDrawableFactory; // To access the constructor

    // Constructor
    SolidPlane(const unsigned pid, const unsigned longs = 10, const unsigned lats = 5);

    // Shape data
    unsigned longitudes, // Number of longitudinal slices.
             latitudes; // Number of latitudinal slices.

private:
    /// Methods to fill vertex data
    // Fill the vertex array with co-ordinates of the sample points.
    static void fillPlaneVertexArray(Vertex* planeVertices, const unsigned lats, const unsigned longs)
    {
        int k = 0;
        for (unsigned j = 0; j <= lats; j++)
        {
            float y = (float)j / lats;
            for (unsigned i = 0; i <= longs; i++)
            {
                planeVertices[k].coords.x = (float)i / lats;
                planeVertices[k].coords.y = y;
                planeVertices[k].coords.z = 0.;
                planeVertices[k].coords.w = 1.0;
                k++;
            }
        }
    }

    // Fill the array of index arrays.
    static void fillPlaneIndices(unsigned* planeIndices, const unsigned lats, const unsigned longs)
    {
        for(unsigned j = 0; j < lats; j++)
            for (unsigned i = 0; i <= longs; i++)
            {
                // this might work
                planeIndices[2*(j*(longs+1)+i)]   = (j+1)*(longs + 1) + i;
                planeIndices[2*(j*(longs+1)+i)+1] =   j  *(longs + 1) + i;
            }
    }

    // Fill the array of counts.
    static void fillPlaneCounts(int planeCounts[], const unsigned lats, const unsigned longs)
    {
        for(unsigned j = 0; j < lats; j++)
            planeCounts[j] = 2*(longs + 1);
    }


    // Fill the array of buffer offsets.
    static void fillPlaneOffsets(void* planeOffsets[], const unsigned lats, const unsigned longs)
    {
        for(unsigned j = 0; j < lats; j++)
            planeOffsets[j] = (void*)(2*(longs+1)*j*sizeof(unsigned));
    }
};

#endif // SOLIDPLANE_H_INCLUDED
