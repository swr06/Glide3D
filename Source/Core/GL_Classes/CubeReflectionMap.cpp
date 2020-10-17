#include "CubeReflectionMap.h"

namespace GLClasses
{
	CubeReflectionMap::CubeReflectionMap(GLuint resolution) : m_Resolution(resolution)
	{
		// Create the cubemap texture
		glGenTextures(1, &m_CubemapTexture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapTexture);

		for (int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, resolution, resolution, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		// Generate the framebuffer 
		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		// Generate the depth buffers
		GLuint depth_buffer = 0;
		glGenRenderbuffers(1, &depth_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, resolution, resolution);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);
		glViewport(0, 0, resolution, resolution);

		// Unbind the stuff
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
