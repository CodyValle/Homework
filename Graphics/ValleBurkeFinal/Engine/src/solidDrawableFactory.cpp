#include "solidDrawableFactory.h"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "solidPlane.h"
#include "light.h"
#include "shader.h"

// Light properties.
static const Light light0 =
{
    vec4(0.0, 0.0, 0.0, 1.0),
    vec4(1.0, 1.0, 1.0, 1.0),
    vec4(1.0, 1.0, 1.0, 1.0),
    vec4(0.0, -30.0, -45.0, 1.0)
};

// Global ambient.
static const vec4 globAmb = vec4(.2, 0.2, .2, 1.0);

SolidDrawableFactory::SolidDrawableFactory() :
    DrawableFactory()
{
    // Create shader program executable.
    programId = glCreateProgram();
    glAttachShader(programId, setShader("vertex", "Engine\\vertex.glsl"));
    glAttachShader(programId, setShader("fragment", "Engine\\fragment.glsl"));
    glLinkProgram(programId);
    glUseProgram(programId);

    // Obtain light property uniform locations and set values.
    glUniform4fv(glGetUniformLocation(programId, "light0.ambCols"), 1, &light0.ambCols[0]);
    glUniform4fv(glGetUniformLocation(programId, "light0.difCols"), 1, &light0.difCols[0]);
    glUniform4fv(glGetUniformLocation(programId, "light0.specCols"), 1, &light0.specCols[0]);
    glUniform4fv(glGetUniformLocation(programId, "light0.coords"), 1, &light0.coords[0]);

    // Obtain global ambient uniform location and set value.
    glUniform4fv(glGetUniformLocation(programId, "globAmb"), 1, &globAmb[0]);

    // Set projection matrix uniform value.
    glUniformMatrix4fv(glGetUniformLocation(programId,"projMat"), 1, GL_FALSE, value_ptr(projMat));
}

Sphere* SolidDrawableFactory::makeSphere(unsigned longs, unsigned lats)
{
    return new Sphere(programId, longs, lats);
}

Hemisphere* SolidDrawableFactory::makeHemisphere(float radius, unsigned longs, unsigned lats)
{
    return new Hemisphere(programId, radius, longs, lats);
}

Plane* SolidDrawableFactory::makePlane(unsigned longs, unsigned lats)
{
    return new SolidPlane(programId, longs, lats);
}

Plane* SolidDrawableFactory::makePlane(std::string file)
{
    return new SolidPlane(programId, 30, 30);
}
