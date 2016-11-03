#ifndef TRANSFORM_H_INCLUDED
#define TRANSFORM_H_INCLUDED

#include <glm/glm.hpp>

class Transform
{
public:
    Transform() :
        trans(glm::vec3(0., 0., 0.)),
        rot(glm::vec4(0., 0., 0., 0.)),
        scale(glm::vec3(1., 1., 1.))
    {
    }

    Transform(glm::vec3 translate, glm::vec4 rotate, glm::vec3 scale) :
        trans(translate),
        rot(rotate),
        scale(scale)
    {
    }

    inline glm::vec3 getTranslate()
        { return trans; }

    inline double getAngle()
        { return rot.get(0); }

    inline glm::vec4 getRotateAxis()
        { return rot; }

    inline glm::vec3 getScale()
        { return scale; }

private:
    glm::vec3 trans;
    glm::vec4 rot;
    glm::vec3 scale;
};

#endif // TRANSFORM_H_INCLUDED
