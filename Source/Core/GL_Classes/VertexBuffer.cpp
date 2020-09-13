#include "VertexBuffer.h"

namespace GLClasses
{
	VertexBuffer::VertexBuffer(GLenum type)
	{
		this->buffer_id = 0;
		this->type = type;
		GLCall(glGenBuffers(1, &(this->buffer_id)));
		//this->Bind();
	}

	VertexBuffer::~VertexBuffer()
	{
		GLCall(glDeleteBuffers(1, &(this->buffer_id)));
		this->Unbind();
	}


	void VertexBuffer::BufferData(GLsizeiptr size, void* data, GLenum usage)
	{
		this->Bind();
		GLCall(glBufferData(this->type, size, data, usage));
	}

	void VertexBuffer::BufferSubData(GLintptr offset, GLsizeiptr size, void* data)
	{
		this->Bind();
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
	}

	void VertexBuffer::Bind()
	{
		GLCall(glBindBuffer(this->type, buffer_id));
	}

	void VertexBuffer::Unbind()
	{
		GLCall(glBindBuffer(this->type, 0));
	}

	void VertexBuffer::VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
	{
		this->Bind();
		GLCall(glVertexAttribPointer(index, size, type, normalized, stride, pointer));
		GLCall(glEnableVertexAttribArray(index));
	}

	void VertexBuffer::VertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer)
	{
		this->Bind();
		GLCall(glVertexAttribIPointer(index, size, type, stride, pointer));
		GLCall(glEnableVertexAttribArray(index));
	}
}