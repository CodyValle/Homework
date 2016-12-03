#ifndef DRAWABLEFACTORY_H_INCLUDED
#define DRAWABLEFACTORY_H_INCLUDED

#include "sphere.h"

class DrawableFactory
{
public:
    // Constructor
    DrawableFactory();

    // Produce Spheres
    Sphere* makeSphere(float radius = 2., unsigned longs = 10, unsigned lats = 5);
    Sphere* makeSphere(float const col[4], float radius = 2., unsigned longs = 10, unsigned lats = 5);

    // Produce Hemispheres
    Hemisphere* makeHemisphere(float radius = 2., unsigned longs = 10, unsigned lats = 5);
    Hemisphere* makeHemisphere(float const col[4], float radius = 2., unsigned longs = 10, unsigned lats = 5);

protected:
    // GL ids
    unsigned programId;
};

#endif // DRAWABLEFACTORY_H_INCLUDED
