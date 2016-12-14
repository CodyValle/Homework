#version 430 core

in vec2 texCoordsExport;

uniform sampler2D Tex;

out vec4 colorsOut;

vec4 TexColor;

void main(void)
{
   TexColor = texture(Tex, texCoordsExport);

   colorsOut = TexColor;
}
