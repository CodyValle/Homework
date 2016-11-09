#include "sphere.h"

#include <glm/gtc/matrix_transform.hpp>

Sphere::Sphere(const unsigned pid, float const color[4], Hemisphere* hemi) :
    Drawable(pid, color),
    hemisphere(hemi)
{
}

Sphere::~Sphere()
{
    delete hemisphere;
}

void Sphere::draw(mat4 modelViewMat)
{
    // Draw the top
    hemisphere->draw(modelViewMat);
    // Draw the bottom
    modelViewMat = scale(modelViewMat, vec3(1., -1., 1.));
    hemisphere->draw(modelViewMat);
}
