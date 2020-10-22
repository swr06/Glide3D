#include "Object.h"
#include <cstdlib>
#include <cstdio>

namespace Glide3D
{
	Object::Object(const ReflectionMapProperties& props) : p_ReflectionProps(props), p_MatrixBuffer(GL_ARRAY_BUFFER)
	{

	}

	Object::~Object()
	{

	}

	void Object::Buffer()
	{
		CalculateCenter();

		for (auto& e : p_Meshes)
		{
			e.Buffer();
		}
	}

	void Object::AddTextureMapToMesh(const std::string& path, TextureType tex_type, bool flip)
	{
		if (p_Meshes.size() > 0)
		{
			Mesh* mesh = &p_Meshes.back();

			switch (tex_type)
			{
			case TextureType::Albedo : 
			{
				mesh->p_AlbedoMap.CreateTexture(path, flip);
				break;
			}

			case TextureType::Specular:
			{
				mesh->p_LightMap.CreateTexture(path, flip);
				break;
			}

			case TextureType::Normal:
			{
				mesh->p_NormalMap.CreateTexture(path, flip);
				break;
			}

			default : 
			{
				Logger::Log("Invalid argument passed to Object::AddTextureToMesh()!");
				break;
			}
			}
		}

		else
		{
			Logger::Log("Can't add texture path : " + path + " When there are no meshes in the object!");
		}
	}

	Mesh& Object::GenerateMesh()
	{
		p_Meshes.emplace_back(p_MatrixBuffer);
		return p_Meshes.back();
	}

	void Object::CalculateTangentNormals()
	{
		for (auto& e : p_Meshes)
		{
			e.CalculateTangentNormals();
		}
	}

	void Object::CalculateCenter()
	{
		uint32_t count = 0;
		glm::vec3& center = this->p_Center;

		for (auto& e : p_Meshes)
		{
			count += e.p_VertexCount;
			
			for (auto& f : e.p_Vertices)
			{
				const glm::vec3& pos = f.position;
				center.x += pos.x;
				center.y += pos.y;
				center.z += pos.z;
			}
		}

		center.x /= count;
		center.y /= count;
		center.z /= count;
	}
}