#pragma once

#include <glad/glad.h>

#include <iostream>
#include <vector>
#include "../GL_Classes/VertexBuffer.h"
#include "../GL_Classes/Shader.h"
#include "../GL_Classes/VertexArray.h"
#include "../GL_Classes/IndexBuffer.h"
#include "../GL_Classes/Framebuffer.h"

/* --------- Post processor -----------

attributes : a_Position, a_TexCoords
Each shader inserted to the pipe line is executed step by step. 
*/

namespace Glide3D
{
	class PostProcessingPipeline
	{
	public :
		PostProcessingPipeline(uint32_t w, uint32_t h);
		void AddShaderToPipeline(const GLClasses::Shader& shader);
		void StartPipeline(GLClasses::Framebuffer& fbo);
		void SetSize(uint32_t w, uint32_t h);

	private :
		std::vector<GLuint> m_Shaders;
		GLClasses::Framebuffer m_Framebuffer;
		GLClasses::VertexArray m_FBOVAO;
		GLClasses::VertexBuffer m_FBOVBO;
	};
}