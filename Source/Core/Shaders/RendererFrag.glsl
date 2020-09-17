#version 330 core

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
	float m_SpecularStrength;
	int m_SpecularExponent;
};

struct PointLight
{
	vec3 m_Position;
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

// Function prototypes
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal);
vec3 CalculatePointLight(PointLight light, vec3 normal);

vec3 g_Ambient;

void main()
{
	bool light_cast = false;

	// Calculate the ambient light only once
	vec3 FinalColor = vec3(0.0f, 0.0f, 0.0f);
	g_Ambient = u_AmbientStrength * u_Color;

	for (int i = 0 ; i < u_SceneDirectionalLightCount ; i++)
	{
		FinalColor += CalculateDirectionalLight(u_SceneDirectionalLights[i], normalize(v_Normal));
	}

	for (int i = 0 ; i < u_ScenePointLightCount ; i++)
	{
		FinalColor += CalculatePointLight(u_ScenePointLights[i], normalize(v_Normal));
	}

	o_Color = vec4(FinalColor.xyz, 1.0f);
}

/*
Calculates phong lighting with a bunch of parameters
*/
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal)
{
	vec3 LightDirection = normalize(light.m_Position - normal);

	float Diffuse = max(dot(normal, LightDirection), 0.0f);

	// Calculating the specular highlight
	vec3 ViewDir = normalize(u_ViewerPosition - v_FragPosition); // Get the direction of the fragment
	vec3 ReflectDir = reflect(-LightDirection, normal);  
	float Specular = pow(max(dot(ViewDir, ReflectDir), 0.0), light.m_SpecularExponent);
	
	vec3 DiffuseColor = Diffuse * u_Color; // To be replaced with diffuse map
	vec3 SpecularColor = light.m_SpecularStrength * Specular * vec3(1.0f, 1.0f, 1.0f); // To be replaced with specular map

	return vec3((g_Ambient + DiffuseColor + SpecularColor) * u_Color);  
}

vec3 CalculatePointLight(PointLight light, vec3 normal)
{
	vec3 LightDirection = normalize(light.m_Position - normal);

	float Diffuse = max(dot(normal, LightDirection), 0.0f);

	// Calculating the specular highlight
	vec3 ViewDir = normalize(u_ViewerPosition - v_FragPosition); // Get the direction of the fragment
	vec3 ReflectDir = reflect(-LightDirection, normal);  
	float Specular = pow(max(dot(ViewDir, ReflectDir), 0.0), light.m_SpecularExponent);

	vec3 DiffuseColor = Diffuse * u_Color;
	vec3 SpecularColor = light.m_SpecularStrength * Specular * vec3(1.0f, 1.0f, 1.0f);    

	// Calculate the attenuation
	float Distance = length(light.m_Position - v_FragPosition);
    float Attenuation = 1.0 / (light.m_Constant + light.m_Linear * Distance + light.m_Quadratic * (Distance * Distance));
	
	DiffuseColor  *= Attenuation;
	SpecularColor *= Attenuation;
	return vec3(((g_Ambient * Attenuation) + DiffuseColor + SpecularColor) * u_Color);
}
