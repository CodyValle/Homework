#include "enemyLauncher.h"

EnemyLauncher::EnemyLauncher(EnemyDistributor* distributor, float launchRate, YerionDistributor* yerionDistributor, float yerionLaunchRate) :
    distributor(distributor),
    launchRate(launchRate),
    yerionDistributor(yerionDistributor),
    yerionLaunchRate(yerionLaunchRate)
{
}

// Inherited from Animator
void EnemyLauncher::animate(float deltaTime)
{
    static float curLaunchRate = 0;
    static float curYerionLaunchRate = 1500;

    curLaunchRate -= deltaTime;
    if (curLaunchRate < 0)
    {
        curLaunchRate = launchRate;
        if (launchRate > 150.)
            launchRate *= 0.95;

        int x = (std::rand()  % 50) - 25; // Limits to [-25, 25]
        int y = (std::rand()  % 50) - 25; // Limits to [-25, 25]
        distributor->launch(vec3(x, y, -250));
    }

    curYerionLaunchRate -= deltaTime;
    if (curYerionLaunchRate < 0)
    {
        curYerionLaunchRate  = yerionLaunchRate;

        yerionDistributor->launch(vec3(0, 0, -250));
    }
}
