#pragma once
#include <glm/glm.hpp>

namespace Glide3D
{
	/*
	1) Position ~ 12 bytes
	2) Normals ~ 12 bytes
	3) Texture UV Coordinates ~ 8 Bytes
	Total : 32 bytes
	*/

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normals;
		glm::vec2 tex_coords;
	};
}