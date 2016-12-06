#ifndef COLLIDER_H_INCLUDED
#define COLLIDER_H_INCLUDED

#include "boundingBox.h"
#include "transform.h"
#include "node.h"

class Collider
{
public:
    Collider(Transform& transform, BoundingBox* bBox) :
        transform(transform),
        node(0),
        bBox(bBox)
    {
    }
    Collider(Node* node, BoundingBox* bBox) :
        transform(node->getTransform()),
        node(node),
        bBox(bBox)
    {
    }

    virtual ~Collider()
    {
        delete bBox;
    }

    // Base code from http://stackoverflow.com/questions/6083626/box-collision-code
    bool collides(Collider* collider)
    {
        vec3 translate = transform.getTranslate();
        float x = translate[0];
        float y = translate[1];
        float z = translate[2];
        float xTwo = collider->getTranslate()[0];
        float yTwo = collider->getTranslate()[1];
        float zTwo = collider->getTranslate()[2];
        float oWidth = bBox->getWidth() / 2;
        float oHeight = bBox->getHeight() / 2;
        float oDepth = bBox->getDepth() / 2;
        float oTwoWidth = collider->bBox->getWidth() / 2;
        float oTwoHeight = collider->bBox->getHeight() / 2;
        float oTwoDepth = collider->bBox->getDepth() / 2;

        if ( x+oWidth < xTwo || x > xTwo+oTwoWidth ) return false;
        if ( y+oHeight < yTwo || y > yTwo+oTwoHeight ) return false;
        if ( z+oDepth < zTwo || z > zTwo+oTwoDepth ) return false;

        return true;
    }

    virtual void collision()
    {
        node->destroy();
    }

    inline vec3 getTranslate()
        { return transform.getTranslate(); }

protected:
    Transform& transform;
    Node* node;
    BoundingBox* bBox;
};

#endif // COLLIDER_H_INCLUDED
