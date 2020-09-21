#include "Object.h"

namespace Glide3D
{
	Object::Object() : p_DefaultColor(glm::vec3(1.0f, 0.5f, 0.31f)), p_VertexBuffer(GL_ARRAY_BUFFER), p_MatrixBuffer(GL_ARRAY_BUFFER)
	{
		p_Texture = new GLClasses::Texture();
		p_LightMap = new GLClasses::Texture();
		p_AlbedoMap = new GLClasses::Texture();
		p_NormalMap = new GLClasses::Texture();

		/*
		Setup all the ogl objects
		*/
		p_VertexArray.Bind();
		p_VertexBuffer.Bind();
		p_IndexBuffer.Bind();
		p_VertexBuffer.VertexAttribPointer(0, 3, GL_FLOAT, 0, sizeof(Vertex), (void*)(offsetof(Vertex, position)));
		p_VertexBuffer.VertexAttribPointer(1, 3, GL_FLOAT, 0, sizeof(Vertex), (void*)(offsetof(Vertex, normals)));
		p_VertexBuffer.VertexAttribPointer(2, 2, GL_FLOAT, 0, sizeof(Vertex), (void*)(offsetof(Vertex, tex_coords)));
		p_MatrixBuffer.Bind();

		constexpr float matrix_stride = 32 * sizeof(GLfloat);

		// Structure padding shouldn't be an issue since the size is 32 * 4 bytes 
		p_MatrixBuffer.VertexAttribPointer(3, 4, GL_FLOAT, 0, matrix_stride, (void*)0); // column 1
		p_MatrixBuffer.VertexAttribPointer(4, 4, GL_FLOAT, 0, matrix_stride, (void*)(sizeof(GLfloat) * 4)); // column 2
		p_MatrixBuffer.VertexAttribPointer(5, 4, GL_FLOAT, 0, matrix_stride, (void*)(sizeof(GLfloat) * 8)); // column 3
		p_MatrixBuffer.VertexAttribPointer(6, 4, GL_FLOAT, 0, matrix_stride, (void*)(sizeof(GLfloat) * 12)); // column 4

		// Normal matrices								   
		p_MatrixBuffer.VertexAttribPointer(7, 4, GL_FLOAT, 0, matrix_stride, (void*)(sizeof(GLfloat) * 16)); // column 1
		p_MatrixBuffer.VertexAttribPointer(8, 4, GL_FLOAT, 0, matrix_stride, (void*)(sizeof(GLfloat) * 20)); // column 2
		p_MatrixBuffer.VertexAttribPointer(9, 4, GL_FLOAT, 0, matrix_stride, (void*)(sizeof(GLfloat) * 24)); // column 3
		p_MatrixBuffer.VertexAttribPointer(10, 4, GL_FLOAT, 0, matrix_stride, (void*)(sizeof(GLfloat) * 28)); // column 4

		// Set the matrix attributes to be per-instance
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);
		glVertexAttribDivisor(8, 1);
		glVertexAttribDivisor(9, 1);
		glVertexAttribDivisor(10, 1);

		p_VertexArray.Unbind();
	}

	Object::~Object()
	{
		delete p_Texture;
		delete p_LightMap;
		delete p_AlbedoMap;
		delete p_NormalMap;
	}

	void Object::Buffer()
	{
		if (p_Vertices.size() > 0)
		{
			p_VertexCount = p_Vertices.size();
			p_VertexBuffer.BufferData(p_Vertices.size() * sizeof(Vertex), &p_Vertices.front(), GL_STATIC_DRAW);
			p_Vertices.clear();
		}

		if (p_Indices.size() > 0)
		{
			p_IndicesCount = p_Indices.size();
			p_IndexBuffer.BufferData(p_Indices.size() * sizeof(GLuint), &p_Indices.front(), GL_STATIC_DRAW);
			p_Indexed = true;
			p_Indices.clear(); 
		}

		else
		{
			p_Indexed = false;
		}
	}
}