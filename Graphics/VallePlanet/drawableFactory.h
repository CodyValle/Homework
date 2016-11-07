#ifndef DRAWABLEFACTORY_H_INCLUDED
#define DRAWABLEFACTORY_H_INCLUDED

#include "sphere.h"

class DrawableFactory
{
public:
    DrawableFactory(const unsigned pid) :
        programId(pid)
    {
        color[0] = 0.;
        color[1] = 0.;
        color[2] = 0.;
        color[3] = 1.;
    }

    Sphere* makeSphere(float radius = 2., unsigned longs = 10, unsigned lats = 5)
    {
        return new Sphere(programId, color, makeHemisphere(radius, longs, lats));
    }

    Sphere* makeSphere(float const col[4], float radius = 2., unsigned longs = 10, unsigned lats = 5)
    {
        return new Sphere(programId,col, makeHemisphere(col, radius, longs, lats));
    }

    Hemisphere* makeHemisphere(float radius = 2., unsigned longs = 10, unsigned lats = 5)
    {
        return new Hemisphere(programId, color, radius, longs, lats);
    }

    Hemisphere* makeHemisphere(float const col[4], float radius = 2., unsigned longs = 10, unsigned lats = 5)
    {
        return new Hemisphere(programId, col, radius, longs, lats);
    }

protected:
    // GL ids
    const unsigned
        programId;

    // Default color
    float color[4]; // Color of object
};

#endif // DRAWABLEFACTORY_H_INCLUDED
