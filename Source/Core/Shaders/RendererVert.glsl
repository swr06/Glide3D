#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;

// Per instance attributes 
layout (location = 3) in vec4 a_ModelColumn0;
layout (location = 4) in vec4 a_ModelColumn1;
layout (location = 5) in vec4 a_ModelColumn2;
layout (location = 6) in vec4 a_ModelColumn3;

out vec2 v_TexCoords;
out vec3 v_FragPosition;
out vec3 v_Normal;

uniform mat4 u_ViewProjection;

void main()
{
	mat4 model_matrix;
	model_matrix[0] = a_ModelColumn0;
	model_matrix[1] = a_ModelColumn1;
	model_matrix[2] = a_ModelColumn2;
	model_matrix[3] = a_ModelColumn3;

	v_FragPosition = vec3(model_matrix * vec4(a_Position, 1.0f)); // For lighting calculations
	gl_Position = u_ViewProjection * model_matrix * vec4(a_Position, 1.0f);

	v_TexCoords = a_TexCoords;
	v_Normal = a_Normal;
	v_Normal = mat3(transpose(inverse(model_matrix))) * a_Normal;  
}