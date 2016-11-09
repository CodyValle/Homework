#include "transform.h"

Transform::Transform() :
    trans(glm::vec3(0., 0., 0.)),
    rot(glm::vec3(0., 0., 0.)),
    scal(glm::vec3(1., 1., 1.))
{
}

Transform::Transform(glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale) :
    trans(translate),
    rot(rotate),
    scal(scale)
{
}
