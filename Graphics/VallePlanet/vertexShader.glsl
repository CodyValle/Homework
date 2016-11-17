#version 430 core

in vec4 Coords;

uniform mat4 projMat;
uniform mat4 modelViewMat;

void main(void)
{
   gl_Position = projMat * modelViewMat * Coords;
}
