#ifndef YERIONDISTRIBUTOR_H_INCLUDED
#define YERIONDISTRIBUTOR_H_INCLUDED

/**
 * This distributor controls the appearance of MechaYerion.
**/

#include "shipAnimator.h"
#include "drawableFactory.h"
#include "enemyCollider.h"
#include "distributor.h"

class YerionDistributor : public Distributor
{
public:
    YerionDistributor(DrawableFactory* factory, Node* node, float maxDistance);

    // Launch a MechaYerion
    void launch(vec3 translate);

    // Remove the MechaYerion from the screen
    void remove(Node* node, ShipAnimator* animator);

private:
    std::deque<Node*> ships;
    std::deque<ShipAnimator*> animators;
};

#endif // YERIONDISTRIBUTOR_H_INCLUDED
