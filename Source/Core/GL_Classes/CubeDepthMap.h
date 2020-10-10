#pragma once

#include <glad/glad.h>
#include <iostream>
#include <vector>

namespace Glide3D
{
	class CubeDepthMap
	{
	public : 

		CubeDepthMap(GLuint res);
		CubeDepthMap(const CubeDepthMap&) = delete;
		CubeDepthMap operator=(CubeDepthMap const&) = delete;

		CubeDepthMap& operator=(CubeDepthMap&& other)
		{
			std::swap(*this, other);
			return *this;
		}  

		CubeDepthMap(CubeDepthMap&& v)
		{
			m_FBO = v.m_FBO;
			m_CubemapDepthTexture = v.m_CubemapDepthTexture;

			v.m_FBO = 0;
			v.m_CubemapDepthTexture = 0;
		}

		inline void Bind() const noexcept
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
			glViewport(0, 0, m_Dim, m_Dim);
		}

		inline void Unbind() const noexcept
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		inline GLuint GetFramebuffer() const noexcept
		{
			return m_FBO;
		}

		inline GLuint GetTexture() const noexcept
		{
			return m_CubemapDepthTexture;
		}

		inline GLuint GetDimensions() const noexcept
		{
			return m_Dim;
		}


	private :
		GLuint m_CubemapDepthTexture;
		GLuint m_FBO;
		GLuint m_Dim;
	};
}