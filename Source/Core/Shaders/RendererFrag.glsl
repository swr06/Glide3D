#version 330 core

/*
The renderer's fragment shader which includes a phong lighting model
*/

#define MAX_DIRECTIONAL_LIGHTS 4
#define MAX_POINT_LIGHTS 100

/*
Lighting is based on the phong lighting model.
*/

in vec2 v_TexCoords;
in vec3 v_FragPosition;
in vec3 v_Normal;

out vec4 o_Color;

uniform float u_AmbientStrength;
uniform vec3 u_Color;
uniform vec3 u_ViewerPosition;

// Light maps
uniform sampler2D u_AlbedoMap; // Or the diffuse map.
uniform sampler2D u_SpecularMap; 
uniform sampler2D u_NormalMap;

struct DirectionalLight
{
	vec3 m_Position;
	vec3 m_SpecularColor;
	float m_SpecularStrength;
	int m_SpecularExponent;
};

struct PointLight
{
	vec3 m_Position;
	vec3 m_SpecularColor;
	float m_Constant;
	float m_Linear;
	float m_Quadratic;
	float m_SpecularStrength;
	int m_SpecularExponent;
};

uniform DirectionalLight u_SceneDirectionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform PointLight u_ScenePointLights[MAX_POINT_LIGHTS];
uniform int u_SceneDirectionalLightCount = 0;
uniform int u_ScenePointLightCount = 0;

/* Flags */
uniform int u_HasAlbedoMap = 0;
uniform int u_HasNormalMap = 0;

// Function prototypes
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 specular_color);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 specular_color);

vec3 g_Ambient;
vec3 g_Color;

void main()
{
	vec3 Normal;
	bool light_cast = false;

	if (u_HasAlbedoMap == 1)
	{
		g_Color = vec3(texture(u_AlbedoMap, v_TexCoords));
	}

	else
	{
		g_Color = u_Color;
	}

	if (u_HasNormalMap == 1)
	{
		Normal = vec3(texture(u_NormalMap, v_TexCoords));
		Normal = normalize(Normal * 2.0f - 1.0f);
	}

	else
	{
		Normal = normalize(v_Normal);
	}

	// Calculate the ambient light only once
	vec3 FinalColor = vec3(0.0f, 0.0f, 0.0f);
	g_Ambient = u_AmbientStrength * g_Color;

	for (int i = 0 ; i < u_SceneDirectionalLightCount ; i++)
	{
		FinalColor += CalculateDirectionalLight(u_SceneDirectionalLights[i], Normal, u_SceneDirectionalLights[i].m_SpecularColor);
	}

	for (int i = 0 ; i < u_ScenePointLightCount ; i++)
	{
		FinalColor += CalculatePointLight(u_ScenePointLights[i], Normal, u_ScenePointLights[i].m_SpecularColor);
	}
	 
	o_Color = vec4(FinalColor.xyz, 1.0f);
}

/*
Calculates phong lighting with a bunch of parameters
*/
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 specular_color)
{
	vec3 LightDirection = normalize(light.m_Position - normal);

	float Diffuse = max(dot(normal, LightDirection), 0.0f);

	// Calculating the specular highlight
	vec3 ViewDir = normalize(u_ViewerPosition - v_FragPosition); // Get the direction of the fragment
	vec3 ReflectDir = reflect(-LightDirection, normal);  
	float Specular = pow(max(dot(ViewDir, ReflectDir), 0.0), light.m_SpecularExponent);
	
	vec3 DiffuseColor = Diffuse * g_Color; // To be replaced with diffuse map
	vec3 SpecularColor = light.m_SpecularStrength * Specular * specular_color ; // To be also sampled with specular map

	return vec3((g_Ambient + DiffuseColor + SpecularColor) * g_Color);  
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 specular_color)
{
	vec3 LightDirection = normalize(light.m_Position - normal);

	float Diffuse = max(dot(normal, LightDirection), 0.0f);

	// Calculating the specular highlight
	vec3 ViewDir = normalize(u_ViewerPosition - v_FragPosition); // Get the direction of the fragment
	vec3 ReflectDir = reflect(-LightDirection, normal);  
	float Specular = pow(max(dot(ViewDir, ReflectDir), 0.0), light.m_SpecularExponent);

	vec3 DiffuseColor = Diffuse * g_Color;
	vec3 SpecularColor = light.m_SpecularStrength * Specular * specular_color;    

	// Calculate the attenuation
	float Distance = length(light.m_Position - v_FragPosition);
    float Attenuation = 1.0 / (light.m_Constant + light.m_Linear * Distance + light.m_Quadratic * (Distance * Distance));
	
	DiffuseColor  *= Attenuation;
	SpecularColor *= Attenuation;
	return vec3(((g_Ambient * Attenuation) + DiffuseColor + SpecularColor) * g_Color);
}
