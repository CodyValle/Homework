#include "YerionDistributor.h"

#include <windows.h>
#include <mmsystem.h>

#include "yerionAnimator.h"
#include "yerionCollider.h"

static const unsigned numShips = 1;

YerionDistributor::YerionDistributor(DrawableFactory* factory, Node* node, float maxDistance)
{
    for (unsigned i = 0; i < numShips; ++i)
    {
        Node* planeNode = new Node();
        planeNode->getTransform().setTranslate(vec3(0, 0, -500));
        planeNode->getTransform().setScale(vec3(30, 30, 30));
        // Create the plane
        Drawable* plane = factory->makePlane("MechaYerion2Test.bmp");
        planeNode->addDrawable(plane);

        // Create an animator
        YerionAnimator* yerionAnimator = new YerionAnimator(planeNode->getTransform(), maxDistance);

        // Connect it to the world
        node->addChild(planeNode, yerionAnimator);
        ships.push_back(planeNode);
        animators.push_back(yerionAnimator);

        // Set up the collider
        BoundingBox* yerionBBox = new BoundingBox(50., 50., 30.);
        Collider* collider = new YerionCollider(planeNode->getTransform(), yerionBBox, this, planeNode, yerionAnimator);
        CollisionDetector::getInstance()->addCollider(collider);
    }
}

void YerionDistributor::launch(vec3 translate)
{
    for (unsigned i = 0; i < numShips; ++i)
    {
        if (!animators.at(i)->isEnabled())
        {
            // From http://ntcoder.com/bab/2007/09/13/using-sndplaysound-to-play-a-wave-file/
            sndPlaySound("BeBop2.wav", SND_ASYNC );
            animators.at(i)->setEnabled(true);
            ships.at(i)->getTransform().setTranslate(translate);
            break;
        }
    }
}

void YerionDistributor::remove(Node* node, ShipAnimator* animator)
{
    for (std::deque<Node*>::iterator it = ships.begin(); it != ships.end(); ++it)
        if (*it == node)
        {
            (*it)->getTransform().setTranslate(vec3(0, 0, 30)); // Move offscreen
            break;
        }
}
