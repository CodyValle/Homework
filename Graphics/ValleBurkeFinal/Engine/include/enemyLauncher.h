#ifndef ENEMYLAUNCHER_H_INCLUDED
#define ENEMYLAUNCHER_H_INCLUDED

#include <cstdlib>

#include "EnemyDistributor.h"

class EnemyLauncher : public Animator
{
public:
    EnemyLauncher(EnemyDistributor* distributor, float launchRate) :
        distributor(distributor),
        launchRate(launchRate),
        curLaunchRate(0)
    {}

    // Inherited from Animator
    void animate(float deltaTime)
    {
        curLaunchRate -= deltaTime;
        if (curLaunchRate > 0)
            return;

        curLaunchRate = launchRate;

        int x = (std::rand()  % 50) - 25; // Limits to [-25, 25]
        int y = (std::rand()  % 50) - 25; // Limits to [-25, 25]
        distributor->launchShip(vec3(x, y, -90));
    }

private:
    EnemyDistributor* distributor;
    float launchRate;
    float curLaunchRate;
};

#endif // ENEMYLAUNCHER_H_INCLUDED
