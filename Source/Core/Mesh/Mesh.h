#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../Application/Logger.h"
#include "../Utils/Vertex.h"
#include "../Utils/Defs.h"
#include "../GL_Classes/Texture.h"
#include "../GL_Classes/VertexBuffer.h"
#include "../GL_Classes/IndexBuffer.h"
#include "../GL_Classes/VertexArray.h"
#include "../GL_Classes/CubeReflectionMap.h"
#include <glad/glad.h>

namespace Glide3D
{
	class Mesh;

	class Mesh
	{
	public:
		Mesh(const GLClasses::VertexBuffer& MatrixVBO);
		void Buffer();
		void CalculateTangentNormals();

		std::vector<Vertex> p_Vertices;
		std::vector<GLuint> p_Indices;
		std::string p_MeshName;
		GLClasses::Texture p_LightMap;
		GLClasses::Texture p_AlbedoMap;
		GLClasses::Texture p_NormalMap;

		GLClasses::VertexBuffer p_VertexBuffer;
		GLClasses::VertexArray p_VertexArray;
		GLClasses::IndexBuffer p_IndexBuffer;

		std::uint32_t p_VertexCount = 0;
		std::uint32_t p_IndicesCount = 0;
		bool p_Indexed = false;

		glm::vec4 p_Color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		float p_Reflectivity = 0.0f;
	};

}