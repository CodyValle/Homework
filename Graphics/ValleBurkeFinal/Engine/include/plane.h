#ifndef PLANE_H
#define PLANE_H

/**
 * The parent class of a plane. SolidPlane and TexturedPlane inherit from this.
**/

#include "drawable.h"

class Plane : public Drawable
{
public:
    // Constructor
    Plane(const unsigned pid);

    // Destructor
    virtual ~Plane();
};

#endif // PLANE_H
