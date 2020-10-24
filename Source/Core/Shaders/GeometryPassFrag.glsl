#version 440 core

in vec2 v_TexCoords;
in vec3 v_FragPosition;
in vec3 v_Normal;
in mat3 v_TBNMatrix;

layout (location = 0) out vec3 o_Position;
layout (location = 1) out vec3 o_Normal;
layout (location = 2) out vec4 o_Color;

uniform vec4 u_Color;

uniform sampler2D u_AlbedoMap;
uniform sampler2D u_SpecularMap; 
uniform sampler2D u_NormalMap;

uniform int u_HasAlbedoMap = 0;
uniform int u_HasNormalMap = 0;

void main()
{
	if (u_HasAlbedoMap == 1)
	{
		o_Color = texture(u_AlbedoMap, v_TexCoords);
	}

	else
	{
		o_Color = u_Color.rgba;
	}

	if (u_HasNormalMap == 1)
	{
		o_Normal = (texture(u_NormalMap, v_TexCoords)).rgb;
		o_Normal = o_Normal * 2.0f - 1.0f;
		o_Normal = v_TBNMatrix * o_Normal; 
		o_Normal = normalize(o_Normal);
	}

	else
	{
		o_Normal = normalize(v_Normal);
	}

	o_Position = v_FragPosition;
}



