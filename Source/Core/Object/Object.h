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
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Glide3D
{
	class Object;
	
	namespace FileLoader
	{
		void ProcessAssimpMesh(aiMesh* mesh, const aiScene* scene, Object* object, const std::string& pth, const glm::vec4& col, const glm::vec3& reflectivity);
	}

	enum class TextureType
	{
		Albedo,
		Normal,
		Specular,
		Metalness,
		Roughness,
		AO // Ambient Occlusion
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

		bool p_CanFacecull = true;
		ReflectionMapProperties p_ReflectionProps;

		inline uint32_t GetID() const noexcept { return m_ObjectID; }

	private :
		void CalculateCenter();
		const uint32_t m_ObjectID;

		std::vector<Mesh> m_Meshes;
		friend class Renderer;
		friend class Application;
		friend class Entity;

		friend void FileLoader::ProcessAssimpMesh(aiMesh* mesh, const aiScene* scene, Object* object, const std::string& pth, const glm::vec4& col, const glm::vec3& reflectivity);
		GLClasses::VertexBuffer m_MatrixBuffer;
		glm::vec3 m_Center; // The center of the mesh in localized space

	};
}