#ifndef DISK_H
#define DISK_H

#include "Angel.h"

class Disk
{
    public:
        Disk(int slices = 12,float radius=1);
        virtual ~Disk();
        Disk(const Disk& other);
        Disk& operator=(const Disk& other);

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

#endif // DISK_H
