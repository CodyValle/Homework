#ifndef WIREDISK_H
#define WIREDISK_H

#include "Angel.h"

class WireDisk
{
    public:
        WireDisk(int slices = 12,float radius=1);
        virtual ~WireDisk();
        WireDisk(const WireDisk& other);
        WireDisk& operator=(const WireDisk& other);

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

#endif // WIREDISK_H
