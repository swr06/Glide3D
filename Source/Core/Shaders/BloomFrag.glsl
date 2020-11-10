#version 330 core

layout (location = 0) out vec4 o_Color;

in vec2 v_TexCoords;

uniform sampler2D u_LightPassTexture;
uniform sampler2D u_BloomPassTexture;

void main()
{
	vec4 Color = texture(u_LightPassTexture, v_TexCoords).rgba;
	vec4 BloomColor = texture(u_BloomPassTexture, v_TexCoords).rgba;
	
	o_Color = BloomColor + Color;
}