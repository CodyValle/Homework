// A car made from cubes, cylinders and disks.
// The car can move forward or backward (see controls in main) causing the wheels to also turn.
#include "Car.h"
#include "Shapes.h"
#include "Globals.h"

Car::Car(): wheelAngle(0), wheelRadius(1), xLoc(0), cHeight(2), cLength(10), cWidth(4)
{
    //ctor
}

Car::~Car()
{
    //dtor
}

void
Car::drawWheel(mat4& mv)
{
    mvMatrixStack.pushMatrix(mv);
    // assume cylindar has height =1, radius = .5, is centered at the origin, aligned with y axis
    mv = mv*Translate(0,wheelRadius,0);
    mv = mv*RotateX(90);     // rotate about x axis
    mv = mv*Scale(2*wheelRadius,.5,2*wheelRadius);
    vec4 color(.5,.5,.5,1);
    mv = mv*RotateY(wheelAngle);
    glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );
    shapes.drawCylinder(color);
    mv = mv*Translate(0,.5,0);
    glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );
    shapes.drawDisk(color);
    mv = mv*Translate(0,-1,0);
    glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );
    shapes.drawDisk(color);
    mv = mvMatrixStack.popMatrix();
}

void
Car::drawBody(mat4& mv)
{

    // bottom part of car body
    mv = mv*Translate(xLoc,0,0);
    mvMatrixStack.pushMatrix(mv);
    vec4 color(0,0,1,1);
    mv = mv*Translate(0,cHeight/2 + wheelRadius,0);
    mv = mv*Scale(cLength,cHeight,cWidth);
    glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );
    shapes.drawCube(color);
    mv = mvMatrixStack.popMatrix();

    mvMatrixStack.pushMatrix(mv);
    // top part of car body
    mv = mv*Translate(0,1.5*cHeight + wheelRadius,0);
    mv = mv*Scale(cLength/2,cHeight,cWidth);
    glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );
    color = vec4(.5,.5,1,1);
    shapes.drawCube(color);

    mv = mvMatrixStack.popMatrix();
}

void
Car::drawCar(mat4& mv)
{
    // save the current modelview matrix on the stack
    mvMatrixStack.pushMatrix(mv);

    // Draw car body
    glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );
    drawBody(mv);

    // Draw the 4 wheels
    mvMatrixStack.pushMatrix(mv);
    mv = mv*Translate(3.5,0,2.4);
    glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );
    drawWheel(mv);
    mv = mvMatrixStack.popMatrix();

    mvMatrixStack.pushMatrix(mv);
    mv = mv*Translate(-3.5,0,2.4);
    glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );
    drawWheel(mv);
    mv = mvMatrixStack.popMatrix();

    mvMatrixStack.pushMatrix(mv);
    mv = mv*Translate(-3.5,0,-2.4);
    glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );
    drawWheel(mv);
    mv = mvMatrixStack.popMatrix();

    mvMatrixStack.pushMatrix(mv);
    mv = mv*Translate(3.5,0,-2.4);
    glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );
    drawWheel(mv);
    mv = mvMatrixStack.popMatrix();
    mv = mvMatrixStack.popMatrix();
}
