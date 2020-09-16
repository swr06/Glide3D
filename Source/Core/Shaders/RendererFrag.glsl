#version 330 core

/*
Lighting is based on the phong lighting model.
*/

in vec2 v_TexCoords;
in vec3 v_FragPosition;
in vec3 v_Normal;

out vec4 o_Color;

uniform float u_AmbientStrength;
uniform vec3 u_LightPosition;
uniform vec3 u_Color;
uniform vec3 u_ViewerPosition;

// Light maps
uniform sampler2D u_DiffuseMap;
uniform sampler2D u_SpecularMap;
uniform sampler2D u_AlbedoMap;
uniform sampler2D u_NormalMap;

struct DirectionalLight
{
	vec3 m_Position;
	float m_SpecularStrength;
	int m_SpecularExponent;
};

// Shader storage buffer object with the scene's light data
layout (std430, binding = 1) buffer SceneLightData
{
	DirectionalLight b_DirectionalLights[];
	int b_DirectionalLightCount;
};

vec3 CalculateDirectionalLight(float ambient_strength, float specular_strength, int specular_exponent,
							vec3 light_position, vec3 normal);

void main()
{
	o_Color = vec4(CalculateDirectionalLight(u_AmbientStrength, 5.0f, 32, u_LightPosition, normalize(v_Normal)), 1.0f);
}

/*
Calculates phong lighting with a bunch of parameters
*/
vec3 CalculateDirectionalLight(float ambient_strength, float specular_strength, int specular_exponent,
							vec3 light_position, vec3 normal)
{
	vec3 light_dir = normalize(light_position - normal);

	vec3 ambient = ambient_strength * u_Color;
	float diffuse_factor = max(dot(normal, light_dir), 0.0f);
	vec3 diffuse = diffuse_factor * u_Color;

	// Calculating the specular highlight
	vec3 viewer_direction = normalize(u_ViewerPosition - v_FragPosition); // Get the direction of the fragment
	vec3 reflect_direction = reflect(-light_dir, normal);  
	float spec = pow(max(dot(viewer_direction, reflect_direction), 0.0), specular_exponent);
	vec3 specular = specular_strength * spec * vec3(1.0f, 1.0f, 1.0f);  

	return vec3((ambient + diffuse + specular) * u_Color);  
}