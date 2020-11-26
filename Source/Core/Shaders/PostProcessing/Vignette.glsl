#version 330 core

layout (location = 0) out vec3 o_Color;
in vec2 v_TexCoords;

uniform sampler2D u_Texture;

void main()
{
	o_Color = texture(u_Texture, v_TexCoords).rgb;

	o_Color *= 1.0f - (1.2f * distance(v_TexCoords, vec2(0.5f, 0.5f)));
}