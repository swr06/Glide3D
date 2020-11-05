#version 330 core

vec3 tonemapFilmic(vec3 x) 
{
  vec3 X = max(vec3(0.0), x - 0.004);
  vec3 result = (X * (6.2 * X + 0.5)) / (X * (6.2 * X + 1.7) + 0.06);
  return pow(result, vec3(2.2));
}

in vec2 v_TexCoords;
layout(location = 0) out vec4 o_Color;

uniform sampler2D u_FramebufferTexture;
uniform float u_Exposure = 1.0f;

void main()
{
    vec3 HDR = texture(u_FramebufferTexture, v_TexCoords).rgb;
    o_Color = vec4(pow(tonemapFilmic(HDR.rgb), vec3(1.0 / 2.2)), 1.0);
}
