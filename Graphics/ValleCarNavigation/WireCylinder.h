#ifndef WIRECYLINDER_H
#define WIRECYLINDER_H

#include "Angel.h"

class WireCylinder
{
    public:
        WireCylinder(int slices = 12,float radius=1, float height=1);
        virtual ~WireCylinder();
        WireCylinder(const WireCylinder& other);
        WireCylinder& operator=(const WireCylinder& other);

        GLint vao;
        void createVAO(GLint _vao, GLint _program );
        vec4 *points;
        vec4 *colors;
        int numVertices;
        void draw();
    protected:
    private:
};

#endif // WIRECYLINDER_H
