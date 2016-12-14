#version 430 core

layout(location=0) in vec4 objCoords;
layout(location=1) in vec3 objNormal;

uniform mat4 modelViewMat;
uniform mat4 projMat;
uniform mat3 normalMat;

out vec4 frontColsExport, backColsExport;

struct Light
{
   vec4 ambCols;
   vec4 difCols;
   vec4 specCols;
   vec4 coords;
};
uniform Light light0;

uniform vec4 globAmb;

struct Material
{
   vec4 ambRefl;
   vec4 difRefl;
   vec4 specRefl;
   vec4 emitCols;
   float shininess;
};
uniform Material objFront, objBack;

vec3 normal, lightDirection, eyeDirection, halfway;
vec4 frontEmit, frontGlobAmb, frontAmb, frontDif, frontSpec,
     backEmit, backGlobAmb, backAmb, backDif, backSpec;

void main(void)
{
   normal = normalize(normalMat * objNormal * -1);
   lightDirection = normalize(vec3(light0.coords));
   eyeDirection = -1.0 * normalize(vec3(modelViewMat * objCoords));
   halfway = (length(lightDirection + eyeDirection) == 0.0) ?
             vec3(0.0) : (lightDirection + eyeDirection)/length(lightDirection + eyeDirection);

   frontEmit = objFront.emitCols;
   frontGlobAmb = globAmb * objFront.ambRefl;
   frontAmb = light0.ambCols * objFront.ambRefl;
   frontDif = max(dot(normal, lightDirection), 0.0) * light0.difCols * objFront.difRefl;
   frontSpec = pow(max(dot(normal, halfway), 0.0), objFront.shininess) *
               light0.specCols * objFront.specRefl;
   frontColsExport =  vec4(vec3(min(frontEmit + frontGlobAmb + frontAmb +
                           frontDif + frontSpec, vec4(1.0))), 1.0);

   normal = -1.0 * normal;
   backEmit = objBack.emitCols;
   backGlobAmb = globAmb * objBack.ambRefl;
   backAmb = light0.ambCols * objBack.ambRefl;
   backDif = max(dot(normal, lightDirection), 0.0) * light0.difCols * objBack.difRefl;
   backSpec = pow(max(dot(normal, halfway), 0.0), objBack.shininess) *
              light0.specCols * objBack.specRefl;
   backColsExport =  vec4(vec3(min(backEmit + backGlobAmb + backAmb +
                          backDif + backSpec, vec4(1.0))), 1.0);

   gl_Position = projMat * modelViewMat * objCoords;
}
