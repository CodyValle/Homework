#ifndef TRANSFORM_H_INCLUDED
#define TRANSFORM_H_INCLUDED

#include <glm/glm.hpp>

class Transform
{
public:
    Transform() :
        trans(glm::vec3(0., 0., 0.)),
        rot(glm::vec3(0., 0., 0.)),
        scal(glm::vec3(1., 1., 1.))
    {
    }

    Transform(glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale) :
        trans(translate),
        rot(rotate),
        scal(scale)
    {
    }

    inline glm::vec3 getTranslate()
        { return trans; }

    inline float getXAngle()
        { return rot[0] / 180. * PI; }

    inline float getYAngle()
        { return rot[1] / 180. * PI; }

    inline float getZAngle()
        { return rot[2] / 180. * PI; }

    inline glm::vec3 getScale()
        { return scal; }

    inline void translate(glm::vec3 t)
        { trans += t; }

    inline void translate(float x, float y, float z)
        { trans += glm::vec3(x, y, z); }

    inline void rotate(glm::vec3 r)
        { rot += r; }

    inline void rotate(float x, float y, float z)
        { rot += glm::vec3(x, y, z); }

    inline void scale(glm::vec3 s)
        {
            scal[0] *= s[0];
            scal[1] *= s[1];
            scal[2] *= s[2];
        }

    inline void scale(float x, float y, float z)
        {
            scal[0] *= x;
            scal[1] *= y;
            scal[2] *= z;
        }

private:
    glm::vec3 trans;
    glm::vec3 rot;
    glm::vec3 scal;
};

#endif // TRANSFORM_H_INCLUDED
