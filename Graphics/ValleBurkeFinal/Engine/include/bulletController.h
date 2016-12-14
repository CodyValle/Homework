#ifndef BULLETCONTROLLER_H_INCLUDED
#define BULLETCONTROLLER_H_INCLUDED

/**
 * Controls the bullets in the scene
**/

#include "drawableFactory.h"
#include "node.h"
#include "bulletAnimator.h"

static const unsigned numBullets = 15;

class BulletController
{
public:
    BulletController(DrawableFactory* factory, Node* rootNode, float maxDistance);

    ~BulletController();

    void shoot(vec3 translate);

private:
    float maxDistance;
    Node* bullets[numBullets];
    BulletAnimator* animators[numBullets];
};

#endif // BULLETCONTROLLER_H_INCLUDED
