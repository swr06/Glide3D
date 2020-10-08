#pragma once
#include <iostream>
#include <glad/glad.h>

namespace GLClasses
{
	// Shader storage buffer object
	class SSBO
	{
	public :
		SSBO();
		~SSBO();

		SSBO(const SSBO&) = delete;
		SSBO operator=(SSBO const&) = delete;
		SSBO(SSBO&& v)
		{
			m_SSBO = v.m_SSBO;
			v.m_SSBO = 0;
		}

		void Bind()
		{
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO);
		}

		void Unbind()
		{
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		}

		void BindBase(GLuint loc)
		{
			Bind();
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, loc, m_SSBO);
		}

		void BufferData(void* data, GLsizeiptr size, GLenum usage)
		{
			Bind();
			glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, usage);
		}

	private :
		GLuint m_SSBO;
	};
}