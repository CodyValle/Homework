#include "enemyCollider.h"
#include "EnemyDistributor.h"

EnemyCollider::EnemyCollider(Transform& transform, BoundingBox* bBox, EnemyDistributor* distro, Node* node, ShipAnimator* animator) :
    Collider(transform, bBox),
    distributor(distro),
    node(node),
    animator(animator)
{
}

void EnemyCollider::collision()
{
    distributor->remove(node, animator);
}

