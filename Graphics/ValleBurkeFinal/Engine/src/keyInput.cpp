#include "keyInput.h"

KeyInput::KeyInput() :
    upBtn(false),
    downBtn(false),
    leftBtn(false),
    rightBtn(false),
    fireBtn(false)
{
}

KeyInput::~KeyInput()
{
}

void KeyInput::keyUp(unsigned char key)
{
    switch(key)
    {
    case ' ':
        fireBtn = false;
        break;

    case 'w':
        upBtn = false;
        break;

    case 'a':
        leftBtn = false;
        break;

    case 's':
        downBtn = false;
        break;

    case 'd':
        rightBtn = false;
        break;

    default:
        break;
    }
}

void KeyInput::keyDown(unsigned char key)
{
    switch(key)
    {
    case ' ':
        fireBtn = true;
        break;

    case 'w':
        upBtn = true;
        break;

    case 'a':
        leftBtn = true;
        break;

    case 's':
        downBtn = true;
        break;

    case 'd':
        rightBtn = true;
        break;

    default:
        break;
    }
}

const float fireRate = 12.0;

bool KeyInput::fire(float deltaTime)
{
    static float canFire = fireRate;
    if (fireBtn)
    {
        canFire -= deltaTime;
        if (canFire < 0)
        {
            canFire = fireRate;
            return true;
        }
        else return false;
    }
    canFire = fireRate;
    return false;
}
