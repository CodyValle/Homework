#ifndef TRANSFORM_H_INCLUDED
#define TRANSFORM_H_INCLUDED

/**
 * A Transform tracks the translation, rotation, and scale of whatever object it is attached to.
**/

#include <glm/glm.hpp>

#include "globals.h"

using namespace glm;

class Transform
{
public:
    // Constructors
    Transform();
    Transform(vec3 translate, vec3 rotate, vec3 scale);

    // Translation getter, setter
    inline vec3 getTranslate()
        { return trans; }

    inline void setTranslate(vec3 loc)
        { trans = loc; }

    // Rotation getter, setter
    inline vec3 getRotate()
        { return rot; }

    inline float getXAngle()
        { return rot[0] / 180. * PI; }

    inline float getYAngle()
        { return rot[1] / 180. * PI; }

    inline float getZAngle()
        { return rot[2] / 180. * PI; }

    inline void setRotate(vec3 r)
        { rot = r; }

    // Scale getter
    inline vec3 getScale()
        { return scal; }

    inline void setScale(vec3 scale)
        { scal = scale; }

    // Translation modifiers
    inline void translate(vec3 t)
        { trans += t; }

    inline void translate(float x, float y, float z)
        { trans += vec3(x, y, z); }

    // Rotation modifiers
    inline void rotate(vec3 r)
        { rot += r; }

    inline void rotate(float x, float y, float z)
        { rot += vec3(x, y, z); }

    // Scale modifiers
    inline void scale(vec3 s)
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
    vec3 trans; // The current translation
    vec3 rot; // The current angles in respective axes
    vec3 scal; // The current scale
};

#endif // TRANSFORM_H_INCLUDED
