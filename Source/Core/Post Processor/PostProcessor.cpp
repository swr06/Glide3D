#include "PostProcessor.h"

namespace Glide3D
{
	PostProcessingPipeline::PostProcessingPipeline(uint32_t w, uint32_t h) : m_Framebuffer(w, h, true, false), m_FBOVBO(GL_ARRAY_BUFFER)
	{
		// basic quad vertices
		float Vertices[] =
		{
			-1.0f,  1.0f,  0.0f, 1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 1.0f, -1.0f,  1.0f, 
			0.0f, -1.0f, 1.0f,  0.0f, 1.0f, 1.0f, -1.0f,  1.0f, 0.0f,  1.0f,  1.0f,  1.0f, 1.0f
		};

		m_FBOVBO.BufferData(sizeof(Vertices), Vertices, GL_STATIC_DRAW);
		m_FBOVAO.Bind();
		m_FBOVBO.Bind();
		m_FBOVBO.VertexAttribPointer(0, 2, GL_FLOAT, 0, 4 * sizeof(GLfloat), 0);
		m_FBOVBO.VertexAttribPointer(1, 2, GL_FLOAT, 0, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
		m_FBOVAO.Unbind();

		m_BlitShader.CreateShaderProgramFromFile("Core/Shaders/FramebufferVert.glsl", "Core/Shaders/FramebufferFrag.glsl");
		m_BlitShader.CompileShaders();
	}

	void PostProcessingPipeline::AddShaderToPipeline(const GLClasses::Shader& shader)
	{
		m_Shaders.push_back(shader.GetProgramID());
	}

	void PostProcessingPipeline::StartPipeline(GLClasses::Framebuffer& fbo)
	{
		const GLuint FBO_Texture = fbo.GetTexture();
		const GLuint SwapFBO_Texture = m_Framebuffer.GetTexture();
		const GLuint FBO = fbo.GetFramebuffer();
		const GLuint SwapFBO = m_Framebuffer.GetFramebuffer();

		GLuint Texture = 0;
		GLuint Framebuffer = 0;

		Texture = FBO_Texture;
		Framebuffer = SwapFBO;

		for (int i = 0; i < m_Shaders.size(); i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			glUseProgram(m_Shaders[i]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, Texture);

			// Set the texture uniform
			GLuint loc = 0;
			loc = glGetUniformLocation(m_Shaders[i], "u_Texture");
			glUniform1i(loc, 1);

			m_FBOVAO.Bind();
			glDrawArrays(GL_TRIANGLES, 0, 6);
			m_FBOVAO.Unbind();

			if (Texture == FBO_Texture) 
			{ 
				Texture = SwapFBO_Texture;
			}

			else 
			{ 
				Texture = FBO_Texture;
			}

			if (Framebuffer == FBO)
			{
				Framebuffer = SwapFBO;
			}

			else
			{
				Framebuffer = FBO;
			}
			
			glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(0);
		}

		// If the shader size, was not even, blit the other framebuffer to FBO
		if (m_Shaders.size() % 2 != 0)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);
			glDisable(GL_CULL_FACE);
			m_BlitShader.Use();

			m_BlitShader.SetInteger("u_FramebufferTexture", 1);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, SwapFBO_Texture);

			m_FBOVAO.Bind();
			glDrawArrays(GL_TRIANGLES, 0, 6);
			m_FBOVAO.Unbind();

			glUseProgram(0);
		}

		m_Framebuffer.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_Framebuffer.Unbind();
	}

	void PostProcessingPipeline::SetSize(uint32_t w, uint32_t h)
	{
		m_Framebuffer.SetSize(w, h);
	}
}
