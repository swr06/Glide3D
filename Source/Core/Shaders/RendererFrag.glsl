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
in mat3 v_TBNMatrix;

out vec4 o_Color;

uniform float u_AmbientStrength;
uniform vec3 u_Color;
uniform vec3 u_ViewerPosition;

// Light maps
uniform sampler2D u_AlbedoMap; // Or the diffuse map.
uniform sampler2D u_SpecularMap; 
uniform sampler2D u_NormalMap;
uniform sampler2D u_LightMap;
uniform sampler2D u_ParallaxMap;
uniform samplerCube u_EnvironmentMap;

struct DirectionalLight
{
	vec3 m_Direction;
	vec3 m_SpecularColor;
	float m_SpecularStrength;
	int m_SpecularExponent;
	int m_IsBlinn;
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

/* Flags */
uniform int u_HasAlbedoMap = 0;
uniform int u_HasNormalMap = 0;

// Function prototypes
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 specular_color, int use_blinn);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 specular_color, int use_blinn);
vec4 TextureBiCubic(sampler2D sampler, vec2 texCoords);

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
		Normal = Normal * 2.0f - 1.0f;
		Normal = v_TBNMatrix * Normal; 
		Normal = normalize(Normal);
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
		FinalColor += CalculateDirectionalLight(u_SceneDirectionalLights[i], Normal, u_SceneDirectionalLights[i].m_SpecularColor, u_SceneDirectionalLights[i].m_IsBlinn);
	}

	for (int i = 0 ; i < u_ScenePointLightCount ; i++)
	{
		FinalColor += CalculatePointLight(u_ScenePointLights[i], Normal, u_ScenePointLights[i].m_SpecularColor, u_ScenePointLights[i].m_IsBlinn);
	}

	vec3 I = normalize(v_FragPosition - u_ViewerPosition);
    vec3 R = reflect(I, Normal);
    vec4 reflect_color = vec4(texture(u_EnvironmentMap, R).rgb, 1.0);
	
	o_Color = vec4(FinalColor.xyz, 1.0f);
	//o_Color = mix(o_Color, reflect_color, 0.3f);
}

/*
Calculates phong lighting with a bunch of parameters
*/
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 specular_color, int use_blinn)
{
	//vec3 LightDirection = normalize(light.m_Position - v_FragPosition);
	vec3 LightDirection = normalize(-light.m_Direction);

	float Diffuse = max(dot(normal, LightDirection), 0.0f);

	// Calculating the specular highlight
	vec3 ViewDir = normalize(u_ViewerPosition - v_FragPosition); // Get the direction of the fragment
	float Specular;

	if (use_blinn == 1)
	{
		vec3 Halfway = normalize(LightDirection + ViewDir);  
        Specular = pow(max(dot(normal, Halfway), 0.0), light.m_SpecularExponent);
	}

	else
	{
		vec3 ReflectDir = reflect(-LightDirection, normal);		
		Specular = pow(max(dot(ViewDir, ReflectDir), 0.0), light.m_SpecularExponent);
	}
	
	vec3 DiffuseColor = Diffuse * g_Color; // To be replaced with diffuse map
	vec3 SpecularColor = light.m_SpecularStrength * Specular * specular_color ; // To be also sampled with specular map

	return vec3((g_Ambient + DiffuseColor + SpecularColor) * g_Color);  
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 specular_color, int use_blinn)
{
	vec3 LightDirection = normalize(light.m_Position - v_FragPosition);

	float Diffuse = max(dot(normal, LightDirection), 0.0f);

	// Calculating the specular highlight
	vec3 ViewDir = normalize(u_ViewerPosition - v_FragPosition); // Get the direction of the fragment
	float Specular;

	if (use_blinn == 1)
	{
		vec3 Halfway = normalize(LightDirection + ViewDir);  
        Specular = pow(max(dot(normal, Halfway), 0.0), light.m_SpecularExponent);
	}

	else
	{
		vec3 ReflectDir = reflect(-LightDirection, normal);		
		float Specular = pow(max(dot(ViewDir, ReflectDir), 0.0), light.m_SpecularExponent);
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

vec4 cubic(float v)
{
    vec4 n = vec4(1.0, 2.0, 3.0, 4.0) - v;
    vec4 s = n * n * n;
    float x = s.x;
    float y = s.y - 4.0 * s.x;
    float z = s.z - 4.0 * s.y + 6.0 * s.x;
    float w = 6.0 - x - y - z;
    return vec4(x, y, z, w) * (1.0/6.0);
}

vec4 TextureBiCubic(sampler2D sampler, vec2 texCoords)
{
   vec2 texSize = textureSize(sampler, 0);
   vec2 invTexSize = 1.0 / texSize;

   texCoords = texCoords * texSize - 0.5;


    vec2 fxy = fract(texCoords);
    texCoords -= fxy;

    vec4 xcubic = cubic(fxy.x);
    vec4 ycubic = cubic(fxy.y);

    vec4 c = texCoords.xxyy + vec2 (-0.5, +1.5).xyxy;

    vec4 s = vec4(xcubic.xz + xcubic.yw, ycubic.xz + ycubic.yw);
    vec4 offset = c + vec4 (xcubic.yw, ycubic.yw) / s;

    offset *= invTexSize.xxyy;

    vec4 sample0 = texture(sampler, offset.xz);
    vec4 sample1 = texture(sampler, offset.yz);
    vec4 sample2 = texture(sampler, offset.xw);
    vec4 sample3 = texture(sampler, offset.yw);

    float sx = s.x / (s.x + s.y);
    float sy = s.z / (s.z + s.w);

    return mix(
       mix(sample3, sample2, sx), mix(sample1, sample0, sx)
    , sy);
}