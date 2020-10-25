#version 330 core

in vec2 v_TexCoords;
layout(location = 0) out vec4 o_Color;

uniform sampler2D u_FramebufferTexture;
uniform float u_Exposure = 1.0f;
 
void main()
{
    // Simple reinhard tone mapping with exposure control 
    vec3 HDR = texture(u_FramebufferTexture, v_TexCoords).rgb;
    vec3 MappedColor = vec3(1.0) - exp(-HDR * u_Exposure);
    MappedColor = pow(MappedColor, vec3(1.0f / 2.2f));
  
    o_Color = vec4(MappedColor, 1.0);
}