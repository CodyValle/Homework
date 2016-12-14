#ifndef SHIPANIMATOR_H_INCLUDED
#define SHIPANIMATOR_H_INCLUDED

/**
 * ShipAnimator controls each the enemy ships.\
**/

#include "collisionDetection.h"

class ShipAnimator : public Animator
{
public:
    ShipAnimator(Transform& transform, float maxDistance);

    virtual ~ShipAnimator();

    inline void setEnabled(bool e)
        { enabled = e; }

    inline bool isEnabled()
        { return enabled; }

    // Inherited from Animator
    void animate(float deltaTime);

protected:
    Transform& transform;
    float maxDistance;
    bool enabled;
    CollisionDetector* collisionDetector;
};

#endif // SHIPANIMATOR_H_INCLUDED
