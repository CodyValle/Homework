#version 430 core

layout(location=0) in vec4 Coords;
layout(location=1) in vec2 TexCoords;

uniform mat4 modelViewMat;
uniform mat4 projMat;

out vec2 texCoordsExport;

vec4 coords;

void main(void)
{
  coords = Coords;
  texCoordsExport = TexCoords;

   gl_Position = projMat * modelViewMat * coords;
}
