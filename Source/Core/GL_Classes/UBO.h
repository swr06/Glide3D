#pragma once

#include <glad/glad.h>
#include <iostream>

namespace Glide3D
{
	// Uniform buffer object
	class UBO
	{
	public :
		UBO(GLsizeiptr size);
		~UBO();

		UBO(const UBO&) = delete;
		UBO operator=(UBO const&) = delete;
		UBO(UBO&& v)
		{
			m_UBO = v.m_UBO;
			v.m_UBO = 0;
		}

		void Bind() const
		{
			glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
		}

		void BindBase(GLuint index)  const
		{
			Bind();
			glBindBufferBase(GL_UNIFORM_BUFFER, 2, m_UBO);
		}

		GLuint GetID() const 
		{
			return m_UBO;
		}

		void Unbind() const
		{
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		void BufferData(void* data, GLintptr offset, GLsizeiptr size)
		{
			Bind();
			glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
		}

	private : 
		GLuint m_UBO;
	};
}