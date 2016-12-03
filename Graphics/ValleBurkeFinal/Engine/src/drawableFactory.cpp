#include "drawableFactory.h"
#include "shader.h"

#include <glm/glm.hpp>

#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

DrawableFactory::DrawableFactory() :
    programId(0)
{
    // Create shader program executable.
    unsigned vertexShaderId = setShader("vertex", "Engine\\vertex.glsl");
    unsigned fragmentShaderId = setShader("fragment", "Engine\\fragment.glsl");
    programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    glLinkProgram(programId);
    glUseProgram(programId);

    // Obtain projection matrix uniform location and set value.
    unsigned projMatLoc = glGetUniformLocation(programId,"projMat");
    mat4 projMat = frustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
    glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));
}

Sphere* DrawableFactory::makeSphere(float radius, unsigned longs, unsigned lats)
{
    return new Sphere(programId, makeHemisphere(radius, longs, lats));
}

Sphere* DrawableFactory::makeSphere(float const col[4], float radius, unsigned longs, unsigned lats)
{
    return new Sphere(programId, col, makeHemisphere(col, radius, longs, lats));
}

Hemisphere* DrawableFactory::makeHemisphere(float radius, unsigned longs, unsigned lats)
{
    return new Hemisphere(programId, radius, longs, lats);
}

Hemisphere* DrawableFactory::makeHemisphere(float const col[4], float radius, unsigned longs, unsigned lats)
{
    return new Hemisphere(programId, col, radius, longs, lats);
}
