#version 330 core

#define MAX_DIRECTIONAL_LIGHTS 2
#define MAX_POINT_LIGHTS 14

layout (location = 0) out vec4 o_Color;

in vec2 v_TextureCoordinates;

uniform sampler2D u_PositionTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_ColorTexture;

struct DirectionalLight
{
	vec3 m_Direction;
	vec3 m_SpecularColor;
	float m_SpecularStrength;
	int m_SpecularExponent;
	int m_IsBlinn;
	int m_DirectionalLightElement;
	sampler2D m_DepthMap;
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
	int m_IsBlinn;
};

uniform DirectionalLight u_SceneDirectionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform PointLight u_ScenePointLights[MAX_POINT_LIGHTS];
uniform vec3 u_ViewerPosition;
uniform vec3 u_AmbientLight;
uniform int u_SceneDirectionalLightCount = 0;
uniform int u_ScenePointLightCount = 0;

vec3 g_FragPosition;
vec3 g_Normal;
vec3 g_Color;
vec3 g_ViewDir;

// Function prototype
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 specular_color, int use_blinn);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 specular_color, int use_blinn);

void main()
{
	vec3 FinalColor = vec3(0.0f);

	// Obtain values from the geometry buffer
	g_Color = vec3(texture(u_ColorTexture, v_TextureCoordinates));
	g_Normal = vec3(texture(u_NormalTexture, v_TextureCoordinates));
	g_FragPosition = vec3(texture(u_PositionTexture, v_TextureCoordinates));
	g_ViewDir = normalize(u_ViewerPosition - g_FragPosition);

	for (int i = 0 ; i < u_SceneDirectionalLightCount ; i++)
	{
		FinalColor += CalculateDirectionalLight(u_SceneDirectionalLights[i], g_Normal, u_SceneDirectionalLights[i].m_SpecularColor, u_SceneDirectionalLights[i].m_IsBlinn);
	}	

	for (int i = 0 ; i < u_ScenePointLightCount ; i++)
	{
		FinalColor += CalculatePointLight(u_ScenePointLights[i], g_Normal, u_ScenePointLights[i].m_SpecularColor, u_ScenePointLights[i].m_IsBlinn);
	}	

	o_Color = vec4(FinalColor, 1.0f);
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 specular_color, int use_blinn)
{
	vec3 LightDirection = normalize(-light.m_Direction);

	float Diffuse = max(dot(normal, LightDirection), 0.0f);

	float Specular;

	if (use_blinn == 1)
	{
		vec3 Halfway = normalize(LightDirection + g_ViewDir);  
        Specular = pow(max(dot(normal, Halfway), 0.0), light.m_SpecularExponent);
	}

	else
	{
		vec3 ReflectDir = reflect(-LightDirection, normal);		
		Specular = pow(max(dot(g_ViewDir, ReflectDir), 0.0), light.m_SpecularExponent);
	}
	
	vec3 DiffuseColor = Diffuse * g_Color; 
	vec3 SpecularColor = light.m_SpecularStrength * Specular * specular_color ; // To be also sampled with specular map

	return vec3(u_AmbientLight * (DiffuseColor + SpecularColor) * g_Color);  
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 specular_color, int use_blinn)
{
	vec3 LightDirection = normalize(light.m_Position - g_FragPosition);

	float Diffuse = max(dot(normal, LightDirection), 0.0f);

	float Specular;

	if (use_blinn == 1)
	{
		vec3 Halfway = normalize(LightDirection + g_ViewDir);  
        Specular = pow(max(dot(normal, Halfway), 0.0), light.m_SpecularExponent);
	}

	else
	{
		vec3 ReflectDir = reflect(-LightDirection, normal);		
		float Specular = pow(max(dot(g_ViewDir, ReflectDir), 0.0), light.m_SpecularExponent);
	}

	vec3 DiffuseColor = Diffuse * g_Color;
	vec3 SpecularColor = light.m_SpecularStrength * Specular * specular_color;    

	// Calculate the attenuation
	float Distance = length(light.m_Position - g_FragPosition);
    float Attenuation = 1.0 / (light.m_Constant + light.m_Linear * Distance + light.m_Quadratic * (Distance * Distance));
	
	DiffuseColor  *= Attenuation;
	SpecularColor *= Attenuation;
	return vec3(((u_AmbientLight * Attenuation) + DiffuseColor + SpecularColor) * g_Color);
}

