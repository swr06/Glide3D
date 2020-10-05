#include "OBJFileLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/* Model Loader
Uses the assimp model loading library to load the models. It uses a recursive model to process the meshes and materials
*/

namespace Glide3D
{
	namespace FileLoader
	{
		void LoadMaterialTexture(aiMesh* mesh, aiMaterial* mat, aiTextureType type, Mesh* _mesh, const std::string& path)
		{
			std::filesystem::path pth(path);

			aiString material_name;
			std::string texture_path = pth.parent_path().string().c_str();

			mat->GetTexture(type, 0, &material_name);

			texture_path = texture_path + "/" + material_name.C_Str();

			if (material_name.length > 0 && material_name.C_Str())
			{
				switch (type)
				{
				case aiTextureType_DIFFUSE:
				{
					_mesh->p_AlbedoMap.CreateTexture(texture_path);
					break;
				}

				case aiTextureType_SPECULAR:
				{
					_mesh->p_LightMap.CreateTexture(texture_path);
					break;
				}

				case aiTextureType_HEIGHT:
				{
					_mesh->p_NormalMap.CreateTexture(texture_path);
					break;
				}
				}
			}
		}

		void ProcessAssimpMesh(aiMesh* mesh, const aiScene* scene, Object* object, const std::string& pth)
		{
			object->p_Meshes.emplace_back();
			Mesh* _mesh = &(object->p_Meshes.back());

			std::vector<Vertex>& vertices = _mesh->p_Vertices;
			std::vector<GLuint>& indices = _mesh->p_Indices;

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

					vt.tangent.x = mesh->mTangents[i].x;
					vt.tangent.y = mesh->mTangents[i].y;
					vt.tangent.z = mesh->mTangents[i].z;

					vt.bitangent.x = mesh->mBitangents[i].x;
					vt.bitangent.y = mesh->mBitangents[i].y;
					vt.bitangent.z = mesh->mBitangents[i].z;
				}

				else
				{
					vt.tex_coords = glm::vec2(0.0f, 0.0f);
				}

				vertices.push_back(vt);
			}

			/* Push back the indices */
			for (int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];

				for (unsigned int j = 0; j < face.mNumIndices; j++)
				{
					indices.push_back(face.mIndices[j]);
				}
			}

			/* Load material maps
			- Albedo map
			- Specular map
			- Normal map
			*/

			// process materials
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			LoadMaterialTexture(mesh, material, aiTextureType_DIFFUSE, _mesh, pth);
			LoadMaterialTexture(mesh, material, aiTextureType_SPECULAR, _mesh, pth);
			LoadMaterialTexture(mesh, material, aiTextureType_HEIGHT, _mesh, pth);
		}

		void ProcessAssimpNode(aiNode* Node, const aiScene* Scene, Object* object, const std::string& pth)
		{
			for (int i = 0; i < Node->mNumMeshes; i++)
			{
				aiMesh* mesh = Scene->mMeshes[Node->mMeshes[i]];
				ProcessAssimpMesh(mesh, Scene, object, pth);
			}

			for (int i = 0; i < Node->mNumChildren; i++)
			{
				ProcessAssimpNode(Node->mChildren[i], Scene, object, pth);
			}
		}

		void LoadOBJFile(Object* object, const std::string& filepath)
		{
			Assimp::Importer importer;
			//const aiScene* Scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
			const aiScene* Scene = importer.ReadFile
			(
				filepath,
				aiProcess_JoinIdenticalVertices |
				aiProcess_Triangulate |
				aiProcess_GenSmoothNormals |
				aiProcess_CalcTangentSpace |
				aiProcess_ImproveCacheLocality |
				aiProcess_RemoveRedundantMaterials |
				aiProcess_GenUVCoords |
				aiProcess_FlipUVs |
				aiProcess_SortByPType |
				aiProcess_FindDegenerates |
				aiProcess_FindInvalidData |
				aiProcess_OptimizeMeshes
			);

			if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
			{
				std::stringstream str;
				str << "ERROR LOADING ASSIMP MODEL (" << filepath << ") || ASSIMP ERROR : " << importer.GetErrorString();
				Logger::Log(str.str());
				return; 
			}

			ProcessAssimpNode(Scene->mRootNode, Scene, object, filepath);
			object->Buffer();
			return;
		}
	}
}
