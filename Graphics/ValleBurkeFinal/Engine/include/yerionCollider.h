#ifndef YERIONCOLLIDER_H_INCLUDED
#define YERIONCOLLIDER_H_INCLUDED

/**
 * A YeronCollider checks to see that a bullet has hit a MechYerion object.
**/

#include "collider.h"
#include "shipAnimator.h"
#include "distributor.h"

class EnemyDistributor;

class YerionCollider : public Collider
{
public:
    YerionCollider(Transform& transform, BoundingBox* bBox, Distributor* distro, Node* node, ShipAnimator* animator);

    // Called when something strikes a MechaYerion object
    void collision();

private:
    Distributor* distributor;
    Node* node;
    ShipAnimator* animator;
    int health;
};

#endif // YERIONCOLLIDER_H_INCLUDED
