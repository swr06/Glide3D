#include "PostProcessor.h"

namespace Glide3D
{
	PostProcessingPipeline::PostProcessingPipeline(uint32_t w, uint32_t h) : m_Framebuffer(w, h), m_FBOVBO(GL_ARRAY_BUFFER)
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
	}

	void PostProcessingPipeline::AddShaderToPipeline(const GLClasses::Shader& shader)
	{
		m_Shaders.push_back(shader.GetProgramID());
	}

	void PostProcessingPipeline::StartPipeline(GLClasses::Framebuffer& fbo)
	{
		GLuint Texture;

		for (int i = 0; i < m_Shaders.size(); i++)
		{

		}
	}

	void PostProcessingPipeline::SetSize(uint32_t w, uint32_t h)
	{
		m_Framebuffer.SetSize(w, h);
	}
}
