#include "MatrixStack.h"


MatrixStack::MatrixStack()
{
   // mat4 id(1);
    m.push(mat4(1));
}

MatrixStack::~MatrixStack()
{
    //dtor
}

void
MatrixStack::loadIdentity() {
     // clear stack
    while ( ! m.empty() )
    {
        m.pop();
    }
    m.push(mat4(1));
}

void
MatrixStack::pushMatrix(mat4 mm){
    m.push(mm);
}

mat4
MatrixStack::popMatrix(){
    mat4 t = m.top();
    m.pop();
    return t;
}
