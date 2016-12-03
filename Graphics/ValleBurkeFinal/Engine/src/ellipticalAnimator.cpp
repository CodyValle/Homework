#include "ellipticalAnimator.h"

EllipticalAnimator::EllipticalAnimator(Transform& t, float major, float minor, float radsPerSec, float initialAngle) :
    object(t),
    mjr(major),
    mnr(minor),
    rads(radsPerSec),
    angle(initialAngle)
{
}

EllipticalAnimator::~EllipticalAnimator()
{
}

void EllipticalAnimator::animate()
{
    // Calculate the new x and z for the transform, then apply translational offset
    float x = cos(angle) * mjr + transform.getTranslate()[0];
    float z = sin(angle) * mnr + transform.getTranslate()[2];
    object.setTranslate(glm::vec3(x, 0, z)); // Update the transform we are animating
    angle = fmod(angle + rads, 2 * PI); // Update the current angle
}
