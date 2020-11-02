/*
-------- Glide3D ---------
-- Lighting Pass Shader --
*/

/* -- Reference --           

 --Material--				-- Linear --				-- sRGB --
Water 				|	(0.02f, 0.02f, 0.02f) 	|  (0.15f, 0.15f, 0.15f)   	
Plastic / Glass     | 	(0.03f, 0.03f, 0.03f) 	|  (0.21f, 0.21f, 0.21f) 	
Plastic High 		|	(0.05f, 0.05f, 0.05f) 	|  (0.24f, 0.24f, 0.24f) 		
Glass / Ruby 		|	(0.08f, 0.08f, 0.08f) 	|  (0.31f, 0.31f, 0.31f) 	
Diamond				|	(0.17f, 0.17f, 0.17f) 	|  (0.45f, 0.45f, 0.45f) 	
Iron 				|	(0.56f, 0.57f, 0.58f) 	|  (0.77f, 0.78f, 0.78f) 	
Copper				|	(0.95f, 0.64f, 0.54f) 	|  (0.98f, 0.82f, 0.76f) 	
Gold 				|	(1.00f, 0.71f, 0.29f) 	|  (1.00f, 0.86f, 0.57f) 	
Aluminium			|	(0.91f, 0.92f, 0.92f) 	|  (0.96f, 0.96f, 0.97f) 	
Silver 				|	(0.95f, 0.93f, 0.88f) 	|  (0.98f, 0.97f, 0.95f)
*/

#version 450 core

#define MAX_DIRECTIONAL_LIGHTS 2
#define MAX_POINT_LIGHTS 4
#define PI 3.14159265359

layout (location = 0) out vec4 o_Color;

in vec2 v_TextureCoordinates;

uniform sampler2D u_PositionTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_ColorTexture;

// PBR TEXTURES
uniform sampler2D u_PBRComponentTexture;

struct DirectionalLight
{
	vec3 m_Direction;
	vec3 m_SpecularColor;
	float m_SpecularStrength;
	int m_SpecularExponent;
	int m_IsBlinn;
	int m_DirectionalLightElement;
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

// Shadow maps
uniform sampler2D m_DirectionalLightShadowmaps[MAX_DIRECTIONAL_LIGHTS];
uniform samplerCube m_PointlightShadowmaps[MAX_POINT_LIGHTS] ;

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

// PBR
float g_Roughness = 0.1f;
float g_Metalness = 0.1f;
uniform int u_UsesPBRLighting = 0;

vec3 g_F0;

// Function prototype
vec3 CalculateDirectionalLightPBR(DirectionalLight light, mat4 vp, sampler2D map);
vec3 CalculatePointLightPBR(PointLight light, samplerCube map);
float ShadowCalculation(vec4 light_fragpos, sampler2D map, vec3 light_dir);
float ShadowCalculationPOINT(PointLight pointlight, samplerCube map);
vec3 CalculateDirectionalLightPHONG(DirectionalLight light, mat4 vp, sampler2D map);
vec3 CalculatePointLightPHONG(PointLight light, samplerCube map);
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

const vec3 EmptyPixel = vec3(0.0f);

void main()
{
	// Obtain values from the geometry buffer
	g_Color = vec3(texture(u_ColorTexture, v_TextureCoordinates));
	g_Normal = vec3(texture(u_NormalTexture, v_TextureCoordinates));
	g_FragPosition = vec3(texture(u_PositionTexture, v_TextureCoordinates));

	vec3 PBRComponent = texture(u_PBRComponentTexture, v_TextureCoordinates).rgb;
	g_Metalness = PBRComponent.r;
	g_Roughness = PBRComponent.g;

	g_ViewDir = normalize(u_ViewerPosition - g_FragPosition);
	if (g_Color == EmptyPixel && g_Normal == EmptyPixel && g_FragPosition == EmptyPixel) 
	{ 
		o_Color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
		return;
	}

	g_Ambient = u_AmbientStrength * PBRComponent.b * g_Color;

	if (u_UsesPBRLighting == 1)
	{
		g_F0 = vec3(0.05f); // Dielectric Material
		g_F0 = mix(g_F0, g_Color, g_Metalness);

		vec3 Lo = vec3(0.0f);

		for (int i = 0 ; i < u_SceneDirectionalLightCount ; i++)
		{
			Lo += CalculateDirectionalLightPBR(u_SceneDirectionalLights[i], u_DirectionalLightSpaceVP[i], m_DirectionalLightShadowmaps[i]);
		}	

		for (int i = 0 ; i < u_ScenePointLightCount ; i++)
		{
			Lo += CalculatePointLightPBR(u_ScenePointLights[i], m_PointlightShadowmaps[i]);
		}	

		o_Color = vec4(g_Ambient + Lo, 1.0f);

		return;
	}

	else
	{
		vec3 FinalColor;

		for (int i = 0 ; i < u_SceneDirectionalLightCount ; i++)
		{
			FinalColor += CalculateDirectionalLightPHONG(u_SceneDirectionalLights[i], u_DirectionalLightSpaceVP[i], m_DirectionalLightShadowmaps[i]);
		}	

		for (int i = 0 ; i < u_ScenePointLightCount ; i++)
		{
			FinalColor += CalculatePointLightPHONG(u_ScenePointLights[i], m_PointlightShadowmaps[i]);
		}	

		o_Color = vec4(FinalColor, 1.0f);

		return;
	}

	return;
}

vec3 CalculateDirectionalLightPBR(DirectionalLight light, mat4 vp, sampler2D map)
{
	float Shadow = max(light.m_ShadowStrength * ShadowCalculation(vp * vec4(g_FragPosition, 1.0f), map, light.m_Direction), 0.1f);

	vec3 V = normalize(u_ViewerPosition - g_FragPosition);
    vec3 L = normalize(-light.m_Direction);
    vec3 H = normalize(V + L);
	vec3 radiance = light.m_SpecularColor ;

    float NDF = DistributionGGX(g_Normal, H, g_Roughness);   
    float G = GeometrySmith(g_Normal, V, L, g_Roughness);      
    vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), g_F0);
       
    vec3 nominator = NDF * G * F; 
    float denominator = 4 * max(dot(g_Normal, V), 0.0) * max(dot(g_Normal, L), 0.0);
    vec3 specular = nominator / max(denominator, 0.001f);
    
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - g_Metalness;	

    float NdotL = max(dot(g_Normal, L), 0.0);
    return (kD * g_Color / PI + specular) * radiance * NdotL * (1.0f - Shadow);
}

