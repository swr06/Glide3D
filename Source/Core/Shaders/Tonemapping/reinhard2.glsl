#version 330 core

vec3 reinhard2(vec3 x) 
{
  const float L_white = 4.0;
  return (x * (1.0 + x / (L_white * L_white))) / (1.0 + x);
}

in vec2 v_TexCoords;
layout(location = 0) out vec4 o_Color;

uniform sampler2D u_FramebufferTexture;
uniform float u_Exposure = 1.0f;

void main()
{
    vec3 HDR = texture(u_FramebufferTexture, v_TexCoords).rgb;
    o_Color = vec4(pow(reinhard2(HDR.rgb), vec3(1.0 / 2.2)), 1.0);
}

