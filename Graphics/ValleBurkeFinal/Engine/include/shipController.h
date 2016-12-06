#ifndef SHIPCONTROLLER_H_INCLUDED
#define SHIPCONTROLLER_H_INCLUDED

#include "keyInput.h"
#include "bulletController.h"

class ShipController : public Animator
{
public:
    // Constructor
    ShipController(Transform& t, KeyInput* keys, BulletController* bulletController) :
        object(t),
        keys(keys),
        bullets(bulletController)
    {
        bulletColor[0] = 0.;
        bulletColor[1] = 0.;
        bulletColor[2] = 1.;
        bulletColor[3] = 1.;
    }

    // Destructor
    ~ShipController()
    {

    }

    // Inherited from Animator
    void animate(float deltaTime)
    {
        glm::vec3 translate = object.getTranslate();

        // Translate according to input
        if (keys->up())
            translate[1] += .2 * deltaTime;

        if (keys->down())
            translate[1] -= .2 * deltaTime;

        if (keys->left())
            translate[0] -= .2 * deltaTime;

        if (keys->right())
            translate[0] += .2 * deltaTime;

        // Limit movement
        if (translate[0] > 25.) translate[0] = 25.;
        else if (translate[0] < -25.) translate[0] = -25.;
        if (translate[1] > 25.) translate[1] = 25.;
        else if (translate[1] < -25.) translate[1] = -25.;

        object.setTranslate(translate); // Update the ship location

        if (keys->fire())
            bullets->shoot(translate);
    }

protected:
    Transform& object;
    KeyInput* keys;
    BulletController* bullets;

    float bulletColor[4];
};

#endif // SHIPCONTROLLER_H_INCLUDED
