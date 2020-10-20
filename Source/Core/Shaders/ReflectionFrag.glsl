#version 330 core

layout (location = 0) out vec4 o_Color;

in vec2 v_TexCoords;

uniform sampler2D u_AlbedoMap;
uniform vec3 u_DefaultColor;
uniform int u_HasAlbedoMap = 1;

void main()
{
	/*if (u_HasAlbedoMap == 1)
	{
		o_Color = texture(u_AlbedoMap, v_TexCoords);
	}

	else
	{
		o_Color = vec4(u_DefaultColor, 1.0f);
	}*/

	o_Color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	
}
