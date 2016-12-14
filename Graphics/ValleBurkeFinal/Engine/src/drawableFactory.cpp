#include "drawableFactory.h"
#include "shader.h"

#include <glm/glm.hpp>

#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

DrawableFactory::DrawableFactory() :
    programId(0)
{
    projMat = frustum(-5.0, 5.0, -5.0, 5.0, 5.0, 300.0);
}
