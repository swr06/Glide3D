#pragma once

#include <array>
#include "../Object/Object.h"

namespace Glide3D
{
	class PlaneObject : public Object
	{
	public:

		PlaneObject(const PlaneTextureCoordinates& texture_coordinates)
		{
			p_Vertices.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
			p_Vertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
			p_Vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
			p_Vertices.push_back(glm::vec3(0.0f, 1.0f, 1.0f));

			p_Normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
			p_Normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
			p_Normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
			p_Normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

			p_TextureCoords.push_back(glm::vec2(texture_coordinates[0], texture_coordinates[1]));
			p_TextureCoords.push_back(glm::vec2(texture_coordinates[2], texture_coordinates[3]));
			p_TextureCoords.push_back(glm::vec2(texture_coordinates[4], texture_coordinates[5]));
			p_TextureCoords.push_back(glm::vec2(texture_coordinates[6], texture_coordinates[7]));
		}
	};
}