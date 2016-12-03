#ifndef CYLINDER_H
#define CYLINDER_H

#include "Angel.h"

class Cylinder
{
    public:
        Cylinder(int slices = 12,float radius=1, float height=1);
        virtual ~Cylinder();
        Cylinder(const Cylinder& other);
        Cylinder& operator=(const Cylinder& other);

        GLint vao;
        void createVAO(GLint _vao, GLint _program );
        vec4 *points;
        vec4 *colors;
        vec4 *normals;
        int numVertices;
        void draw();
    protected:
    private:
};

#endif // CYLINDER_H
