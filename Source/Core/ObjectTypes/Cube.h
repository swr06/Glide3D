#pragma once

#include <array>
#include "../Object/Object.h"

namespace Glide3D
{
	class CubeObject : public Object
	{
	public :
		/*
		Takes in a 2D array of texture coordinates.
		The order should be as follows : 
		1) Forward face
		2) Back face
		3) Top face
		4) Bottom face
		5) Left face
		6) Right face
		*/

		CubeObject(const std::array<PlaneTextureCoordinates, 6>& texture_coordinates)
		{
			Mesh& mesh = GenerateMesh();
			std::vector<Vertex>& vertices = mesh.p_Vertices;
			std::vector<GLuint>& indices = mesh.p_Indices;

			// Define the indices for the cube with the format (0, 1, 2, 2, 3, 0)
			indices =
			{
				0, 1, 2, 2, 3, 0,
				0 + 4, 1 + 4, 2 + 4, 2 + 4, 3 + 4, 0 + 4,
				0 + 8, 1 + 8, 2 + 8, 2 + 8, 3 + 8, 0 + 8,
				0 + 12, 1 + 12, 2 + 12, 2 + 12, 3 + 12, 0 + 12,
				0 + 16, 1 + 16, 2 + 16, 2 + 16, 3 + 16, 0 + 16,
				0 + 20, 1 + 20, 2 + 20, 2 + 20, 3 + 20, 0 + 20,
			};

			// Forward Face
			vertices.push_back({glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(texture_coordinates[0][0], texture_coordinates[0][1]) });
			vertices.push_back({glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(texture_coordinates[0][2], texture_coordinates[0][3]) });
			vertices.push_back({glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(texture_coordinates[0][4], texture_coordinates[0][5]) });
			vertices.push_back({glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(texture_coordinates[0][4], texture_coordinates[0][5]) });
			vertices.push_back({glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(texture_coordinates[0][6], texture_coordinates[0][7])});
			vertices.push_back({ glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(texture_coordinates[0][0], texture_coordinates[0][1]) });

			// Back Face									  			 
			vertices.push_back({glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(texture_coordinates[1][0], texture_coordinates[1][1]) });
			vertices.push_back({glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(texture_coordinates[1][2], texture_coordinates[1][3]) });
			vertices.push_back({glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(texture_coordinates[1][4], texture_coordinates[1][5]) });
			vertices.push_back({glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(texture_coordinates[1][4], texture_coordinates[1][5]) });
			vertices.push_back({glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(texture_coordinates[1][6], texture_coordinates[1][7])});
			vertices.push_back({ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(texture_coordinates[1][0], texture_coordinates[1][1]) });

			// Top Face													 
			vertices.push_back({glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(texture_coordinates[2][0], texture_coordinates[2][1]) });
			vertices.push_back({glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(texture_coordinates[2][2], texture_coordinates[2][3]) });
			vertices.push_back({glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(texture_coordinates[2][4], texture_coordinates[2][5]) });
			vertices.push_back({glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(texture_coordinates[2][4], texture_coordinates[2][5]) });
			vertices.push_back({glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(texture_coordinates[2][6], texture_coordinates[2][7])});
			vertices.push_back({ glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(texture_coordinates[2][0], texture_coordinates[2][1]) });

			// Bottom Face									 			 
			vertices.push_back({glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(texture_coordinates[3][0], texture_coordinates[3][1]) });
			vertices.push_back({glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(texture_coordinates[3][2], texture_coordinates[3][3]) });
			vertices.push_back({glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(texture_coordinates[3][4], texture_coordinates[3][5]) });
			vertices.push_back({glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(texture_coordinates[3][4], texture_coordinates[3][5]) });
			vertices.push_back({glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(texture_coordinates[3][6], texture_coordinates[3][7])});
			vertices.push_back({ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(texture_coordinates[3][0], texture_coordinates[3][1]) });

			// Left Face									 			 
			vertices.push_back({glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(texture_coordinates[4][0], texture_coordinates[4][1]) });
			vertices.push_back({glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(texture_coordinates[4][2], texture_coordinates[4][3]) });
			vertices.push_back({glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(texture_coordinates[4][4], texture_coordinates[4][5]) });
			vertices.push_back({glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(texture_coordinates[4][4], texture_coordinates[4][5]) });
			vertices.push_back({glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(texture_coordinates[4][6], texture_coordinates[4][7])});
			vertices.push_back({ glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(texture_coordinates[4][0], texture_coordinates[4][1]) });

			// Right Face									  			 
			vertices.push_back({glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(texture_coordinates[5][0], texture_coordinates[5][1]) });
			vertices.push_back({glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(texture_coordinates[5][2], texture_coordinates[5][3]) });
			vertices.push_back({glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(texture_coordinates[5][4], texture_coordinates[5][5]) });
			vertices.push_back({glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(texture_coordinates[5][4], texture_coordinates[5][5]) });
			vertices.push_back({glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(texture_coordinates[5][6], texture_coordinates[5][7])});
			vertices.push_back({ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(texture_coordinates[5][0], texture_coordinates[5][1]) });

			p_CanFacecull = false; // Todo : Change winding order so it can facecull

			mesh.p_Color = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);

			// Calculate tangent normals
			CalculateTangentNormals();

			// Upload the data to the gpu
			Buffer();
		}

		CubeObject()
		{
			Mesh& mesh = GenerateMesh();
			std::vector<Vertex>& vertices = mesh.p_Vertices;
			std::vector<GLuint>& indices = mesh.p_Indices;

			// Forward Face
			vertices.push_back({ glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)});
			vertices.push_back({ glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)});
			vertices.push_back({ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)});
			vertices.push_back({ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)});
			vertices.push_back({ glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)});
			vertices.push_back({ glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f) });

			// Back Face									  			 
			vertices.push_back({ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 1.0f)});
			vertices.push_back({ glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 1.0f)});
			vertices.push_back({ glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 0.0f)});
			vertices.push_back({ glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 0.0f)});
			vertices.push_back({ glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 0.0f)});
			vertices.push_back({ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 1.0f) });

			// Top Face													 
			vertices.push_back({ glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 1.0f)});
			vertices.push_back({ glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 1.0f)});
			vertices.push_back({ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.0f)});
			vertices.push_back({ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.0f)});
			vertices.push_back({ glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 0.0f)});
			vertices.push_back({ glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 1.0f) });

			// Bottom Face									 			 
			vertices.push_back({ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 1.0f)});
			vertices.push_back({ glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 1.0f)});
			vertices.push_back({ glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 0.0f)});
			vertices.push_back({ glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 0.0f)});
			vertices.push_back({ glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 0.0f)});
			vertices.push_back({ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 1.0f) });

			// Left Face									 			 
			vertices.push_back({ glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f)});
			vertices.push_back({ glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f)});
			vertices.push_back({ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f)});
			vertices.push_back({ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f)});
			vertices.push_back({ glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f)});
			vertices.push_back({ glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f) });

			// Right Face									  			 
			vertices.push_back({ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f)});
			vertices.push_back({ glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f)});
			vertices.push_back({ glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f)});
			vertices.push_back({ glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f)});
			vertices.push_back({ glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f)});
			vertices.push_back({ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f) });

			mesh.p_Color = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
			p_CanFacecull = false; // Todo : Change winding order so it can facecull

			// Calculate tangent normals
			CalculateTangentNormals();

			// Upload the data to the gpu
			Buffer();
		}
	};
}