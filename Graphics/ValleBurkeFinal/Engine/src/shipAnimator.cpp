#include "shipAnimator.h"

ShipAnimator::ShipAnimator(Transform& transform, float maxDistance) :
    Animator(),
    transform(transform),
    maxDistance(maxDistance),
    enabled(false)
{
}

ShipAnimator::~ShipAnimator()
{
}

// Inherited from Animator
void ShipAnimator::animate(float deltaTime)
{
    if (enabled)
    {
        // Move
        transform.translate(vec3(0, 0, 0.2 * deltaTime));

        if (transform.getTranslate()[2] >= maxDistance)
            setEnabled(false);
    }
}
