#include "Framebuffer.h"

#ifndef NULL
	#define NULL 0
#endif

namespace GLClasses
{
	/*Framebuffer::Framebuffer(unsigned int w, unsigned int h) : m_FBO(0)
	{
        glGenFramebuffers(1, &m_FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

        GLuint m_DepthRenderBuffer;

        glGenTextures(1, &m_TextureAttachment);
        glBindTexture(GL_TEXTURE_2D, m_TextureAttachment);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        // Set the texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureAttachment, 0);

        glGenRenderbuffers(1, &m_DepthRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRenderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthRenderBuffer);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_TextureAttachment, 0);

        GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, DrawBuffers); 

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            Glide3D::Logger::Log("Fatal error! Framebuffer creation failed!");
        }
        
        // Unbind the framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}*/
    
    Framebuffer::Framebuffer(unsigned int w, unsigned int h) : m_FBO(0)
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

	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &m_FBO);
	}
}