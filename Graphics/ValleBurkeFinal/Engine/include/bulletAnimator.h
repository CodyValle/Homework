#ifndef BULLETANIMATOR_H_INCLUDED
#define BULLETANIMATOR_H_INCLUDED

#include "animator.h"
#include "collisionDetection.h"

class BulletAnimator : public Animator, Collider
{
public:
    BulletAnimator(Transform& transform, BoundingBox* bBox, float speed, float maxDistance) :
        Animator(),
        Collider(transform, bBox),
        object(transform),
        speed(speed),
        maxDistance(maxDistance),
        enabled(false),
        collisionDetector(CollisionDetector::getInstance())
    {}

    ~BulletAnimator()
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
            object.translate(vec3(0, 0, speed * deltaTime));

            if (object.getTranslate()[2] <= maxDistance)
            {
                setEnabled(false);
                return;
            }

            // Check for collision
            collisionDetector->checkCollision(this);
        }
    }

private:
    Transform& object;
    float speed;
    float maxDistance;
    bool enabled;
    CollisionDetector* collisionDetector;
};

#endif // BULLETANIMATOR_H_INCLUDED
