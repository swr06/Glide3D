#pragma once

#include <glm/glm.hpp>
#include <iostream>

namespace Glide3D
{
	struct DirectionalLight
	{
		glm::vec3 m_Position = glm::vec3(1.0f);
		float m_SpecularStrength = 0;
		int m_SpecularExponent = 0;
	};

	struct PointLight
	{
		glm::vec3 m_Position;
		float m_Constant;
		float m_Linear;
		float m_Quadratic;
		float m_SpecularStrength;
		int m_SpecularExponent;
	};
}