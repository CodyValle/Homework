#ifndef CAR_H
#define CAR_H

#include "mat.h"

class Car
{
    public:
        Car();
        virtual ~Car();
        void drawCar(mat4& mv);
        float wheelAngle;
        float wheelRadius;
        float xLoc;
        float cHeight;  // height of car body
        float cLength;  // length of body
        float cWidth;  // height of body
// Viewing transformation parameters
    protected:
    private:
        void drawBody(mat4& mv);
        void drawWheel(mat4& mv);
};

#endif // CAR_H
