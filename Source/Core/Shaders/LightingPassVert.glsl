#version 330 core

#define MAX_DIRECTIONAL_LIGHTS 2
#define MAX_POINT_LIGHTS 14

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TextureCoordinates;

out vec2 v_TextureCoordinates;

void main()
{
	// the position is in NDC
	gl_Position = vec4(a_Position, 1.0f);
	v_TextureCoordinates = a_TextureCoordinates;
}