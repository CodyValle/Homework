#version 100 compatibility
//#version 150

in  vec4 vPosition;
in  vec4 vColor;
in  vec4 vNormal;
varying vec4 Color;

uniform mat4 model_view;
uniform mat4 projection;

void main()
{
    Color = vColor;
    //gl_Position = projection*model_view * gl_Vertex;
    gl_Position = projection*model_view * vPosition/vPosition.w;
}
