#include "Object.h"
#include <cstdlib>
#include <cstdio>

namespace Glide3D
{
	Object::Object(const ReflectionMapProperties& props) : p_ReflectionProps(props)
	{

	}

	Object::~Object()
	{

	}

	void Object::Buffer()
	{
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

	void Object::CalculateTangentNormals()
	{
		for (auto& e : p_Meshes)
		{
			e.CalculateTangentNormals();
		}
	}
}