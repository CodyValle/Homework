#ifndef HEMISPHERE_H
#define HEMISPHERE_H

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

#include "vertex.h"
#include "transform.h"

using namespace glm;

#define HEM_RADIUS 2.0 // Hemisphere radius.
#define HEM_LONGS 10 // Number of longitudinal slices.
#define HEM_LATS 5 // Number of latitudinal slices.
#define HEM_COLORS 0.0, 0.0, 1.0, 1.0 // Hemisphere colors.

static const double PI = 3.14159265358979323;

class Hemisphere
{
public:
    Hemisphere(Transform t) :
        transform(t)
    {
        hemColors = vec4(HEM_COLORS);
        modelViewMat = mat4(1.0);
        projMat = mat4(1.0);
    }

    // Hemisphere data.
    Vertex hemVertices[(HEM_LONGS + 1) * (HEM_LATS + 1)];
    unsigned int hemIndices[HEM_LATS][2*(HEM_LONGS+1)];
    int hemCounts[HEM_LATS];
    void* hemOffsets[HEM_LATS];
    vec4 hemColors;

    enum object {HEMISPHERE}; // VAO ids.
    enum buffer {HEM_VERTICES, HEM_INDICES}; // VBO ids.

    mat4 modelViewMat;
    mat4 projMat;

    unsigned int
        modelViewMatLoc,
        projMatLoc,
        objectLoc,
        hemColorLoc,
        buffer[4],
        vao[2];

    // Fill the vertex array with co-ordinates of the sample points.
    void fillHemVertexArray(Vertex hemVertices[(HEM_LONGS + 1) * (HEM_LATS + 1)])
    {
       int i, j, k;

       k = 0;
       for (j = 0; j <= HEM_LATS; j++)
          for (i = 0; i <= HEM_LONGS; i++)
          {
             hemVertices[k].coords.x = HEM_RADIUS * cos( (float)j/HEM_LATS * PI/2.0 ) * cos( 2.0 * (float)i/HEM_LONGS * PI );
             hemVertices[k].coords.y = HEM_RADIUS * sin( (float)j/HEM_LATS * PI/2.0 );
             hemVertices[k].coords.z = HEM_RADIUS * cos( (float)j/HEM_LATS * PI/2.0 ) * sin( 2.0 * (float)i/HEM_LONGS * PI );
             hemVertices[k].coords.w = 1.0;
             k++;
          }
    }

    // Fill the array of index arrays.
    void fillHemIndices(unsigned int hemIndices[HEM_LATS][2*(HEM_LONGS+1)])
    {
       int i, j;
       for(j = 0; j < HEM_LATS; j++)
       {
          for (i = 0; i <= HEM_LONGS; i++)
          {
             hemIndices[j][2*i] = (j+1)*(HEM_LONGS + 1) + i;
             hemIndices[j][2*i+1] = j*(HEM_LONGS + 1) + i;
          }
       }
    }

    // Fill the array of counts.
    void fillHemCounts(int hemCounts[HEM_LATS])
    {
       int j;
       for(j = 0; j < HEM_LATS; j++) hemCounts[j] = 2*(HEM_LONGS + 1);
    }

    // Fill the array of buffer offsets.
    void fillHemOffsets(void* hemOffsets[HEM_LATS])
    {
       int j;
       for(j = 0; j < HEM_LATS; j++) hemOffsets[j] = (GLvoid*)(2*(HEM_LONGS+1)*j*sizeof(unsigned int));
    }

    // Initialize the hemisphere.
    void fillHemisphere(Vertex hemVertices[(HEM_LONGS + 1) * (HEM_LATS + 1)],
                 unsigned int hemIndices[HEM_LATS][2*(HEM_LONGS+1)],
                 int hemCounts[HEM_LATS],
                 void* hemOffsets[HEM_LATS])
    {
       fillHemVertexArray(hemVertices);
       fillHemIndices(hemIndices);
       fillHemCounts(hemCounts);
       fillHemOffsets(hemOffsets);
    }

    void setup(const unsigned int programId)
    {
        //hemiTop.fillHemisphere();

        // Initialize hemishpere.
        fillHemisphere(hemVertices, hemIndices, hemCounts, hemOffsets);

        // Create VAOs and VBOs...
        glGenVertexArrays(1, vao);
        glGenBuffers(2, buffer);

        // ...and associate data with vertex shader.
        glBindVertexArray(vao[HEMISPHERE]);
        glBindBuffer(GL_ARRAY_BUFFER, buffer[HEM_VERTICES]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(hemVertices), hemVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[HEM_INDICES]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(hemIndices), hemIndices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(hemVertices[0]), 0);
        glEnableVertexAttribArray(0);

        // Obtain projection matrix uniform location and set value.
        projMatLoc = glGetUniformLocation(programId,"projMat");
        projMat = frustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
        glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));

        // Obtain color uniform locations and set values.
        hemColorLoc = glGetUniformLocation(programId, "hemColor");
        glUniform4fv(hemColorLoc, 1, &hemColors[0]);

        // Obtain modelview matrix uniform and object uniform locations.
        modelViewMatLoc = glGetUniformLocation(programId,"modelViewMat");
        objectLoc = glGetUniformLocation(programId, "object");

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void draw()
    {
        // Calculate and update modelview matrix.
        modelViewMat = mat4(1.0);
        modelViewMat = translate(modelViewMat, transform.getTranslate());
        modelViewMat = rotate(modelViewMat, transform.getAngle(), transform.getRotateAxis());
        modelViewMat = scale(modelViewMat, transform.getScale());
        glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

        // Draw ball as two hemispheres.
        glUniform1ui(objectLoc, HEMISPHERE); // Update object name.
        glBindVertexArray(vao[HEMISPHERE]);
        glMultiDrawElements(GL_TRIANGLE_STRIP, hemCounts, GL_UNSIGNED_INT, (const void **)hemOffsets, HEM_LATS);
    }
private:
    Transform transform;
};

#endif
