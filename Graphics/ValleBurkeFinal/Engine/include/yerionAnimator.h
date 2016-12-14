#ifndef YERIONANIMATOR_H_INCLUDED
#define YERIONANIMATOR_H_INCLUDED

/**
 * YerionAnimator controls the movement of a MechYerion object.
**/

#include "shipAnimator.h"

class YerionAnimator : public ShipAnimator
{
public:
    YerionAnimator(Transform& transform, float maxDistance);

    ~YerionAnimator();

    // Inherited from Animator
    void animate(float deltaTime);
};

#endif // YERIONANIMATOR_H_INCLUDED
