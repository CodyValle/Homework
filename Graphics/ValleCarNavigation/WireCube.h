#ifndef WIRECUBE_H
#define WIRECUBE_H

#include "Angel.h"

class WireCube
{
    public:
        WireCube();
        WireCube(const WireCube& other);
        WireCube& operator=(const WireCube& rhs);

        GLint vao;
        void createVAO(GLint _vao, GLint _program );
        virtual ~WireCube();
        vec4 points[48];
        vec4 colors[48];
        int numVertices;
        void draw();
    protected:
    private:
        void quad( int a, int b, int c, int d);
        vec4 vertices[8];
        int Index;
};

#endif // WIRECUBE_H
