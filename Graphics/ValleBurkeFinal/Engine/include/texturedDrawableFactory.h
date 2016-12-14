#ifndef TEXTUREDDRAWABLEFACTORY_H_INCLUDED
#define TEXTUREDDRAWABLEFACTORY_H_INCLUDED

/**
 * This factory will produce textured objects.
**/

#include "drawableFactory.h"

#include <string>

class TexturedDrawableFactory : public DrawableFactory
{
public:
    // Constructor
    TexturedDrawableFactory();

    // Produce Spheres
    Sphere* makeSphere(unsigned longs = 10, unsigned lats = 5);

    // Produce Hemispheres
    Hemisphere* makeHemisphere(float radius = 2., unsigned longs = 10, unsigned lats = 5);

    // Produce Planes
    Plane* makePlane(unsigned longs = 5, unsigned lats = 5);
    Plane* makePlane(std::string file);
};

#endif // TEXTUREDDRAWABLEFACTORY_H_INCLUDED
