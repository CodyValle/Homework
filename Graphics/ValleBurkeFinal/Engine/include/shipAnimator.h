#ifndef SHIPANIMATOR_H_INCLUDED
#define SHIPANIMATOR_H_INCLUDED

#include "collisionDetection.h"

class ShipAnimator : public Animator
{
public:
    ShipAnimator(Transform& transform, float maxDistance) :
        Animator(),
        transform(transform),
        maxDistance(maxDistance),
        enabled(false)
    {
    }

    ~ShipAnimator()
    {
    }

    inline void setEnabled(bool e)
        { enabled = e; }

    inline bool isEnabled()
        { return enabled; }

    // Inherited from Animator
    void animate(float deltaTime)
    {
        if (enabled)
        {
            // Move
            transform.translate(vec3(0, 0, 0.1 * deltaTime));

            if (transform.getTranslate()[2] >= maxDistance)
                setEnabled(false);
        }
    }

private:
    Transform& transform;
    float maxDistance;
    bool enabled;
    CollisionDetector* collisionDetector;
};

#endif // SHIPANIMATOR_H_INCLUDED
