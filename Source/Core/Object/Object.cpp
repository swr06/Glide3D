#include "Object.h"
#include <cstdlib>
#include <cstdio>

namespace Glide3D
{
	static uint32_t _CurrentObjectID = 1;

	Object::Object(const ReflectionMapProperties& props) : p_ReflectionProps(props), m_ObjectID(++_CurrentObjectID)
	{
	
	}

	Object::~Object()
	{

	}

	void Object::Buffer()
	{
		CalculateCenter();

		for (auto& e : m_Meshes)
		{
			e.Buffer();
		}
	}

	void Object::AddTextureMapToMesh(const std::string& path, TextureType tex_type, bool flip)
	{
		if (m_Meshes.size() > 0)
		{
			Mesh* mesh = &m_Meshes.back();

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

			case TextureType::Metalness:
			{
				mesh->p_MetalnessMap.CreateTexture(path, flip);
				break;
			}

			case TextureType::Roughness:
			{
				mesh->p_RoughnessMap.CreateTexture(path, flip);
				break;
			}

			case TextureType::AO:
			{
				mesh->p_AmbientOcclusionMap.CreateTexture(path, flip);
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
		m_Meshes.emplace_back(m_Meshes.size());
		return m_Meshes.back();
	}

	void Object::CalculateTangentNormals()
	{
		for (auto& e : m_Meshes)
		{
			e.CalculateTangentNormals();
		}
	}

	void Object::CalculateCenter()
	{
		uint32_t count = 0;
		glm::vec3& center = this->m_Center;

		for (auto& e : m_Meshes)
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