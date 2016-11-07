#ifndef SPHERE_H_INCLUDED
#define SPHERE_H_INCLUDED

#include "hemisphere.h"

class Sphere : public Drawable
{
public:
    void draw(mat4 modelViewMat)
    {
        // Draw the top
        hemisphere->draw(modelViewMat);
        // Draw the bottom
        modelViewMat = scale(modelViewMat, vec3(1., -1., 1.));
        hemisphere->draw(modelViewMat);
    }

    ~Sphere()
    {
        delete hemisphere;
    }

protected:
    friend class DrawableFactory;

    Sphere(const unsigned int pid, float const color[4], Hemisphere* hemi) :
        Drawable(pid, color),
        hemisphere(hemi)
    {
    }

    Hemisphere* hemisphere;
};

#endif // SPHERE_H_INCLUDED
