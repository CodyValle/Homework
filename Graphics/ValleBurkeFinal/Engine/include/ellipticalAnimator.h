#ifndef ELLIPTICALANIMATOR_H_INCLUDED
#define ELLIPTICALANIMATOR_H_INCLUDED

/**
 * Moves an object in an elliptical fashion
**/

#include "animator.h"

class EllipticalAnimator : public Animator
{
public:
    // Constructor
    EllipticalAnimator(Transform& t, float major, float minor, float radsPerSec, float initialAngle = 0);

    // Destructor
    ~EllipticalAnimator();

    // Inherited from Animator
    void animate(float deltaTime);

protected:
    Transform& object;
    float mjr; // Major axis radius
    float mnr; // Minor axis radius
    float rads; // Radians per update

    float angle; // the current angle for the ellipse
};

#endif // ELLIPTICALANIMATOR_H_INCLUDED
