#ifndef ENEMYCOLLIDER_H_INCLUDED
#define ENEMYCOLLIDER_H_INCLUDED

#include "collider.h"
#include "shipAnimator.h"

class EnemyDistributor;

class EnemyCollider : public Collider
{
public:
    EnemyCollider(Transform& transform, BoundingBox* bBox, EnemyDistributor* distro, Node* node, ShipAnimator* animator);

    void collision();

private:
    EnemyDistributor* distributor;
    Node* node;
    ShipAnimator* animator;
};

#endif // ENEMYCOLLIDER_H_INCLUDED
