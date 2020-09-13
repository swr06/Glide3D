#include "Renderer.h"

namespace Glide3D
{
	constexpr unsigned int indices_count = 10000;

	Renderer::Renderer() : m_VBO(GL_ARRAY_BUFFER), m_MatrixVBO(GL_ARRAY_BUFFER)
	{
		bool IndexBufferInitialized = false;

		if (IndexBufferInitialized == false)
		{
			IndexBufferInitialized = true;

			GLuint* IndexBuffer = nullptr;

			int index_size = 1;
			int index_offset = 0;

			IndexBuffer = new GLuint[index_size * 6];

			for (size_t i = 0; i < index_size; i += 6)
			{
				IndexBuffer[i] = 0 + index_offset;
				IndexBuffer[i + 1] = 1 + index_offset;
				IndexBuffer[i + 2] = 2 + index_offset;
				IndexBuffer[i + 3] = 2 + index_offset;
				IndexBuffer[i + 4] = 3 + index_offset;
				IndexBuffer[i + 5] = 0 + index_offset;

				index_offset = index_offset + 4;
			}

			m_IBO.BufferData(index_size * 6 * sizeof(GLuint), IndexBuffer, GL_STATIC_DRAW);

			delete[] IndexBuffer;
		}

		m_VAO.Bind();
		m_VBO.Bind();
		m_VBO.VertexAttribPointer(0, 3, GL_FLOAT, 0, sizeof(Vertex), (void*)(offsetof(Vertex, position)));
		m_VBO.VertexAttribPointer(1, 2, GL_FLOAT, 0, sizeof(Vertex), (void*)(offsetof(Vertex, tex_coords)));
		m_MatrixVBO.Bind();
		m_MatrixVBO.VertexAttribPointer(2, 4, GL_FLOAT, 0, 4 * 4 * sizeof(GLfloat), (void*)0); // column 1
		m_MatrixVBO.VertexAttribPointer(3, 4, GL_FLOAT, 0, 4 * 4 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 4)); // column 2
		m_MatrixVBO.VertexAttribPointer(4, 4, GL_FLOAT, 0, 4 * 4 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 8)); // column 3
		m_MatrixVBO.VertexAttribPointer(5, 4, GL_FLOAT, 0, 4 * 4 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 12)); // column 4
		m_VAO.Unbind();
	}

	void Renderer::RenderObjects(const std::vector<Entity>& entities)
	{
		unsigned int entity_num = 0;

		std::vector<Vertex> Vertices;
		std::vector<glm::mat4> ModelMatrices;

		Vertices = entities[0].p_Object->p_Vertices;

		for (auto& e : entities)
		{
			ModelMatrices.push_back(e.p_Transform.GetTransformationMatrix());
			entity_num++;
		}

		m_VBO.BufferData(Vertices.size() * sizeof(Vertex), &Vertices.front(), GL_STATIC_DRAW);
		m_MatrixVBO.BufferData(ModelMatrices.size() * 4 * 4 * sizeof(GLfloat), &ModelMatrices.front(), GL_STATIC_DRAW);
		m_VAO.Bind();
		glDrawElementsInstanced(GL_ARRAY_BUFFER, 6, GL_UNSIGNED_INT, 0, entities.size());
		m_VAO.Unbind();

		return;
	}
}
