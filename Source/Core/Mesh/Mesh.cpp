#include "Mesh.h"

namespace Glide3D
{
	Mesh::Mesh(const uint32_t number) : p_VertexBuffer(GL_ARRAY_BUFFER), p_MeshNumber(number)
	{
		/*
		Setup all the ogl objects
		*/
		p_VertexArray.Bind();
		p_VertexBuffer.Bind();
		p_IndexBuffer.Bind();
		p_VertexBuffer.VertexAttribPointer(0, 3, GL_FLOAT, 0, sizeof(Vertex), (void*)(offsetof(Vertex, position)));
		p_VertexBuffer.VertexAttribPointer(1, 3, GL_FLOAT, 0, sizeof(Vertex), (void*)(offsetof(Vertex, normals)));
		p_VertexBuffer.VertexAttribPointer(2, 2, GL_FLOAT, 0, sizeof(Vertex), (void*)(offsetof(Vertex, tex_coords)));
		p_VertexBuffer.VertexAttribPointer(3, 3, GL_FLOAT, 0, sizeof(Vertex), (void*)(offsetof(Vertex, tangent)));
		p_VertexBuffer.VertexAttribPointer(4, 3, GL_FLOAT, 0, sizeof(Vertex), (void*)(offsetof(Vertex, bitangent)));
		p_VertexArray.Unbind();
	}

	void Mesh::Buffer()
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

	void Mesh::CalculateTangentNormals()
	{
		std::vector<Vertex>& vertices = p_Vertices;
		int tris = ceil(vertices.size() / 3);

		/* Calculate tangents and bi tangents */
		for (int i = 0; i < vertices.size(); i += 3)
		{
			if (i >= vertices.size()) { break; }

			const Vertex& v0 = vertices[i];
			const Vertex& v1 = vertices[i + 1];
			const Vertex& v2 = vertices[i + 2];

			glm::vec3 delta_p1 = v1.position - v0.position;
			glm::vec3 delta_p2 = v2.position - v0.position;

			glm::vec2 delta_uv1 = v1.tex_coords - v0.tex_coords;
			glm::vec2 delta_uv2 = v2.tex_coords - v0.tex_coords;

			float f = 1.0f / (delta_uv1.x * delta_uv2.y - delta_uv2.x * delta_uv1.y);

			vertices[i].tangent.x = f * (delta_uv2.y * delta_p1.x - delta_uv1.y * delta_p2.x);
			vertices[i].tangent.y = f * (delta_uv2.y * delta_p1.y - delta_uv1.y * delta_p2.y);
			vertices[i].tangent.z = f * (delta_uv2.y * delta_p1.z - delta_uv1.y * delta_p2.z);
			vertices[i].bitangent.x = f * (-delta_uv2.x * delta_p1.x + delta_uv1.x * delta_p2.x);
			vertices[i].bitangent.y = f * (-delta_uv2.x * delta_p1.y + delta_uv1.x * delta_p2.y);
			vertices[i].bitangent.z = f * (-delta_uv2.x * delta_p1.z + delta_uv1.x * delta_p2.z);
		}

		return;
	}
}