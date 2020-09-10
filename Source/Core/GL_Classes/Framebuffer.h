#pragma once

#include <iostream>
#include <string>
#include <glad/glad.h>

/*
	Todo ! Highly wip
*/

namespace GLClasses
{
	class Framebuffer
	{
	public :
		Framebuffer(unsigned int w, unsigned int h);
		~Framebuffer();

		inline void Bind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		}

	private :
		GLuint m_FBO;
		GLuint m_TextureAttachment;
	};
}