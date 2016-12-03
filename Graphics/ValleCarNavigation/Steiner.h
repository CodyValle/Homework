#ifndef STEINER_H
#define STEINER_H

#include "Angel.h"

class Steiner
{
    public:
        Steiner(int rDivisions = 10, int tDivisions=10);
        virtual ~Steiner();
        Steiner(const Steiner& other);
        Steiner& operator=(const Steiner& other);

        GLint vao;
        void createVAO(GLint _vao, GLint _program );
        vec4 *points;
        vec4 *colors;
        int numVertices;
        void draw();
    protected:
    private:
};

#endif // STEINER_H
