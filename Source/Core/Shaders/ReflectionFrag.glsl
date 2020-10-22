#version 330 core

layout (location = 0) out vec3 o_Color;

in vec2 v_TexCoords;

uniform sampler2D u_AlbedoMap;
uniform vec3 u_DefaultColor = vec3(0.3f, 0.3f, 0.3f);
uniform int u_HasAlbedoMap = 1;

void main()
{
	if (u_HasAlbedoMap == 1)
	{
		o_Color = vec3(texture(u_AlbedoMap, v_TexCoords));
	}

	else
	{
		o_Color = u_DefaultColor;
	}
}
