#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;

// Per instance attributes 

// Model Matrix
in mat4 ModelMatrix;

// Normal Matrix
in mat4 NormalMatrix;

out vec2 v_TexCoords;
out vec3 v_FragPosition;
out vec3 v_Normal;

uniform mat4 u_ViewProjection;

void main()
{
	gl_Position = u_ViewProjection * ModelMatrix * vec4(a_Position, 1.0f);

	/* Stuff for lighting */
	v_TexCoords = a_TexCoords;
	v_Normal = mat3(NormalMatrix) * a_Normal;  
	v_FragPosition = vec3(ModelMatrix * vec4(a_Position, 1.0f)); // For lighting calculations
}