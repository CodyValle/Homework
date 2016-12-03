#version 150

in  vec4 vPosition;
in  vec4 vColor;
in  vec4 vNormal;
out vec4 Color;

uniform mat4 model_view;
uniform mat4 projection;
uniform vec4 model_color;

void main()
{
   Color = model_color;
  // Color = vColor;
    gl_Position = projection*model_view * vPosition/vPosition.w;
}
