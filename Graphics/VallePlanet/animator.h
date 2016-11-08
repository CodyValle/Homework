#ifndef ANIMATOR_H_INCLUDED
#define ANIMATOR_H_INCLUDED

class Animator
{
public:
    Animator()
    {}

    ~Animator()
    {}

    virtual void animate() = 0;

    void setTranslation(glm::vec3 trans)
        { transform.setTranslate(trans); }

protected:
    Transform transform;
};

#endif // ANIMATOR_H_INCLUDED
