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
	enum class TextureType
	{
		Albedo,
		Normal,
		Specular
	};

	class Mesh
	{
	public : 
		Mesh();
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
		GLClasses::VertexBuffer p_MatrixBuffer; // Internal. Should not be accessed
		GLClasses::IndexBuffer p_IndexBuffer;

		std::uint32_t p_VertexCount = 0;
		std::uint32_t p_IndicesCount = 0;
		bool p_Indexed = false;

		glm::vec4 p_Color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	};

	/*
	Has a texture, light map, albedo and normal map
	*/
	class Object
	{
	public:
		Object(bool has_reflection_cubemap = false, uint32_t reflection_cubemap_res = 128, uint32_t reflection_map_update_rate = 32);
		~Object();

		/*
		Uploads the vertex data to the GPU
		*/
		void Buffer();

		void AddTextureMapToMesh(const std::string& path, TextureType tex_type, bool flip = false);

		/*
		Calculates the tangent and bitangents based on the texture coordinates to en
		*/
		void CalculateTangentNormals();

		std::vector<Mesh> p_Meshes;
		bool p_CanFacecull = false;
		float p_Reflectance = 0.0f; // Specifies the reflectance of the object. 0.0 being non reflective and 1.0 being fully reflective
		
		// The cube reflection map to reflect the in game scenes
		bool p_RenderReflectionCubemap = false;
		GLClasses::CubeReflectionMap p_ReflectionCubemap;
		uint32_t p_ReflectionMapUpdateRate = 30;
	};
}