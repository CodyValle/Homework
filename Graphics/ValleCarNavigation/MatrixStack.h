#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H

#include <stack>
#include "Angel.h"

class MatrixStack
{
    public:
        MatrixStack();
        virtual ~MatrixStack();
        void loadIdentity();
        void pushMatrix(mat4 m);
        mat4 popMatrix();
    protected:
    private:
       std::stack<mat4> m;
};

#endif // MATRIXSTACK_H
