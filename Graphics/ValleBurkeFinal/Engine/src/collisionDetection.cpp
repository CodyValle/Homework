#include "collisionDetection.h"

CollisionDetector* CollisionDetector::instance;

// Only pass in bullet colliders
void CollisionDetector::checkCollision(Collider* collider)
{
    for (std::deque<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
    {
        if ((*it) == collider) continue;
        if ((*it)->collides(collider))
        {
            //colliders.erase(it);
            (*it)->collision();
            break;
        }
    }
}

CollisionDetector::CollisionDetector()
{}
