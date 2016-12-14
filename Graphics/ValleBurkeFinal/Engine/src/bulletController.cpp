#include "bulletController.h"

BulletController::BulletController(DrawableFactory* factory, Node* rootNode, float maxDistance) :
    maxDistance(maxDistance)
{
    for (unsigned i = 0; i < numBullets; ++i)
    {
        // Set up the bullet node
        bullets[i] = new Node();
        bullets[i]->getTransform().setTranslate(vec3(0.,0.,-500.)); // Off the screen
        bullets[i]->getTransform().setScale(vec3(0.5, 0.5, 2));
        // Set up the bullet drawable
        Drawable* bullet = factory->makeSphere(20, 20);
        bullet->setAmbient(1.0 , 1.0, 0.);
        bullet->setDiffuse(1.0, 1.0, 0.);
        bullet->setSpecular(1.0, 1.0, 1.0);
        bullet->setShininess(1000.0);
        bullets[i]->addDrawable(bullet);
        // Set up the animator
        BoundingBox* bBox = new BoundingBox(.5, .5, .5);
        animators[i] = new BulletAnimator(bullets[i]->getTransform(), bBox, -2.5, maxDistance); // Speed: -1 MaxDistance: 100
        animators[i]->setEnabled(false);
        // Attach to the rootNode
        rootNode->addChild(bullets[i], animators[i]);
    }
}

BulletController::~BulletController()
{
}

void BulletController::shoot(vec3 translate)
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
