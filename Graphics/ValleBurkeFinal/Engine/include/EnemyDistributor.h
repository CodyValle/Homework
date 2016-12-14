#ifndef ENEMYDISTRIBUTOR_H_INCLUDED
#define ENEMYDISTRIBUTOR_H_INCLUDED

/**
 * This class places the enemies in the scene when told to launch.
**/

#include "shipAnimator.h"
#include "drawableFactory.h"
#include "enemyCollider.h"
#include "distributor.h"

static const unsigned numShips = 10;

class EnemyDistributor : public Distributor
{
public:
    EnemyDistributor(DrawableFactory* factory, Node* node, float maxDistance);

    // Launches an enemy ship
    void launch(vec3 translate);

    // Removes an enemy ship from the scene
    void remove(Node* node, ShipAnimator* animator);

private:
    std::deque<Node*> ships;
    std::deque<ShipAnimator*> animators;
};

#endif // ENEMYDISTRIBUTOR_H_INCLUDED
