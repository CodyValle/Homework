#ifndef COLLISIONDETECTION_H_INCLUDED
#define COLLISIONDETECTION_H_INCLUDED

/**
 * Singleton object that tracks all collidable objects.
**/

#include "collider.h"

class CollisionDetector
{
public:
    static CollisionDetector *getInstance()
    {
        if (!instance)
          instance = new CollisionDetector;
        return instance;
    }

    inline void addCollider(Collider* collider)
        { colliders.push_back(collider); }

    // Only pass in bullet colliders
    bool checkCollision(Collider* collider);

private:
    CollisionDetector();

    static CollisionDetector *instance;

    deque<Collider*> colliders;
};

#endif // COLLISIONDETECTION_H_INCLUDED
