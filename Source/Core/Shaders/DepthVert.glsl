#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 5) in vec4 ModelColumn1;
layout (location = 6) in vec4 ModelColumn2;
layout (location = 7) in vec4 ModelColumn3;
layout (location = 8) in vec4 ModelColumn4;

uniform mat4 u_ViewProjection;

void main()
{
	// Assemble the model matrix
	mat4 ModelMatrix;

	ModelMatrix[0] = ModelColumn1;
	ModelMatrix[1] = ModelColumn2;
	ModelMatrix[2] = ModelColumn3;
	ModelMatrix[3] = ModelColumn4;

	gl_Position = u_ViewProjection * ModelMatrix * vec4(a_Position, 1.0f) ;
}