#ifndef KEYINPUT_H_INCLUDED
#define KEYINPUT_H_INCLUDED

class KeyInput
{
public:
    KeyInput() :
        upBtn(false),
        downBtn(false),
        leftBtn(false),
        rightBtn(false),
        fireBtn(false)
    {
    }

    ~KeyInput()
    {
    }

    void keyUp(unsigned char key)
    {
        switch(key)
        {
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

    void keyDown(unsigned char key)
    {
        switch(key)
        {
        case 27:
            exit(0);

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

    bool fire()
    {
        if (fireBtn)
        {
            fireBtn = false;
            return true;
        }
        return false;
    }

    inline bool up()
        { return upBtn; }

    inline bool down()
        { return downBtn; }

    inline bool left()
        { return leftBtn; }

    inline bool right()
        { return rightBtn; }

private:
    bool upBtn;
    bool downBtn;
    bool leftBtn;
    bool rightBtn;
    bool fireBtn;
};

#endif // KEYINPUT_H_INCLUDED
