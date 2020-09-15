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

		/*
		To be called once every frame (Before the rendering)
		It updates the view port and clears the framebuffer.

		width and height should be the dimensions of the window
		*/
		void OnUpdate(int width, int height) 
		{
			this->Bind();

			if (width != m_FBWidth || height != m_FBHeight)
			{
				glDeleteFramebuffers(1, &m_FBO);
				CreateFramebuffer(width, height);
				m_FBWidth = width;
				m_FBHeight = height;

				glViewport(0, 0, width, height);
			}
			
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
		void CreateFramebuffer(int w, int h);

		GLuint m_FBO; // The Framebuffer object
		GLuint m_TextureAttachment; // The actual texture attachment
		GLuint m_DepthStencilBuffer;
		int m_FBWidth;
		int m_FBHeight;
	};
}