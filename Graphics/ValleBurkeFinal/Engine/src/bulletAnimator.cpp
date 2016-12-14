#include "bulletAnimator.h"

BulletAnimator::BulletAnimator(Transform& transform, BoundingBox* bBox, float speed, float maxDistance) :
    Animator(),
    Collider(transform, bBox),
    object(transform),
    speed(speed),
    maxDistance(maxDistance),
    enabled(false),
    collisionDetector(CollisionDetector::getInstance())
{
}

BulletAnimator::~BulletAnimator()
{
}

// Inherited from Animator
void BulletAnimator::animate(float deltaTime)
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
        if (collisionDetector->checkCollision(this))
        {
            object.setTranslate(vec3(0, 0, -500));
            setEnabled(false);
        }
    }
}
