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
			glViewport(0, 0, m_FBWidth, m_FBHeight);
		}


		void Unbind() const
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		/*
		To be called once every frame (Before the rendering)
		It updates the view port and clears the framebuffer.

		width and height should be the dimensions of the window
		*/
		void OnUpdate(int width, int height, bool wireframe = false) 
		{
			this->Bind();

			if (wireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}

			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			if (width != m_FBWidth || height != m_FBHeight)
			{
				glDeleteFramebuffers(1, &m_FBO);
				CreateFramebuffer(width, height);
				m_FBWidth = width;
				m_FBHeight = height;

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

	/*
	Can be used as a shadow map
	*/
	class DepthBuffer
	{
	public :
	public:
		DepthBuffer(unsigned int w, unsigned int h);
		~DepthBuffer();

		inline GLuint GetDepthTexture() const
		{
			return m_DepthMap;
		}

		void Bind() const
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
		}

		void Unbind() const
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void OnUpdate()
		{
			Bind();
			glViewport(0, 0, m_Width, m_Height);
			glClear(GL_DEPTH_BUFFER_BIT);
		}

	private:
		GLuint m_DepthMap = 0;
		GLuint m_DepthMapFBO = 0;
		int m_Width = 0;
		int m_Height = 0;
	};
}