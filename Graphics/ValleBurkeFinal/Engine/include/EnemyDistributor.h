#ifndef ENEMYDISTRIBUTOR_H_INCLUDED
#define ENEMYDISTRIBUTOR_H_INCLUDED

#include "shipAnimator.h"
#include "drawableFactory.h"
#include "enemyCollider.h"

static const unsigned numShips = 10;

class EnemyDistributor
{
public:
    EnemyDistributor(DrawableFactory* factory, Node* node, float maxDistance)
    {
        for (unsigned i = 0; i < numShips; ++i)
        {
            Node* shipParentNode = new Node();
            shipParentNode->getTransform().setTranslate(vec3(0., 0., 30.));
            // Create an animator
            ShipAnimator* shipAnimator = new ShipAnimator(shipParentNode->getTransform(), maxDistance);

            // Connect it to the world
            node->addChild(shipParentNode, shipAnimator);
            ships.push_back(shipParentNode);
            animators.push_back(shipAnimator);

            // Set up the collider
            BoundingBox* shipBBox = new BoundingBox(20., 20., 30.);
            Collider* collider = new EnemyCollider(shipParentNode->getTransform(), shipBBox, this, shipParentNode, shipAnimator);
            CollisionDetector::getInstance()->addCollider(collider);

            // Create the wings
            Node* wingsNode = new Node();
            shipParentNode->addChild(wingsNode);
            // Add the wings
            Drawable* wingsObj = factory->makeSphere(1., 4., 4.);
            wingsNode->addDrawable(wingsObj);
            // Change the color
            float wingsColor[] = {.8, 0.5, 0.2, 1.};
            wingsObj->setColor(wingsColor);
            // Change the shape
            wingsNode->getTransform().setScale(vec3(6., 0.2, .6));

            // Create the body
            Node* bodyNode = new Node();
            shipParentNode->addChild(bodyNode);
            // Add the body
            Drawable* bodyObj = factory->makeSphere(2., 4., 4.);
            bodyNode->addDrawable(bodyObj);
            // Change the color
            float bodyColor[] = {.8, 0.3, 0.2, 1.};
            bodyObj->setColor(bodyColor);
            // Change the shape
            bodyNode->getTransform().setScale(vec3(1., 1., 2.));
        }
    }

    void launchShip(vec3 translate)
    {
        for (unsigned i = 0; i < numShips; ++i)
        {
            if (!animators.at(i)->isEnabled())
            {
                animators.at(i)->setEnabled(true);
                ships.at(i)->getTransform().setTranslate(translate);
                break;
            }
        }
    }

    void remove(unsigned i)
    {
        ships.at(i)->getTransform().setTranslate(vec3(0, 0, 30)); // Move offscreen
        animators.at(i)->setEnabled(false);
    }

    void remove(Node* node, ShipAnimator* animator)
    {
        for (std::deque<Node*>::iterator it = ships.begin(); it != ships.end(); ++it)
            if (*it == node)
            {
                (*it)->getTransform().setTranslate(vec3(0, 0, 30)); // Move offscreen
                break;
            }
    }

private:
    std::deque<Node*> ships;
    std::deque<ShipAnimator*> animators;
};

#endif // ENEMYDISTRIBUTOR_H_INCLUDED
