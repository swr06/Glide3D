#include "VertexBuffer.h"

namespace GLClasses
{
	VertexBuffer::VertexBuffer(GLenum type)
	{
		this->buffer_id = 0;
		this->type = type;
		glGenBuffers(1, &(this->buffer_id));
		//this->Bind();
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &(this->buffer_id));
		this->Unbind();
	}


	void VertexBuffer::BufferData(GLsizeiptr size, void* data, GLenum usage) const
	{
		this->Bind();
		glBufferData(this->type, size, data, usage);
	}

	void VertexBuffer::BufferSubData(GLintptr offset, GLsizeiptr size, void* data) const
	{
		this->Bind();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(this->type, buffer_id);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(this->type, 0);
	}

	void VertexBuffer::VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer) const
	{
		this->Bind();
		glVertexAttribPointer(index, size, type, normalized, stride, pointer);
		glEnableVertexAttribArray(index);
	}

	void VertexBuffer::VertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer) const
	{
		this->Bind();
		glVertexAttribIPointer(index, size, type, stride, pointer);
		glEnableVertexAttribArray(index);
	}
}