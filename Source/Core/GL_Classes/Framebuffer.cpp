#include "Framebuffer.h"

#ifndef NULL
	#define NULL 0
#endif

namespace GLClasses
{
    Framebuffer::Framebuffer(unsigned int w, unsigned int h) : 
        m_FBO(0), m_FBWidth(w), m_FBHeight(h)
    {
        CreateFramebuffer(w, h);
    }

	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &m_FBO);
	}

	void Framebuffer::CreateFramebuffer(int w, int h)
	{
        glGenFramebuffers(1, &m_FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

        glGenTextures(1, &m_TextureAttachment);
        glBindTexture(GL_TEXTURE_2D, m_TextureAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureAttachment, 0);

        glGenRenderbuffers(1, &m_DepthStencilBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_DepthStencilBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilBuffer);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            Glide3D::Logger::Log("Fatal error! Framebuffer creation failed!");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

    /*
    Depth Map
    */

    DepthBuffer::DepthBuffer(unsigned int w, unsigned int h) : m_Width(w), m_Height(h)
    {
        glGenTextures(1, &m_DepthMap);
        glBindTexture(GL_TEXTURE_2D, m_DepthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float col[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, col);

        glGenFramebuffers(1, &m_DepthMapFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMap, 0);

        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            Glide3D::Logger::Log("Fatal error! DEPTH Framebuffer creation failed!");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    DepthBuffer::~DepthBuffer()
    {
        glDeleteFramebuffers(1, &m_DepthMapFBO);
    }
}