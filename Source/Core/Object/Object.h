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
		float p_Reflectance = 0.4f; // Specifies the reflectance of the object. 0.0 being non reflective and 1.0 being fully reflective
		ReflectionMapProperties p_ReflectionProps;
	};
}