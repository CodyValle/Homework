#ifndef HELIX_H
#define HELIX_H

#include "vertex.h"

const int HEL_SEGS = 400; // Number of line segments comprising the helix.

void fillHelVertexArray(Vertex helVertices[HEL_SEGS + 1]);

void fillHelix(Vertex helVertices[HEL_SEGS + 1]);

#endif
