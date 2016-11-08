#ifndef ELLIPTICALANIMATOR_H_INCLUDED
#define ELLIPTICALANIMATOR_H_INCLUDED

#include "animator.h"
#include "drawable.h"

class EllipticalAnimator : public Animator
{
public:
    EllipticalAnimator(Transform& t, float major, float minor, float radsPerSec, float initialAngle = 0) :
        object(t),
        mjr(major),
        mnr(minor),
        rads(radsPerSec),
        angle(initialAngle)
    {
    }

    ~EllipticalAnimator()
    {}

    virtual void animate()
    {
        float x = cos(angle) * mjr + transform.getTranslate()[0];
        float z = sin(angle) * mnr + transform.getTranslate()[2];
        object.setTranslate(glm::vec3(x, 0, z));
        angle = fmod(angle + rads, 2 * PI);
    }

protected:
    Transform& object;
    float mjr; // Major axis radius
    float mnr; // Minor axis radius
    float rads; // Radians per update

    float angle; // the current angle for the ellipse
};

#endif // ELLIPTICALANIMATOR_H_INCLUDED
