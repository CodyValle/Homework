#include "yerionAnimator.h"

YerionAnimator::YerionAnimator(Transform& transform, float maxDistance) :
    ShipAnimator(transform, maxDistance)
{
}

YerionAnimator::~YerionAnimator()
{
}

// Inherited from Animator
void YerionAnimator::animate(float deltaTime)
{
    if (enabled)
    {
        // Move
        transform.translate(vec3(0, 0, 0.2 * deltaTime));

        if (transform.getTranslate()[2] >= maxDistance)
            setEnabled(false);
    }
}
