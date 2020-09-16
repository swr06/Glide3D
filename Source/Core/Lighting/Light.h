#pragma once

#include <glm/glm.hpp>
#include <iostream>

namespace Glide3D
{
	struct DirectionalLight
	{
		glm::vec3 m_Position;
		float m_SpecularStrength;
		int m_SpecularExponent;
	};
}