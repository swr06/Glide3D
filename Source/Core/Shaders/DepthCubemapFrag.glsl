#version 330 core

in vec4 gs_FragPosition;

uniform float u_FarPlane;
uniform vec3 u_LightPosition;

void main() 
{
    float Distance = length(gs_FragPosition.xyz - u_LightPosition);
    Distance /= u_FarPlane;

    gl_FragDepth = Distance;
}
   