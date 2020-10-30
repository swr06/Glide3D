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
	float m_ShadowStrength;
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

// PBR
uniform float u_Roughness = 0.1f;
uniform float u_Metalness = 0.1f;
vec3 g_F0;

// Function prototype
vec3 CalculateDirectionalLight(DirectionalLight light);
vec3 CalculatePointLight(PointLight light);
float ShadowCalculation(vec4 light_fragpos, sampler2D map, vec3 light_dir);

const vec3 EmptyPixel = vec3(0.0f);
const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / max(denom, 0.001); 
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
	// Obtain values from the geometry buffer
	g_Color = vec3(texture(u_ColorTexture, v_TextureCoordinates));
	g_Normal = vec3(texture(u_NormalTexture, v_TextureCoordinates));
	g_FragPosition = vec3(texture(u_PositionTexture, v_TextureCoordinates));
	g_ViewDir = normalize(u_ViewerPosition - g_FragPosition);

	if (g_Color == EmptyPixel && g_Normal == EmptyPixel && g_FragPosition == EmptyPixel) 
	{ 
		o_Color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
		return;
	}

	for (int i = 0 ; i < u_SceneDirectionalLightCount ; i++)
	{
		g_Shadow += u_SceneDirectionalLights[i].m_ShadowStrength * ShadowCalculation(u_DirectionalLightSpaceVP[i] * vec4(g_FragPosition, 1.0f), u_SceneDirectionalLights[i].m_DepthMap, u_SceneDirectionalLights[i].m_Direction);
	}

	g_Shadow = 1.0f - g_Shadow;
	g_Shadow = max(g_Shadow, 0.01f);
	g_Ambient = g_Color * g_Shadow;

	g_F0 = vec3(0.04f);
	g_F0 = mix(g_F0, g_Color, u_Metalness);

	vec3 Lo = vec3(0.0f);

	for (int i = 0 ; i < u_SceneDirectionalLightCount ; i++)
	{
		Lo += CalculateDirectionalLight(u_SceneDirectionalLights[i]);
	}	

	for (int i = 0 ; i < u_ScenePointLightCount ; i++)
	{
		Lo += CalculatePointLight(u_ScenePointLights[i]);
	}	

	o_Color = vec4(g_Ambient + Lo, 1.0f);
}

vec3 CalculateDirectionalLight(DirectionalLight light)
{
	vec3 V = normalize(u_ViewerPosition - g_FragPosition);
    vec3 L = normalize(-light.m_Direction);
    vec3 H = normalize(V + L);
	vec3 radiance = light.m_SpecularColor ;

    float NDF = DistributionGGX(g_Normal, H, u_Roughness);   
    float G = GeometrySmith(g_Normal, V, L, u_Roughness);      
    vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), g_F0);
       
    vec3 nominator = NDF * G * F; 
    float denominator = 4 * max(dot(g_Normal, V), 0.0) * max(dot(g_Normal, L), 0.0);
    vec3 specular = nominator / max(denominator, 0.001f);
    
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - u_Metalness;	  

    float NdotL = max(dot(g_Normal, L), 0.0);        

    return ((kD * g_Color / PI + specular * light.m_SpecularStrength) * radiance * NdotL);
}

vec3 CalculatePointLight(PointLight light)
{
	vec3 V = normalize(u_ViewerPosition - g_FragPosition);
    vec3 L = normalize(light.m_Position - g_FragPosition);
    vec3 H = normalize(V + L);
	float Distance = length(light.m_Position - g_FragPosition);
    float Attenuation = 1.0 / (light.m_Constant + light.m_Linear * Distance + light.m_Quadratic * (Distance * Distance)); 
	vec3 radiance = light.m_SpecularColor * Attenuation;

    float NDF = DistributionGGX(g_Normal, H, u_Roughness);   
    float G = GeometrySmith(g_Normal, V, L, u_Roughness);      
    vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), g_F0);
       
    vec3 nominator = NDF * G * F; 
    float denominator = 4 * max(dot(g_Normal, V), 0.0) * max(dot(g_Normal, L), 0.0);
    vec3 specular = nominator / max(denominator, 0.001f);
    
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - u_Metalness;	  

    float NdotL = max(dot(g_Normal, L), 0.0);        

    return (kD * g_Color / PI + specular * light.m_SpecularStrength) * radiance * NdotL; 
}

float ShadowCalculation(vec4 light_fragpos, sampler2D map, vec3 light_dir)
{
    vec3 ProjectionCoordinates = light_fragpos.xyz;
    ProjectionCoordinates = ProjectionCoordinates * 0.5f + 0.5f;
	float shadow = 0.0;

	if (ProjectionCoordinates.z > 1.0)
	{
		return 0.0f;
	}

    float ClosestDepth = texture(map, ProjectionCoordinates.xy).r; 
    float Depth = ProjectionCoordinates.z;
    //float Bias =  max(0.05f * (1.0f - dot(g_Normal, light_dir)), 0.005f);
    float Bias =  0.005f;
	vec2 TexelSize = 1.0 / textureSize(map, 0); // LOD = 0

	// Take the average of the surrounding texels to create the PCF effect
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcf = texture(map, ProjectionCoordinates.xy + vec2(x, y) * TexelSize).r; 
			shadow += Depth - Bias > pcf ? 1.0 : 0.0;        
		}    
	}

	shadow /= 9.0;
    return shadow;
}