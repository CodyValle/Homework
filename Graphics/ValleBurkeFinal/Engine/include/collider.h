#ifndef COLLIDER_H_INCLUDED
#define COLLIDER_H_INCLUDED

/**
 * Base class for all types of colliders.
**/

#include "boundingBox.h"
#include "transform.h"
#include "node.h"

class Collider
{
public:
    Collider(Transform& transform, BoundingBox* bBox);

    Collider(Node* node, BoundingBox* bBox);

    virtual ~Collider();

    // Does this collider collide with the passed in collider?
    bool collides(Collider* collider);

    // A collision occured
    virtual void collision();

    // The location of this collider
    inline vec3 getTranslate()
        { return transform.getTranslate(); }

protected:
    Transform& transform;
    Node* node;
    BoundingBox* bBox;
};

#endif // COLLIDER_H_INCLUDED
