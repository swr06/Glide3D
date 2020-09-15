#include "OBJFileLoader.h"
#include "../../Dependencies/obj_loader/OBJ_Loader.h"

namespace Glide3D
{
	namespace FileLoader
	{
		void LoadOBJFile(Object* object, const std::string& filepath)
		{
			std::vector<Vertex>& vertices = object->p_Vertices;
			std::vector<GLuint>& indices = object->p_Indices;
			std::string& mesh_name = object->p_MeshName;

			objl::Loader Loader;
			bool res = Loader.LoadFile(filepath);

			if (res)
			{
				objl::Mesh mesh = Loader.LoadedMeshes[0];
				for (auto &e : mesh.Vertices)
				{
					vertices.push_back({ 
						glm::vec3(e.Position.X, e.Position.Y, e.Position.Z), 
						glm::vec3(e.Normal.X, e.Normal.Y, e.Normal.Z),
						glm::vec2(e.TextureCoordinate.X, e.TextureCoordinate.Y)});
				}

				for (auto& e : mesh.Indices)
				{
					indices.push_back(e);
				}

				mesh_name = mesh.MeshName;
			}
		}
	}
}
