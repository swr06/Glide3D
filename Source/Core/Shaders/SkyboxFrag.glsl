#version 330 core

in vec3 v_TexCoords;

uniform samplerCube u_Skybox;

out vec4 o_Color;

void main()
{    
    o_Color = texture(u_Skybox, v_TexCoords);
}