#ifndef BULLETCONTROLLER_H_INCLUDED
#define BULLETCONTROLLER_H_INCLUDED

#include "drawableFactory.h"
#include "node.h"
#include "bulletAnimator.h"

static const unsigned numBullets = 10;

class BulletController
{
public:
    BulletController(DrawableFactory* factory, Node* rootNode, float maxDistance) :
        maxDistance(maxDistance)
    {
        for (unsigned i = 0; i < numBullets; ++i)
        {
            // Set up the bullet node
            bullets[i] = new Node();
            bullets[i]->getTransform().setTranslate(vec3(0.,0.,-500.)); // Off the screen
            bullets[i]->getTransform().setScale(vec3(0.5, 0.5, 0.5));
            // Set up the bullet drawable
            Drawable* bullet = factory->makeSphere(1, 4, 4);
            float bulletColor[] = {.8, .8, 0., 1.};
            bullet->setColor(bulletColor);
            bullets[i]->addDrawable(bullet);
            // Set up the animator
            BoundingBox* bBox = new BoundingBox(.5, .5, .5);
            animators[i] = new BulletAnimator(bullets[i]->getTransform(), bBox, -1, maxDistance); // Speed: -1 MaxDistance: 100
            animators[i]->setEnabled(false);
            // Attach to the rootNode
            rootNode->addChild(bullets[i], animators[i]);
        }
    }

    ~BulletController()
    {}

    void shoot(vec3 translate)
    {
        for (unsigned i = 0; i < numBullets; ++i)
        {
            Transform& transform = bullets[i]->getTransform();

            if (!animators[i]->isEnabled())
            {
                animators[i]->setEnabled(true);
                transform.setTranslate(translate);
                break;
            }
        }
    }

private:
    float maxDistance;
    Node* bullets[numBullets];
    BulletAnimator* animators[numBullets];
};

#endif // BULLETCONTROLLER_H_INCLUDED
