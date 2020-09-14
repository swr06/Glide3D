#pragma once

#include <iostream>
#include <string>
#include <glad/glad.h>
#include "../Application/Logger.h"

namespace GLClasses
{
	class Framebuffer
	{
	public :
		Framebuffer(unsigned int w, unsigned int h);
		~Framebuffer();

		void Bind() const
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		}

		GLuint GetTexture() const 
		{
			return m_TextureAttachment;
		}

		GLuint GetDepthStencilBuffer() const
		{
			return m_DepthStencilBuffer;
		}

	private :
		GLuint m_FBO; // The Framebuffer object
		GLuint m_TextureAttachment; // The actual texture attachment
		GLuint m_DepthStencilBuffer;
	};
}