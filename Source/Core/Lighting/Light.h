#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include "../GL_Classes/Framebuffer.h"
#include "../GL_Classes/CubeDepthMap.h"

namespace Glide3D
{
	struct OrthogonalProperties
	{
		float left;
		float right;
		float bottom;
		float top;
		float znear;
		float zfar;
	};

	typedef std::pair<uint32_t, uint32_t> ShadowBufferResolution;

	struct DirectionalLight
	{
		DirectionalLight(const OrthogonalProperties& ortho_properties = { -60.0f, 60.0f, -60.0f, 60.0f, 0.15f, 60.0f }
			, const ShadowBufferResolution& shadow_res = {(int)4096, (int)4096})
			: m_DepthBuffer(shadow_res.first, shadow_res.second)
		{ 
			m_OrthagonalProperties = ortho_properties;
			m_LightSpaceProjection = glm::ortho(m_OrthagonalProperties.left,
				m_OrthagonalProperties.right,
				m_OrthagonalProperties.bottom,
				m_OrthagonalProperties.top,
				m_OrthagonalProperties.znear,
				m_OrthagonalProperties.zfar);
		}

		glm::vec3 m_Direction = glm::vec3(1.0f);
		glm::vec3 m_SpecularColor = glm::vec3(1.0f);
		glm::vec3 m_ShadowPosition = glm::vec3(1.0f); // Places the light at this position to render the shadow or depth map
		float m_SpecularStrength = 1.0f;
		int m_SpecularExponent = 4.0f;
		bool m_IsBlinn = true;
		uint32_t m_UpdateRate = 4;
		GLClasses::DepthBuffer m_DepthBuffer;

		glm::mat4 m_LightSpaceViewProjection;
		glm::mat4 m_LightSpaceProjection;
		glm::mat4 m_LightSpaceView;
		OrthogonalProperties m_OrthagonalProperties;
	};

	class PointLight
	{
	public :
		/*PointLight(GLuint res) : m_ShaderCubeMap(res)
		{

		}*/

		glm::vec3 m_Position = glm::vec3(1.0f);
		glm::vec3 m_SpecularColor = glm::vec3(1.0f);
		float m_Constant = 1.0f;
		float m_Linear = 0.0014;
		float m_Quadratic = 0.000007f;
		float m_SpecularStrength = 1.0f;
		int m_SpecularExponent = 32;
		bool m_IsBlinn = true;

		//GLClasses::CubeDepthMap m_ShaderCubeMap;
	};
}