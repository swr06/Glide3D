#version 440 core

layout (location = 0) in vec3 a_Position;
layout (location = 2) in vec2 a_TexCoords;

out vec2 v_TexCoords;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewProjection;

void main()
{
	gl_Position = u_ViewProjection * u_ModelMatrix * vec4(a_Position, 1.0f);

	v_TexCoords = a_TexCoords;
}