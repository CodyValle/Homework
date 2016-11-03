#ifndef SPHERE_H_INCLUDED
#define SPHERE_H_INCLUDED

#  include <GL/glew.h>
#  include <GL/freeglut.h>
#ifdef __APPLE__
#  include <OpenGL/glext.h>
#else
#  include <GL/glext.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "hemisphere.h"
#include "transform.h"

using namespace std;
using namespace glm;

class Sphere
{
public:
    Hemisphere hemiTop;
    Hemisphere hemiBtm;

    Sphere(Transform transform) :
        hemiTop(Hemisphere(transform)),
        hemiBtm(Hemisphere(transform))
    {
    }

    void setup(const unsigned int programId)
    {
        hemiTop.setup(programId);
        hemiBtm.setup(programId);
    }

    void draw()
    {
        hemiTop.draw();
        hemiBtm.draw();
    }
};

#endif // SPHERE_H_INCLUDED
