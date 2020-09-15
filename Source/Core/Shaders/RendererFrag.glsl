#version 330 core

in vec2 v_TexCoords;
in vec3 v_FragPosition;
in vec3 v_Normal;

out vec4 o_Color;

uniform float u_AmbientStrength;
uniform vec3 u_LightPosition;
uniform vec3 u_Color;
uniform vec3 u_ViewerPosition;

void main()
{
	float specular_strength = 0.5f;
	int specular_exponent = 32;

	vec3 normal = normalize(v_Normal);
	vec3 light_dir = normalize(u_LightPosition - normal);

	vec3 ambient = u_AmbientStrength * u_Color;
	float diffuse_factor = max(dot(normal, light_dir), 0.0f);
	vec3 diffuse = diffuse_factor * u_Color;

	// Calculating the specular highlight
	vec3 viewer_direction = normalize(u_ViewerPosition - v_FragPosition); // Get the direction of the fragment
	vec3 reflect_direction = reflect(-light_dir, normal);  
	float spec = pow(max(dot(viewer_direction, reflect_direction), 0.0), specular_exponent);
	vec3 specular = specular_strength * spec * u_Color;  

	o_Color = vec4((ambient + diffuse + specular) * u_Color, 1.0f);
}