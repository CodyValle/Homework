#ifndef DRAWABLEFACTORY_H_INCLUDED
#define DRAWABLEFACTORY_H_INCLUDED

/**
 * Parent class for all drawables.
**/

#include "sphere.h"
#include "plane.h"

#include <string>

class DrawableFactory
{
public:
    // Constructor
    DrawableFactory();

    // Produce Spheres
    virtual Sphere* makeSphere(unsigned longs, unsigned lats) = 0;

    // Produce Hemispheres
    virtual Hemisphere* makeHemisphere(float radius, unsigned longs, unsigned lats) = 0;

    // Produce Planes
    virtual Plane* makePlane(unsigned longs = 30, unsigned lats = 30) = 0;
    virtual Plane* makePlane(std::string file) = 0;

protected:
    // GL ids
    unsigned programId;
    mat4 projMat;
};

#endif // DRAWABLEFACTORY_H_INCLUDED
