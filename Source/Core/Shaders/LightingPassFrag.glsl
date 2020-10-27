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
uniform float u_AmbientStrength = 0.75;
uniform int u_SceneDirectionalLightCount = 0;
uniform int u_ScenePointLightCount = 0;
uniform mat4 u_DirectionalLightSpaceVP[MAX_DIRECTIONAL_LIGHTS];

vec3 g_FragPosition;
vec3 g_Normal;
vec3 g_Color;
vec3 g_ViewDir;
vec3 g_Ambient;
float g_Shadow;

// Function prototype
vec3 CalculateDirectionalLight(DirectionalLight light);
vec3 CalculatePointLight(PointLight light);
float ShadowCalculation(vec4 light_fragpos, sampler2D map);

const vec3 EmptyPixel = vec3(0.0f);

void main()
{
	// Obtain values from the geometry buffer
	g_Color = vec3(texture(u_ColorTexture, v_TextureCoordinates));
	g_Normal = vec3(texture(u_NormalTexture, v_TextureCoordinates));
	g_FragPosition = vec3(texture(u_PositionTexture, v_TextureCoordinates));

	if (g_Color == EmptyPixel && g_Normal == EmptyPixel && g_FragPosition == EmptyPixel) 
	{ 
		o_Color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
		return;
	}

	vec3 FinalColor = vec3(0.0f);
	g_ViewDir = normalize(u_ViewerPosition - g_FragPosition);

	for (int i = 0 ; i < u_SceneDirectionalLightCount ; i++)
	{
		g_Shadow += ShadowCalculation(u_DirectionalLightSpaceVP[i] * vec4(g_FragPosition, 1.0f), u_SceneDirectionalLights[i].m_DepthMap);
	}

	g_Ambient = u_AmbientStrength * g_Color;
	g_Ambient = g_Ambient + 1.0f - g_Shadow;

	for (int i = 0 ; i < u_SceneDirectionalLightCount ; i++)
	{
		FinalColor += CalculateDirectionalLight(u_SceneDirectionalLights[i]);
	}	

	for (int i = 0 ; i < u_ScenePointLightCount ; i++)
	{
		FinalColor += CalculatePointLight(u_ScenePointLights[i]);
	}	

	o_Color = vec4(FinalColor, 1.0f);
}

vec3 CalculateDirectionalLight(DirectionalLight light)
{
	vec3 LightDirection = normalize(-light.m_Direction);

	float Diffuse = max(dot(g_Normal, LightDirection), 0.0f);

	float Specular;

	if (light.m_IsBlinn == 1) 
	{
		vec3 Halfway = normalize(LightDirection + g_ViewDir);  
        Specular = pow(max(dot(g_Normal, Halfway), 0.0), light.m_SpecularExponent);
	}

	else
	{
		vec3 ReflectDir = reflect(-LightDirection, g_Normal);		
		Specular = pow(max(dot(g_ViewDir, ReflectDir), 0.0), light.m_SpecularExponent);
	}
	
	vec3 DiffuseColor = Diffuse * g_Color; 
	vec3 SpecularColor = light.m_SpecularStrength * Specular * light.m_SpecularColor ; // To be also sampled with specular map

	return vec3((g_Ambient + DiffuseColor + SpecularColor) * g_Color);  
}

vec3 CalculatePointLight(PointLight light)
{
	vec3 LightDirection = normalize(light.m_Position - g_FragPosition);

	float Diffuse = max(dot(g_Normal, LightDirection), 0.0f);

	float Specular;

	if (light.m_IsBlinn == 1)
	{
		vec3 Halfway = normalize(LightDirection + g_ViewDir);  
        Specular = pow(max(dot(g_Normal, Halfway), 0.0), light.m_SpecularExponent);
	}

	else
	{
		vec3 ReflectDir = reflect(-LightDirection, g_Normal);		
		float Specular = pow(max(dot(g_ViewDir, ReflectDir), 0.0), light.m_SpecularExponent);
	}

	vec3 DiffuseColor = Diffuse * g_Color;
	vec3 SpecularColor = light.m_SpecularStrength * Specular * light.m_SpecularColor;    

	// Calculate the attenuation
	float Distance = length(light.m_Position - g_FragPosition);
    float Attenuation = 1.0 / (light.m_Constant + light.m_Linear * Distance + light.m_Quadratic * (Distance * Distance));
	
	DiffuseColor  *= Attenuation;
	SpecularColor *= Attenuation;
	return vec3(((g_Ambient * Attenuation) + DiffuseColor + SpecularColor) * g_Color);
}

float ShadowCalculation(vec4 light_fragpos, sampler2D map)
{
    vec3 ProjectionCoordinates = light_fragpos.xyz / light_fragpos.w;
    ProjectionCoordinates = ProjectionCoordinates * 0.5f + 0.5f;
	float shadow = 0.0;

	if(ProjectionCoordinates.z > 1.0)
	{
        shadow = 0.0;
		return shadow;
	}

    float ClosestDepth = texture(map, ProjectionCoordinates.xy).r; 
    float Depth = ProjectionCoordinates.z;
    float bias = 0.005f;

	vec2 texelsz = 1.0 / textureSize(map, 0);

	// Take the average of the surrounding texels to create the PCF effect
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcf = texture(map, ProjectionCoordinates.xy + vec2(x, y) * texelsz).r; 
			shadow += Depth - bias > pcf ? 1.0 : 0.0;        
		}    
	}

	shadow /= 9.0;
    return shadow;
}
