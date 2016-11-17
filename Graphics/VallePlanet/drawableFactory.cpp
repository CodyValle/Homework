#include "drawableFactory.h"

DrawableFactory::DrawableFactory(const unsigned pid) :
    programId(pid)
{
    color[0] = 0.;
    color[1] = 0.;
    color[2] = 0.;
    color[3] = 1.;
}

Sphere* DrawableFactory::makeSphere(float radius, unsigned longs, unsigned lats)
{
    return new Sphere(programId, color, makeHemisphere(radius, longs, lats));
}

Sphere* DrawableFactory::makeSphere(float const col[4], float radius, unsigned longs, unsigned lats)
{
    return new Sphere(programId, col, makeHemisphere(col, radius, longs, lats));
}

Hemisphere* DrawableFactory::makeHemisphere(float radius, unsigned longs, unsigned lats)
{
    return new Hemisphere(programId, color, radius, longs, lats);
}

Hemisphere* DrawableFactory::makeHemisphere(float const col[4], float radius, unsigned longs, unsigned lats)
{
    return new Hemisphere(programId, col, radius, longs, lats);
}
