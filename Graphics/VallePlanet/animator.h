#ifndef ANIMATOR_H_INCLUDED
#define ANIMATOR_H_INCLUDED

class Animator
{
public:
    void animate()
    {
        transform.translate(animation.getTranslate());
        transform.rotate(animation.getXAngle(), animation.getYAngle(), animation.getZAngle());
        transform.scale(animation.getScale());
    }

    inline Transform& getAnimation()
        { return animation; }

protected:
    friend class Node;
    Animator(Transform& t) :
        transform(t)
    {
    }

    Transform& transform;
    Transform animation;
};

#endif // ANIMATOR_H_INCLUDED
