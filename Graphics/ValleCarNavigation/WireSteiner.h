#ifndef WIRESTEINER_H
#define WIRESTEINER_H

#include "Angel.h"

class WireSteiner
{
    public:
        WireSteiner(int rDivisions = 10, int tDivisions=10);
        virtual ~WireSteiner();
        WireSteiner(const WireSteiner& other);
        WireSteiner& operator=(const WireSteiner& other);

        GLint vao;
        void createVAO(GLint _vao, GLint _program );
        vec4 *points;
        vec4 *colors;
        int numVertices;
        void draw();
    protected:
    private:
};

#endif // WIRESTEINER_H
