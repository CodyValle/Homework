#include "hemisphere.h"

#include <cmath>
#include <iostream>

#include <GL/glew.h>

#include <glm/gtc/type_ptr.hpp>


using namespace std;

Hemisphere::Hemisphere(const unsigned pid, float const color[4], float r, unsigned longs, unsigned lats) :
    Drawable(pid, color),
    radius(r),
    longitudes(longs),
    latitudes(lats)
{
    vertices = new Vertex[(longs + 1) * (lats + 1)];
    indices = new unsigned[lats * (2*(longs+1))];
    counts = new int[lats];
    offsets = new void*[lats];

    // Initialize hemisphere.
    fillHemVertexArray(vertices, radius, latitudes, longitudes);
    fillHemIndices(indices, latitudes, longitudes);
    fillHemCounts(counts, latitudes, longitudes);
    fillHemOffsets(offsets, latitudes, longitudes);

    // ...and associate data with vertex shader.
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * (longitudes + 1) * (latitudes + 1), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * (latitudes * 2*(longitudes+1)), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), 0);
    glEnableVertexAttribArray(0);

    // Wireframe for now
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

Hemisphere::~Hemisphere()
{
    delete[] vertices;
    delete[] indices;
    delete[] counts;
    delete[] offsets;
}

void Hemisphere::draw(mat4 modelViewMat)
{
    // Change color uniform locations and set values.
    glUniform4fv(glGetUniformLocation(programId, "colorIn"), 1, objectColor);

    // Calculate and update modelview matrix.
    glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, value_ptr(modelViewMat));

    // Draw this hemisphere
    glUniform1ui(objectLoc, 0); // Update object name.
    glBindVertexArray(vao);
    glMultiDrawElements(GL_TRIANGLE_STRIP, counts, GL_UNSIGNED_INT, (const void **)offsets, latitudes);
}