vec3 CalculatePointLightPBR(PointLight light, samplerCube map)
{
	float shadow = ShadowCalculationPOINT(light, map) * 2.0f;
	shadow = 1.0f - shadow;

	vec3 V = normalize(u_ViewerPosition - g_FragPosition);
    vec3 L = normalize(light.m_Position - g_FragPosition);
    vec3 H = normalize(V + L);
	float Distance = length(light.m_Position - g_FragPosition);
    float Attenuation = 1.0 / (light.m_Constant + light.m_Linear * Distance + light.m_Quadratic * (Distance * Distance)); 
	vec3 radiance = light.m_SpecularColor * Attenuation;

    float NDF = DistributionGGX(g_Normal, H, g_Roughness);   
    float G = GeometrySmith(g_Normal, V, L, g_Roughness);      
    vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), g_F0);
       
    vec3 nominator = NDF * G * F; 
    float denominator = 4 * max(dot(g_Normal, V), 0.0) * max(dot(g_Normal, L), 0.0);
    vec3 specular = nominator / max(denominator, 0.001f);
    
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - g_Metalness;	  

    float NdotL = max(dot(g_Normal, L), 0.0);        

    return ((kD * g_Color * shadow) / PI + specular * light.m_SpecularStrength) * radiance * NdotL; 
}

float ShadowCalculation(vec4 light_fragpos, sampler2D map, vec3 light_dir)
{
    vec3 ProjectionCoordinates = light_fragpos.xyz / light_fragpos.w; // Perspective division is not really needed for orthagonal projection but whatever
    ProjectionCoordinates = ProjectionCoordinates * 0.5f + 0.5f;
	float shadow = 0.0;

	if (ProjectionCoordinates.z > 1.0)
	{
		return 0.0f;
	}

    float ClosestDepth = texture(map, ProjectionCoordinates.xy).r; 
    float Depth = ProjectionCoordinates.z;
    float Bias =  max(0.05f * (1.0f - dot(g_Normal, light_dir)), 0.005f);
    //float Bias =  0.005f;
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

float ShadowCalculationPOINT(PointLight pointlight, samplerCube map)
{
    vec3 fragToLight = g_FragPosition - pointlight.m_Position;

    float closestDepth = texture(map, fragToLight).r;
    closestDepth *= 100.0f;
    float currentDepth = length(fragToLight);
    float bias = 0.5f; 
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;        
        
    return shadow;
}

vec3 CalculateDirectionalLightPHONG(DirectionalLight light, mat4 vp, sampler2D map)
{
	float Shadow = max(light.m_ShadowStrength * ShadowCalculation(vp * vec4(g_FragPosition, 1.0f), map, light.m_Direction), 0.1f);
	Shadow = 1.0f - Shadow;

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

	return vec3((g_Ambient + (DiffuseColor * Shadow) + SpecularColor) * g_Color);  
}

vec3 CalculatePointLightPHONG(PointLight light, samplerCube map)
{
	float shadow = ShadowCalculationPOINT(light, map) * 2.0f;
	shadow = 1.0f - shadow;

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
	return vec3(((g_Ambient * Attenuation) + (DiffuseColor * shadow) + SpecularColor) * g_Color);
}

// The below are just implementations of PBR equations
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
    float k = (r * r) / 8.0;

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