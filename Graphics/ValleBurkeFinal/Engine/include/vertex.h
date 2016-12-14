#ifndef VERTEX_H
#define VERTEX_H

/**
 * Binds real world coords and normal vectors
**/

#include <glm/glm.hpp>

struct Vertex
{
   glm::vec4 coords;
   glm::vec3 normal;
};

#endif
