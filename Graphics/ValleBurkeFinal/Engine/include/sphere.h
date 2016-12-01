#ifndef SPHERE_H_INCLUDED
#define SPHERE_H_INCLUDED

#include "hemisphere.h"

/*
 * Sphere class needs to be updated to use its own
 * vertices and such, instead of drawing two Hemispheres.
 */

class Sphere : public Drawable
{
public:
    // Destructor
    ~Sphere();

    // Inherited from Drawable
    void draw(mat4 modelViewMat);

    void setColor(float const color[4]);

protected:
    friend class DrawableFactory; // To access the constructor

    // Constructor
    Sphere(const unsigned int pid, Hemisphere* hemi);
    Sphere(const unsigned int pid, float const color[4], Hemisphere* hemi);

    // The hemisphere that will make up this sphere
    Hemisphere* hemisphere;
};

#endif // SPHERE_H_INCLUDED
