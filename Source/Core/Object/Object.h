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

#include "../Mesh/Mesh.h"

namespace Glide3D
{
	enum class TextureType
	{
		Albedo,
		Normal,
		Specular
	};

	struct ReflectionMapProperties
	{
		uint32_t res;
		uint32_t update_rate;
	};

	/*
	Has a texture, light map, albedo and normal map
	*/
	class Object
	{
	public:
		Object(const ReflectionMapProperties& props = {128, 30});
		~Object();

		//Uploads the vertex data to the GPU
		void Buffer();

		//Calculates the tangent and bitangents based on the texture coordinates to en
		void CalculateTangentNormals();
		void AddTextureMapToMesh(const std::string& path, TextureType tex_type, bool flip = false);
		
		// Generates a mesh and returns a reference to that mesh
		Mesh& GenerateMesh();

		std::vector<Mesh> p_Meshes;

		bool p_CanFacecull = true;
		ReflectionMapProperties p_ReflectionProps;
		glm::vec3 p_Center; // The center of the mesh in localized space
		GLClasses::VertexBuffer p_MatrixBuffer; // Internal. Should not be accessed

	private :
		void CalculateCenter();
	};
}