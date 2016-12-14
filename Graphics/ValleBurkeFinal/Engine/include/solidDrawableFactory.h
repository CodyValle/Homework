#ifndef SOLIDDRAWABLEFACTORY_H_INCLUDED
#define SOLIDDRAWABLEFACTORY_H_INCLUDED

/**
 * This drawable produces solid color objects.
**/

#include "drawableFactory.h"

#include <string>

class SolidDrawableFactory : public DrawableFactory
{
public:
    // Constructor
    SolidDrawableFactory();

    // Produce Spheres
    Sphere* makeSphere(unsigned longs = 10, unsigned lats = 5);

    // Produce Hemispheres
    Hemisphere* makeHemisphere(float radius = 2., unsigned longs = 10, unsigned lats = 5);

    // Produce Planes
    Plane* makePlane(unsigned longs, unsigned lats);
    Plane* makePlane(std::string file);
};

#endif // SOLIDDRAWABLEFACTORY_H_INCLUDED
