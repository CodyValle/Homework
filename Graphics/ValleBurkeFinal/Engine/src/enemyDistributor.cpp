#include "enemyDistributor.h"

EnemyDistributor::EnemyDistributor(DrawableFactory* factory, Node* node, float maxDistance) :
    Distributor()
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
        Drawable* wingsObj = factory->makeSphere(15., 15.);
        wingsNode->addDrawable(wingsObj);
        // Change the color
        wingsObj->setAmbient(.8, 0.5, 0.2);
        // Change the shape
        wingsNode->getTransform().translate(vec3(0, 0.3, 1.0));
        wingsNode->getTransform().setScale(vec3(5., 0.2, .6));

        // Create the body
        Node* bodyNode = new Node();
        shipParentNode->addChild(bodyNode);
        // Add the body
        Drawable* bodyObj = factory->makeSphere(30., 30.);
        bodyNode->addDrawable(bodyObj);
        // Change the color
        bodyObj->setAmbient(.8, 0.3, 0.2);
        // Change the shape
        bodyNode->getTransform().translate(vec3(0., 0., -3.));
        bodyNode->getTransform().rotate(vec3(0., 180., 0.));
        bodyNode->getTransform().setScale(vec3(1., 1., 6.));
    }
}

void EnemyDistributor::launch(vec3 translate)
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

void EnemyDistributor::remove(Node* node, ShipAnimator* animator)
{
    for (std::deque<Node*>::iterator it = ships.begin(); it != ships.end(); ++it)
        if (*it == node)
        {
            (*it)->getTransform().setTranslate(vec3(0, 0, 30)); // Move offscreen
            break;
        }
}
