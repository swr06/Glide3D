#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;
layout (location = 3) in vec3 a_Tangent;
layout (location = 4) in vec3 a_Bitangent;

// Per instance attributes 

// Model Matrix
layout (location = 5) in vec4 ModelColumn1;
layout (location = 6) in vec4 ModelColumn2;
layout (location = 7) in vec4 ModelColumn3;
layout (location = 8) in vec4 ModelColumn4;

// Normal Matrix
layout (location = 9) in vec4 NormalColumn1;
layout (location = 10) in vec4 NormalColumn2;
layout (location = 11) in vec4 NormalColumn3;
layout (location = 12) in vec4 NormalColumn4;

out mat3 v_TBNMatrix;
out vec2 v_TexCoords;
out vec3 v_FragPosition;
out vec3 v_Normal;

uniform mat4 u_ViewProjection;
uniform vec3 u_ViewerPosition;

void main()
{
	// Reconstruct the matrices from the columns
	mat4 ModelMatrix, NormalMatrix;
	ModelMatrix[0] = ModelColumn1;
	ModelMatrix[1] = ModelColumn2;
	ModelMatrix[2] = ModelColumn3;
	ModelMatrix[3] = ModelColumn4;

	NormalMatrix[0] = NormalColumn1;
	NormalMatrix[1] = NormalColumn2;
	NormalMatrix[2] = NormalColumn3;
	NormalMatrix[3] = NormalColumn4;

	gl_Position = u_ViewProjection * ModelMatrix * vec4(a_Position, 1.0f);

	/* Stuff for lighting */
	v_TexCoords = a_TexCoords;
	v_Normal = mat3(NormalMatrix) * a_Normal;  
	v_FragPosition = vec3(ModelMatrix * vec4(a_Position, 1.0f)); // For lighting calculations

	/* Construct the TBN Matrix */
	vec3 T = normalize(vec3(ModelMatrix * vec4(a_Tangent, 0.0)));
	vec3 B = normalize(vec3(ModelMatrix * vec4(a_Bitangent, 0.0)));
	vec3 N = normalize(vec3(ModelMatrix * vec4(a_Normal, 0.0)));
	v_TBNMatrix = mat3(T, B, N);
}