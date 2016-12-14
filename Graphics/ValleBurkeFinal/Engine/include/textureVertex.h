#ifndef TEXTUREVERTEX_H_INCLUDED
#define TEXTUREVERTEX_H_INCLUDED

/**
 * Maps real world coords to texture coordinates.
**/

#include <glm/glm.hpp>

struct TextureVertex
{
    // Real world coords
    glm::vec4 coords;
    // texture space coords
    glm::vec2 texCoords;
};

#endif // TEXTUREVERTEX_H_INCLUDED
