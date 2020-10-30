#version 440 core

in vec2 v_TexCoords;
in vec3 v_FragPosition;
in vec3 v_Normal;
in mat3 v_TBNMatrix;

layout (location = 0) out vec3 o_Position;
layout (location = 1) out vec3 o_Normal;
layout (location = 2) out vec4 o_Color;

// PBR Components (Metallic, Roughness and AO)
layout (location = 3) out vec3 o_PBRComponents;

uniform vec4 u_Color;

uniform sampler2D u_AlbedoMap;
uniform sampler2D u_SpecularMap; 
uniform sampler2D u_NormalMap;

// PBR Textures
uniform sampler2D u_Metalness;
uniform sampler2D u_Roughness;
uniform sampler2D u_AO;

// For reflections
uniform samplerCube u_EnvironmentMap;
uniform vec3 u_ViewerPosition;
uniform vec3 u_Reflectance = vec3(0.0f);
uniform int u_HasReflections = 0;
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

	if (u_HasReflections == 1)
	{
		vec4 reflect_color = vec4(texture(u_EnvironmentMap, 
		reflect(normalize(v_FragPosition - u_ViewerPosition), o_Normal)).rgb, 1.0);

		o_Color.r = mix(o_Color, reflect_color, u_Reflectance.r).r;
		o_Color.g = mix(o_Color, reflect_color, u_Reflectance.g).g;
		o_Color.b = mix(o_Color, reflect_color, u_Reflectance.b).b;
	}

	o_PBRComponents.r = texture(u_Metalness, v_TexCoords).r;
	o_PBRComponents.g = texture(u_Roughness, v_TexCoords).r;
	o_PBRComponents.b = texture(u_AO, v_TexCoords).r;
}



