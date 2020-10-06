#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include "../GL_Classes/Framebuffer.h"

namespace Glide3D
{
	struct DirectionalLight
	{
		DirectionalLight() : m_DepthBuffer(2048, 2048) {}

		glm::vec3 m_Direction = glm::vec3(1.0f);
		glm::vec3 m_SpecularColor = glm::vec3(1.0f);
		glm::vec3 m_ShadowPosition = glm::vec3(1.0f); // Places the light at this position to render the shadow or depth map
		float m_SpecularStrength = 1.0f;
		int m_SpecularExponent = 32;
		bool m_IsBlinn = true;
		GLClasses::DepthBuffer m_DepthBuffer;
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