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
		Mesh(const GLClasses::VertexBuffer& MatrixVBO, const uint32_t number);
		void Buffer();
		void CalculateTangentNormals();

		std::vector<Vertex> p_Vertices;
		std::vector<GLuint> p_Indices;
		std::string p_MeshName;
		GLClasses::Texture p_LightMap;
		GLClasses::Texture p_AlbedoMap;
		GLClasses::Texture p_NormalMap;

		// PBR
		GLClasses::Texture p_MetalnessMap;
		GLClasses::Texture p_RoughnessMap;
		GLClasses::Texture p_AmbientOcclusionMap;

		GLClasses::VertexBuffer p_VertexBuffer;
		GLClasses::VertexArray p_VertexArray;
		GLClasses::IndexBuffer p_IndexBuffer;

		std::uint32_t p_VertexCount = 0;
		std::uint32_t p_IndicesCount = 0;
		bool p_Indexed = false;

		glm::vec4 p_Color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec3 p_Reflectivity = glm::vec3(0.0f);
		std::string p_Name = std::string("");
		const uint32_t p_MeshNumber;
		
		/* Determines where in the mesh the basic wave effect should be the most
		the effect is calculated with distance(position.y, p_ClothAffectY)
		
		The further the vertex is away from this position, the more the impact
		If this variable is 0 then the effect is applied to all the vertices of that mesh
		*/
		float p_WaveAffectY = 0.0f; 

		glm::vec2 p_WaveAffectFreq;
		glm::vec2 p_WaveAffectSpeed; 
		bool p_HasWavePhysics = false;
	};

}