#include "OBJFileLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Glide3D
{
	namespace FileLoader
	{
		void ProcessAssimpMesh(aiMesh* mesh, const aiScene* scene, Object* object)
		{
			std::vector<Vertex>& vertices = object->p_Vertices;
			std::vector<GLuint>& indices = object->p_Indices;
			
			for (int i = 0; i < mesh->mNumVertices; i++)
			{
				Vertex vt;
				vt.position = glm::vec3(
					(float)mesh->mVertices[i].x,
					(float)mesh->mVertices[i].y,
					(float)mesh->mVertices[i].z
				);

				if (mesh->HasNormals())
				{
					vt.normals = glm::vec3(
					(float)mesh->mNormals[i].x,
					(float)mesh->mNormals[i].y,
					(float)mesh->mNormals[i].z
					);
				}

				if (mesh->mTextureCoords[0])
				{
					vt.tex_coords = glm::vec2(
						(float)mesh->mTextureCoords[0][i].x,
						(float)mesh->mTextureCoords[0][i].y
					);
				} 
				
				else
				{
					vt.tex_coords = glm::vec2(0.0f, 0.0f);
				}

				vertices.push_back(vt);
			}

			for (int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];

				for (unsigned int j = 0; j < face.mNumIndices; j++)
				{
					indices.push_back(face.mIndices[j]);
				}
			}
		}

		void ProcessAssimpNode(aiNode* Node, const aiScene* Scene, Object* object)
		{
			for (int i = 0; i < Node->mNumMeshes; i++)
			{
				aiMesh* mesh = Scene->mMeshes[Node->mMeshes[i]];
				ProcessAssimpMesh(mesh, Scene, object);
			}

			for (int i = 0; i < Node->mNumChildren; i++)
			{
				ProcessAssimpNode(Node->mChildren[i], Scene, object);
			}
		}

		void LoadOBJFile(Object* object, const std::string& filepath)
		{
			Assimp::Importer importer;
			const aiScene* Scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

			if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
			{
				std::stringstream str;
				str << "ERROR LOADING ASSIMP MODEL (" << filepath << ") || ASSIMP ERROR : " << importer.GetErrorString();
				Logger::Log(str.str());
				return;
			}

			ProcessAssimpNode(Scene->mRootNode, Scene, object);
		}
	}
}
