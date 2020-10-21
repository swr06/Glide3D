#pragma once

#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cassert>
#include "../Application/Logger.h"

namespace GLClasses
{
	class CubeReflectionMap
	{
	public :

		CubeReflectionMap(GLuint resolution);

		CubeReflectionMap(const CubeReflectionMap&) = delete;
		CubeReflectionMap operator=(CubeReflectionMap const&) = delete;

		CubeReflectionMap(CubeReflectionMap&& v)
		{
			m_FBO = v.m_FBO;
			m_CubemapTexture = v.m_CubemapTexture;
			m_Resolution = v.m_Resolution;
			m_DepthBuffer = v.m_DepthBuffer;
			v.m_FBO = 0;
			v.m_CubemapTexture = 0;
			v.m_DepthBuffer = 0;
		}

		void Bind() const
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
			glViewport(0, 0, m_Resolution, m_Resolution);
		}

		void BindFace(GLuint face) const
		{
			assert(!(face >= 6));
			assert(!(m_CubemapTexture == 0));

			Bind();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, m_CubemapTexture, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void Unbind() const
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}

		inline GLuint GetTexture() const
		{
			return m_CubemapTexture;
		}

		inline GLuint GetFramebuffer() const
		{
			return m_FBO;
		}

		inline GLuint GetResolution() const
		{
			return m_Resolution;
		}

	private : 
		GLuint m_FBO = 0;
		GLuint m_CubemapTexture = 0;
		GLuint m_DepthBuffer = 0;
		GLuint m_Resolution = 0;
	};
}