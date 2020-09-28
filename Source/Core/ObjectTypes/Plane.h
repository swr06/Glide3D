#pragma once

#include <array>
#include "../Object/Object.h"

namespace Glide3D
{
	class PlaneObject : public Object
	{
	public:

		PlaneObject(const std::array<PlaneTextureCoordinates, 6>& texture_coordinates)
		{
			p_Meshes.emplace_back();
			std::vector<Vertex>& vertices = p_Meshes.back().p_Vertices;
			std::vector<GLuint>& indices = p_Meshes.back().p_Indices;

			// Top Face
			vertices.push_back({ glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(texture_coordinates[2][0], texture_coordinates[2][1]) });
			vertices.push_back({ glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(texture_coordinates[2][2], texture_coordinates[2][3]) });
			vertices.push_back({ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(texture_coordinates[2][4], texture_coordinates[2][5]) });
			vertices.push_back({ glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(texture_coordinates[2][6], texture_coordinates[2][7]) });
			indices = { 0,1,2,2,3,0 };

			p_CanFacecull = false; 

			// Calculate tangent normals
			CalculateTangentNormals();

			// Upload the data to the gpu
			Buffer();
		}

		PlaneObject()
		{
			p_Meshes.emplace_back();
			std::vector<Vertex>& vertices = p_Meshes.back().p_Vertices;
			std::vector<GLuint>& indices = p_Meshes.back().p_Indices;

			// Top Face
			vertices.push_back({ glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 1.0f) });
			vertices.push_back({ glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.0f) });
			vertices.push_back({ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 0.0f) });
			vertices.push_back({ glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 1.0f) });
			indices = { 0,1,2,2,3,0 };

			p_CanFacecull = false;

			// Calculate tangent normals
			CalculateTangentNormals();

			// Upload the data to the gpu
			Buffer();
		}
	};
}