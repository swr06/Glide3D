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
				m_FBO = 0;
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

		inline unsigned int GetWidth() const noexcept { return m_FBWidth; }
		inline unsigned int GetHeight() const noexcept { return m_FBHeight; }

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
		DepthBuffer(unsigned int w, unsigned int h);
		~DepthBuffer();
		DepthBuffer(const DepthBuffer&) = delete;
		DepthBuffer operator=(DepthBuffer const&) = delete;

		DepthBuffer& operator=(DepthBuffer&& other) {
			std::swap(*this, other); // will re-use the move constructor
			return *this;
		}

		DepthBuffer(DepthBuffer&& v)
		{
			std::swap(m_DepthMap, v.m_DepthMap);
			m_DepthMapFBO = v.m_DepthMapFBO;
			m_Width = v.m_Width;
			m_Height = v.m_Height;

			v.m_DepthMap = 0;
			v.m_DepthMapFBO = 0;
		}

		inline GLuint GetDepthTexture() const
		{
			return m_DepthMap;
		}

		void Bind() const
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
			glViewport(0, 0, m_Width, m_Height);
		}

		void Unbind() const
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void OnUpdate()
		{
			Bind();
			glClear(GL_DEPTH_BUFFER_BIT);
		}

		inline unsigned int GetWidth() const noexcept { return m_Width; }
		inline unsigned int GetHeight() const noexcept { return m_Height; }

	private:
		GLuint m_DepthMap = 0;
		GLuint m_DepthMapFBO = 0;
		int m_Width = 0;
		int m_Height = 0;
	};
}