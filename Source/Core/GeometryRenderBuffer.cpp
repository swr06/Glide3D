#include "GeometryRenderBuffer.h"

namespace Glide3D
{
	GeometryRenderBuffer::GeometryRenderBuffer(uint32_t w, uint32_t h)
	{
		m_Width = w;
		m_Height = h;
		GenerateFramebuffers();
	}

	GeometryRenderBuffer::~GeometryRenderBuffer()
	{
		DeleteEverything();
	}

	void GeometryRenderBuffer::SetDimensions(uint32_t w, uint32_t h)
	{
		if (w != m_Width || h != m_Height)
		{
			DeleteEverything();
			m_Width = w;
			m_Height = h;
			GenerateFramebuffers();
		}
	}

	void GeometryRenderBuffer::GenerateFramebuffers()
	{
		GLenum DrawBuffers[4] = {
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2, 
			GL_COLOR_ATTACHMENT3 // Metalness, Roughness and AO
		};

		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		glGenTextures(1, &m_PositionTexture);
		glBindTexture(GL_TEXTURE_2D, m_PositionTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PositionTexture, 0);

		glGenTextures(1, &m_NormalTexture);
		glBindTexture(GL_TEXTURE_2D, m_NormalTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_NormalTexture, 0);

		glGenTextures(1, &m_ColorTexture);
		glBindTexture(GL_TEXTURE_2D, m_ColorTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_ColorTexture, 0);

		glGenTextures(1, &m_PBRComponentTexture);
		glBindTexture(GL_TEXTURE_2D, m_PBRComponentTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_PBRComponentTexture, 0);
		
		glGenRenderbuffers(1, &m_DepthRenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthRenderBuffer);
		glDrawBuffers(4, DrawBuffers);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Logger::Log("Geometry Render buffer is not complete!\n\tWIDTH : " +
				std::to_string(m_Width) + "\n\tHEIGHT : " + std::to_string(m_Height));
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GeometryRenderBuffer::DeleteEverything()
	{
		glDeleteFramebuffers(1, &m_FBO);
		glDeleteTextures(1, &m_ColorTexture);
		glDeleteTextures(1, &m_PositionTexture);
		glDeleteTextures(1, &m_NormalTexture);
		glDeleteTextures(1, &m_PBRComponentTexture);

		m_FBO = 0;
		m_ColorTexture = 0;
		m_PositionTexture = 0;
		m_NormalTexture = 0;
		m_PBRComponentTexture = 0;
	}
}