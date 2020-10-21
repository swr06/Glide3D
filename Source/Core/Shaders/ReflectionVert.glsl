#version 440 core

layout (location = 0) in vec3 a_Position;
layout (location = 2) in vec2 a_TexCoords;
layout (location = 5) in vec4 ModelColumn1;
layout (location = 6) in vec4 ModelColumn2;
layout (location = 7) in vec4 ModelColumn3;
layout (location = 8) in vec4 ModelColumn4;

out vec2 v_TexCoords;

uniform mat4 u_ViewProjection;

void main()
{
	mat4 ModelMatrix;
	ModelMatrix[0] = ModelColumn1;
	ModelMatrix[1] = ModelColumn2;
	ModelMatrix[2] = ModelColumn3;
	ModelMatrix[3] = ModelColumn4;

	gl_Position = u_ViewProjection * ModelMatrix * vec4(a_Position, 1.0f);

	v_TexCoords = a_TexCoords;
}