#include "sphere.h"

#include <cmath>
#include <iostream>

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
#include <glm/gtc/matrix_inverse.hpp>

Sphere::Sphere(const unsigned pid, unsigned longs, unsigned lats) :
    Drawable(pid),
    longitudes(longs),
    latitudes(lats)
{
    vertices = new Vertex[(longitudes + 1) * (latitudes + 1)];
    indices = new unsigned[latitudes * 2*(longitudes+1)];
    counts = new int[latitudes];
    offsets = new void*[latitudes];

    // Initialize hemisphere.
    fillSphereVertexArray(vertices, latitudes, longitudes);
    fillSphereIndices(indices, latitudes, longitudes);
    fillSphereCounts(counts, latitudes, longitudes);
    fillSphereOffsets(offsets, latitudes, longitudes);

    // ...and associate data with vertex shader.
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * (longitudes + 1) * (latitudes + 1), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * (latitudes * 2*(longitudes+1)), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(vec4));
    glEnableVertexAttribArray(1);

    // Solid for now
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

Sphere::~Sphere()
{
}

void Sphere::draw(mat4 modelViewMat)
{
    glUseProgram(programId);

    // Change color uniform locations and set values.
    glUniform4fv(glGetUniformLocation(programId, "objFront.ambRefl"), 1, &objFront.ambRefl[0]);
    glUniform4fv(glGetUniformLocation(programId, "objFront.difRefl"), 1, &objFront.difRefl[0]);
    glUniform4fv(glGetUniformLocation(programId, "objFront.specRefl"), 1, &objFront.specRefl[0]);
    glUniform4fv(glGetUniformLocation(programId, "objFront.emitCols"), 1, &objFront.emitCols[0]);
    glUniform1f(glGetUniformLocation(programId, "objFront.shininess"), objFront.shininess);

    // Obtain back material property uniform locations and set values.
    glUniform4fv(glGetUniformLocation(programId, "objBack.ambRefl"), 1, &objFront.ambRefl[0]);
    glUniform4fv(glGetUniformLocation(programId, "objBack.difRefl"), 1, &objFront.difRefl[0]);
    glUniform4fv(glGetUniformLocation(programId, "objBack.specRefl"), 1, &objFront.specRefl[0]);
    glUniform4fv(glGetUniformLocation(programId, "objBack.emitCols"), 1, &objFront.emitCols[0]);
    glUniform1f(glGetUniformLocation(programId, "objBack.shininess"), objFront.shininess);

    // Calculate and update modelview matrix.
    glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, value_ptr(modelViewMat));

    // Calculate and update normal matrix.
    mat3 normalMat = transpose(inverse(mat3(modelViewMat)));
    glUniformMatrix3fv(glGetUniformLocation(programId, "normalMat"), 1, GL_FALSE, value_ptr(normalMat));

    // Draw this hemisphere
    glBindVertexArray(vao);
    glMultiDrawElements(GL_TRIANGLE_STRIP, counts, GL_UNSIGNED_INT, (const void **)offsets, latitudes);
}
