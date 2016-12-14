#ifndef ENEMYLAUNCHER_H_INCLUDED
#define ENEMYLAUNCHER_H_INCLUDED

/**
 * This launches all enemies. MechaYerions and normal enemy ships.
**/

#include <cstdlib>

#include "enemyDistributor.h"
#include "yerionDistributor.h"

class EnemyLauncher : public Animator
{
public:
    EnemyLauncher(EnemyDistributor* distributor, float launchRate, YerionDistributor* yerionDistributor, float yerionLaunchRate);

    // Inherited from Animator
    void animate(float deltaTime);

private:
    EnemyDistributor* distributor;
    float launchRate;
    YerionDistributor* yerionDistributor;
    float yerionLaunchRate;
};

#endif // ENEMYLAUNCHER_H_INCLUDED
