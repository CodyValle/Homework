#ifndef ENEMYCOLLIDER_H_INCLUDED
#define ENEMYCOLLIDER_H_INCLUDED

/**
 * Reacts to a collsion witha  bullet.
**/

#include "collider.h"
#include "shipAnimator.h"
#include "distributor.h"

class EnemyDistributor;

class EnemyCollider : public Collider
{
public:
    EnemyCollider(Transform& transform, BoundingBox* bBox, Distributor* distro, Node* node, ShipAnimator* animator);

    // Called when hit by a bullet
    void collision();

private:
    Distributor* distributor;
    Node* node;
    ShipAnimator* animator;
};

#endif // ENEMYCOLLIDER_H_INCLUDED
