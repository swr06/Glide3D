#include "CubeDepthMap.h"

namespace GLClasses
{
	CubeDepthMap::CubeDepthMap(GLuint res)
	{
		m_Dim = res;
		glGenFramebuffers(1, &m_FBO);
		glGenTextures(1, &m_CubemapDepthTexture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapDepthTexture);

		for (int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, res, res, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		}
		
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glViewport(0, 0, res, res);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_CubemapDepthTexture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}