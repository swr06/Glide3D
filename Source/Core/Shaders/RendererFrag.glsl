/*
Forward rendering shader
Used for initial prototyping 
*/

#version 440 core

/*
The renderer's fragment shader which includes a phong lighting model
*/

#define MAX_DIRECTIONAL_LIGHTS 2
#define MAX_POINT_LIGHTS 14

/*
Lighting is based on the phong lighting model.
*/

in vec2 v_TexCoords;
in vec3 v_FragPosition;
in vec3 v_Normal;
in mat3 v_TBNMatrix;
in vec4 v_DirectionalLightFragPositions[MAX_DIRECTIONAL_LIGHTS];
in flat int v_InstanceID;

layout(location = 0) out vec4 o_Color;

uniform float u_AmbientStrength;
uniform vec4 u_Color;
uniform vec3 u_ViewerPosition;

// 6 samplers for the light maps 
uniform sampler2D u_AlbedoMap; // Or the diffuse map.
uniform sampler2D u_SpecularMap; 
uniform sampler2D u_NormalMap;
uniform sampler2D u_ParallaxMap;
uniform sampler2D u_MetalnessMap;
uniform sampler2D u_RoughnessMap;

// 2 samplers for the directional lights

uniform samplerCube u_EnvironmentMap;
uniform samplerCube u_PointlightDepthMaps[MAX_POINT_LIGHTS];

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
uniform int u_SceneDirectionalLightCount = 0;
uniform int u_ScenePointLightCount = 0;
uniform float u_Reflectance = 0.0f;

/* Flags */
uniform int u_HasAlbedoMap = 0;
uniform int u_HasNormalMap = 0;

// Function prototypes
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 specular_color, int use_blinn);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 specular_color, int use_blinn);
vec4 TextureBiCubic(sampler2D sampler, vec2 texCoords);
float ShadowCalculation(vec4 light_fragpos, sampler2D map);

vec3 g_Ambient;
vec3 g_Color;
float g_Shadow = 0.0f;
vec3 g_ViewDir = normalize(u_ViewerPosition - v_FragPosition); // Get the direction of the fragment

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
		g_Color = vec3(u_Color.rgb);
	}

	if (u_HasNormalMap == 1)
	{
		Normal = (texture(u_NormalMap, v_TexCoords)).rgb;
		Normal = Normal * 2.0f - 1.0f;
		Normal = v_TBNMatrix * Normal; 
		Normal = normalize(Normal);
	}

	else
	{
		Normal = normalize(v_Normal);
	}

	g_Ambient = u_AmbientStrength * g_Color;

	// Calculate the shadow once per fragment

	for (int i = 0 ; i < u_SceneDirectionalLightCount ; i++)
	{
		g_Shadow += ShadowCalculation(v_DirectionalLightFragPositions[i], u_SceneDirectionalLights[i].m_DepthMap);
	}

	g_Ambient = g_Ambient + 1.0f - g_Shadow;

	// Calculate the ambient light only once
	vec3 FinalColor = vec3(0.0f, 0.0f, 0.0f);

	for (int i = 0 ; i < u_SceneDirectionalLightCount ; i++)
	{
		FinalColor += CalculateDirectionalLight(u_SceneDirectionalLights[i], Normal, u_SceneDirectionalLights[i].m_SpecularColor, u_SceneDirectionalLights[i].m_IsBlinn);
	}

	for (int i = 0 ; i < u_ScenePointLightCount ; i++)
	{
		FinalColor += CalculatePointLight(u_ScenePointLights[i], Normal, u_ScenePointLights[i].m_SpecularColor, u_ScenePointLights[i].m_IsBlinn);
	}

	o_Color = vec4(FinalColor.xyz, 1.0f);

	if (u_Reflectance > 0.002f)
	{
		vec3 I = normalize(v_FragPosition - u_ViewerPosition);
		vec3 R = reflect(I, Normal);
		vec4 reflect_color = vec4(texture(u_EnvironmentMap, R).rgb, 1.0);
		o_Color = mix(o_Color, reflect_color, u_Reflectance);
	}

	o_Color.a = u_Color.a;
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

/*
Calculates phong lighting with a bunch of parameters
*/
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 specular_color, int use_blinn)
{
	//vec3 LightDirection = normalize(light.m_Position - v_FragPosition);
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
	
	vec3 DiffuseColor = Diffuse * g_Color; // To be replaced with diffuse map
	vec3 SpecularColor = light.m_SpecularStrength * Specular * specular_color ; // To be also sampled with specular map

	return vec3(g_Ambient * (DiffuseColor + SpecularColor) * g_Color);  
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 specular_color, int use_blinn)
{
	vec3 LightDirection = normalize(light.m_Position - v_FragPosition);

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
	float Distance = length(light.m_Position - v_FragPosition);
    float Attenuation = 1.0 / (light.m_Constant + light.m_Linear * Distance + light.m_Quadratic * (Distance * Distance));
	
	DiffuseColor  *= Attenuation;
	SpecularColor *= Attenuation;
	return vec3(((g_Ambient * Attenuation) + DiffuseColor + SpecularColor) * g_Color);
}

