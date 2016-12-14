#ifndef KEYINPUT_H_INCLUDED
#define KEYINPUT_H_INCLUDED

/**
 * Tracks the state of the keyboard
**/

class KeyInput
{
public:
    // Constructor
    KeyInput();

    // Destructor
    ~KeyInput();

    // Report a key has been lifted
    void keyUp(unsigned char key);

    // Report a key has been pressed
    void keyDown(unsigned char key);

    // Shoot a bullet
    bool fire(float deltaTime);

    // Getters for the button states
    inline bool up()
        { return upBtn; }

    inline bool down()
        { return downBtn; }

    inline bool left()
        { return leftBtn; }

    inline bool right()
        { return rightBtn; }

private:
    // Tracks whether as button is up (false) or down (true)
    bool upBtn;
    bool downBtn;
    bool leftBtn;
    bool rightBtn;
    bool fireBtn;
};

#endif // KEYINPUT_H_INCLUDED
