#include "yerionCollider.h"
#include "EnemyDistributor.h"
#include <windows.h>
#include <mmsystem.h>

YerionCollider::YerionCollider(Transform& transform, BoundingBox* bBox, Distributor* distro, Node* node, ShipAnimator* animator) :
    Collider(transform, bBox),
    distributor(distro),
    node(node),
    animator(animator),
    health(8)
{
}

void YerionCollider::collision()
{
    --health;
    if (health < 0)
    {
        // From http://ntcoder.com/bab/2007/09/13/using-sndplaysound-to-play-a-wave-file/
        sndPlaySound("BeBop3.wav", SND_ASYNC );
        distributor->remove(node, animator);
        health = 8;
    }
    else
        // From http://ntcoder.com/bab/2007/09/13/using-sndplaysound-to-play-a-wave-file/
        sndPlaySound("BeBop1.wav", SND_ASYNC );
}

