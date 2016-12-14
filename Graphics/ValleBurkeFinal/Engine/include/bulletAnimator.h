#ifndef BULLETANIMATOR_H_INCLUDED
#define BULLETANIMATOR_H_INCLUDED

/**
 * Controls the motion of a bullet.
**/

#include "animator.h"
#include "collisionDetection.h"

class BulletAnimator : public Animator, Collider
{
public:
    BulletAnimator(Transform& transform, BoundingBox* bBox, float speed, float maxDistance);

    ~BulletAnimator();

    inline void setEnabled(bool e)
        { enabled = e; }

    inline bool isEnabled()
        { return enabled; }

    // Inherited from Animator
    void animate(float deltaTime);

private:
    Transform& object;
    float speed;
    float maxDistance;
    bool enabled;
    CollisionDetector* collisionDetector;
};

#endif // BULLETANIMATOR_H_INCLUDED
