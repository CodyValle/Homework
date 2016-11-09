#ifndef DRAWABLEFACTORY_H_INCLUDED
#define DRAWABLEFACTORY_H_INCLUDED

#include "sphere.h"

class DrawableFactory
{
public:
    // Constructor
    DrawableFactory(const unsigned pid);

    // Produce Spheres
    Sphere* makeSphere(float radius = 2., unsigned longs = 10, unsigned lats = 5);
    Sphere* makeSphere(float const col[4], float radius = 2., unsigned longs = 10, unsigned lats = 5);

    // Produce Hemispheres
    Hemisphere* makeHemisphere(float radius = 2., unsigned longs = 10, unsigned lats = 5);
    Hemisphere* makeHemisphere(float const col[4], float radius = 2., unsigned longs = 10, unsigned lats = 5);

protected:
    // GL ids
    const unsigned
        programId;

    // Default color is black
    float color[4]; // Color of object
};

#endif // DRAWABLEFACTORY_H_INCLUDED
