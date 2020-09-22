#pragma once
#include <glm/glm.hpp>

namespace Glide3D
{
	/*
	1) Position ~ 12 bytes
	2) Normals ~ 12 bytes
	3) Texture UV Coordinates ~ 8 Bytes 
	4) Tangent ~ 12 bytes
	5) Bi Tangent ~ 12 bytes
	Total : 56 bytes per vertex
	*/

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normals;
		glm::vec2 tex_coords;
		glm::vec3 tangent;
		glm::vec3 bitangent;
	};
}