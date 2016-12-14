#ifndef DISTRIBUTOR_H_INCLUDED
#define DISTRIBUTOR_H_INCLUDED

/**
 * Parent class for all distributors
**/

#include "node.h"
#include "shipAnimator.h"

class Distributor
{
public:
    Distributor();

    ~Distributor();

    virtual void launch(vec3 translate) = 0;

    virtual void remove(Node* node, ShipAnimator* animator) = 0;
};

#endif // DISTRIBUTOR_H_INCLUDED
