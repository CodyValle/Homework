#ifndef SHIPCONTROLLER_H_INCLUDED
#define SHIPCONTROLLER_H_INCLUDED

/**
 * The ship controller controls the main ship given key inputs.
**/

#include "keyInput.h"
#include "bulletController.h"

class ShipController : public Animator
{
public:
    // Constructor
    ShipController(Transform& t, KeyInput* keys, BulletController* bulletController);

    // Destructor
    ~ShipController();

    // Inherited from Animator
    void animate(float deltaTime);

protected:
    Transform& object;
    KeyInput* keys;
    BulletController* bullets;

    float bulletColor[4];
};

#endif // SHIPCONTROLLER_H_INCLUDED
