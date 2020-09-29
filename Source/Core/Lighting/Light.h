#pragma once

#include <glm/glm.hpp>
#include <iostream>

namespace Glide3D
{
	struct DirectionalLight
	{
		glm::vec3 m_Direction = glm::vec3(1.0f);
		glm::vec3 m_SpecularColor = glm::vec3(1.0f);
		float m_SpecularStrength = 1.0f;
		int m_SpecularExponent = 32;
		bool m_IsBlinn = true;
	};

	struct PointLight
	{
		glm::vec3 m_Position = glm::vec3(1.0f);
		glm::vec3 m_SpecularColor = glm::vec3(1.0f);
		float m_Constant = 1.0f;
		float m_Linear = 0.0014;
		float m_Quadratic = 0.000007f;
		float m_SpecularStrength = 1.0f;
		int m_SpecularExponent = 32;
		bool m_IsBlinn = true;
	};
}