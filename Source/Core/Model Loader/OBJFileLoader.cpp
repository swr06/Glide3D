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

		{
			Mesh _mesh;
			std::vector<Vertex>& vertices = _mesh.p_Vertices;
			std::vector<GLuint>& indices = _mesh.p_Indices;

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

					if (mesh->mTangents)
					{
						vt.tangent.x = mesh->mTangents[i].x;
						vt.tangent.y = mesh->mTangents[i].y;
						vt.tangent.z = mesh->mTangents[i].z;
					}

					if (mesh->mBitangents)
					{
						vt.bitangent.x = mesh->mBitangents[i].x;
						vt.bitangent.y = mesh->mBitangents[i].y;
						vt.bitangent.z = mesh->mBitangents[i].z;
					}
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
			_mesh.p_Color = col;

			object->p_Meshes.emplace_back(std::move(_mesh));
	
			LoadMaterialTexture(mesh, material, aiTextureType_DIFFUSE, &object->p_Meshes.back(), pth);
			LoadMaterialTexture(mesh, material, aiTextureType_SPECULAR, &object->p_Meshes.back(), pth);
			LoadMaterialTexture(mesh, material, aiTextureType_HEIGHT, &object->p_Meshes.back(), pth);
		}

		struct TransparentMesh
		{
			aiMesh* mesh;
			aiMaterial* material;
			glm::vec4 final_color;
			aiNode* node;
			aiScene* scene;
			Object* object;
			std::string pth;
		};

		std::vector<TransparentMesh> transparent_meshes;

		void ProcessAssimpNode(aiNode* Node, const aiScene* Scene, Object* object, const std::string& pth)
		{
			// Process all the meshes in the node
			// Add the transparent meshes to the transparent mesh queue and add all the opaque ones

			for (int i = 0; i < Node->mNumMeshes; i++)
			{
				aiMesh* mesh = Scene->mMeshes[Node->mMeshes[i]];
				aiMaterial* material = Scene->mMaterials[mesh->mMaterialIndex];
				aiColor4D diffuse_color;
				aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse_color);

				float transparency;
				aiGetMaterialFloat(material, AI_MATKEY_OPACITY, &transparency);

				glm::vec4 final_color;

				final_color = glm::vec4(diffuse_color.r, diffuse_color.g, diffuse_color.b, diffuse_color.a) *
					transparency;

				if (transparency < 0.95f)
				{
					TransparentMesh m;
					m.mesh = mesh;
					m.material = material;
					m.final_color = final_color;
					m.node = Node;
					m.scene = (aiScene*)Scene;
					m.object = object;
					m.pth = pth;

					transparent_meshes.push_back(m);
					continue;
				}

				ProcessAssimpMesh(mesh, Scene, object, pth, 
					final_color);
			}

			for (int i = 0; i < Node->mNumChildren; i++)
			{
				ProcessAssimpNode(Node->mChildren[i], Scene, object, pth);
			}
		}

		void ProcessTransparentMeshes()
		{
			for (auto& e : transparent_meshes)
			{
				ProcessAssimpMesh(e.mesh, (const aiScene*)e.scene, e.object, e.pth, e.final_color);
			}

			transparent_meshes.clear();
		}

		void LoadOBJFile(Object* object, const std::string& filepath)
		{
			Assimp::Importer importer;
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
				str << "ERROR LOADING ASSIMP MODEL (" << filepath << ") ||  ASSIMP ERROR : " << importer.GetErrorString();
				Logger::Log(str.str());
				return; 
			}

			ProcessAssimpNode(Scene->mRootNode, Scene, object, filepath);
			ProcessTransparentMeshes();
			object->Buffer();

			return;
		}
	}
}
