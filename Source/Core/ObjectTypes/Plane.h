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
			// Top Face
			p_Vertices.push_back({ glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(texture_coordinates[2][0], texture_coordinates[2][1]) });
			p_Vertices.push_back({ glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(texture_coordinates[2][2], texture_coordinates[2][3]) });
			p_Vertices.push_back({ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(texture_coordinates[2][4], texture_coordinates[2][5]) });
			p_Vertices.push_back({ glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(texture_coordinates[2][6], texture_coordinates[2][7]) });
			
			p_CanFacecull = false; 

			// Upload the data to the gpu
			Buffer();
		}
	};
}