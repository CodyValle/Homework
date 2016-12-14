#include "texturedDrawableFactory.h"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "texturedPlane.h"
#include "shader.h"

TexturedDrawableFactory::TexturedDrawableFactory() :
    DrawableFactory()
{
    // Create shader program executable.
    programId = glCreateProgram();
    glAttachShader(programId, setShader("vertex", "Engine\\vertexTexture.glsl"));
    glAttachShader(programId, setShader("fragment", "Engine\\fragmentTexture.glsl"));
    glLinkProgram(programId);
    glUseProgram(programId);

    // Allow alpha blending
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set projection matrix uniform value.
    glUniformMatrix4fv(glGetUniformLocation(programId,"projMat"), 1, GL_FALSE, value_ptr(projMat));
}

Sphere* TexturedDrawableFactory::makeSphere(unsigned longs, unsigned lats)
{
    return 0;
}

Hemisphere* TexturedDrawableFactory::makeHemisphere(float radius, unsigned longs, unsigned lats)
{
    return 0;
}

Plane* TexturedDrawableFactory::makePlane(unsigned longs, unsigned lats)
{
    return new TexturedPlane(programId, std::string("raw.bmp"));
}

Plane* TexturedDrawableFactory::makePlane(std::string file)
{
    return new TexturedPlane(programId, file);
}
