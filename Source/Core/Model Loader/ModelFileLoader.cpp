#include "ModelFileLoader.h"

#include <assimp/pbrmaterial.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <chrono>

/* Model Loader
Uses the assimp model loading library to load the models. It uses a recursive model to process the meshes and materials
*/

namespace Glide3D
{
	namespace FileLoader
	{
		bool is_gltf = false;

		void LoadMaterialTextures(aiMesh* mesh, aiMaterial* mat, Mesh* _mesh, const std::string& path)
		{
			std::filesystem::path pth(path);

			std::string texture_path = pth.parent_path().string().c_str();
			aiString material_name;
			aiString diffuse_texture;
			aiString specular_texture;
			aiString normal_texture;
			aiString roughness_texture;
			aiString metallic_texture;
			aiString ao_texture;

			_mesh->p_IsGLTF = is_gltf;

			if (mat->GetTexture(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_TEXTURE, &diffuse_texture) == aiReturn_SUCCESS)
			{
				std::string pth = texture_path + "/" + diffuse_texture.C_Str();
				_mesh->p_AlbedoMap.CreateTexture(pth.c_str(), true);
			}

			else if(mat->GetTexture(aiTextureType_DIFFUSE, 0, &diffuse_texture) == aiReturn_SUCCESS)
			{
				std::string pth = texture_path + "/" + diffuse_texture.C_Str();
				_mesh->p_AlbedoMap.CreateTexture(pth.c_str(), true);
			}

			if (mat->GetTexture(aiTextureType_SPECULAR, 0, &specular_texture) == aiReturn_SUCCESS)
			{
				std::string pth = texture_path + "/" + specular_texture.C_Str();
				_mesh->p_LightMap.CreateTexture(pth.c_str(), false);
			}

			if (mat->GetTexture(aiTextureType_NORMALS, 0, &normal_texture) == aiReturn_SUCCESS)
			{
				std::string pth = texture_path + "/" + normal_texture.C_Str();
				_mesh->p_NormalMap.CreateTexture(pth.c_str(), false);
			}

			if (mat->GetTexture(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLICROUGHNESS_TEXTURE, &metallic_texture) == aiReturn_SUCCESS)
			{
				std::string pth = texture_path + "/" + metallic_texture.C_Str();
				_mesh->p_MetalnessRoughnessMap.CreateTexture(pth.c_str(), false);
			}

			else 
			{
				if (mat->GetTexture(aiTextureType_METALNESS, 0, &metallic_texture) == aiReturn_SUCCESS)
				{
					std::string pth = texture_path + "/" + metallic_texture.C_Str();
					_mesh->p_MetalnessMap.CreateTexture(pth.c_str(), false);
				}

				if (mat->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &roughness_texture) == aiReturn_SUCCESS)
				{
					std::string pth = texture_path + "/" + roughness_texture.C_Str();
					_mesh->p_RoughnessMap.CreateTexture(pth.c_str(), false);
				}
			
			}
			
			if (mat->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &ao_texture) == aiReturn_SUCCESS)
			{
				std::string pth = texture_path + "/" + ao_texture.C_Str();
				_mesh->p_AmbientOcclusionMap.CreateTexture(pth.c_str(), false);
			}
		}

		void ProcessAssimpMesh(aiMesh* mesh, const aiScene* scene, Object* object, const std::string& pth, const glm::vec4& col, const glm::vec3& reflectivity)
		{
			Mesh& _mesh = object->GenerateMesh();
			std::vector<Vertex>& vertices = _mesh.p_Vertices;
			std::vector<GLuint>& indices = _mesh.p_Indices;

			for (int i = 0; i < mesh->mNumVertices; i++)
			{
				Vertex vt;
				vt.position = glm::vec3(
					mesh->mVertices[i].x,
					mesh->mVertices[i].y,
					mesh->mVertices[i].z
				);

				if (mesh->HasNormals())
				{
					vt.normals = glm::vec3(
						mesh->mNormals[i].x,
						mesh->mNormals[i].y,
						mesh->mNormals[i].z
					);
				}

				if (mesh->mTextureCoords[0])
				{
					vt.tex_coords = glm::vec2(
						mesh->mTextureCoords[0][i].x,
						mesh->mTextureCoords[0][i].y
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

			_mesh.p_Name = mesh->mName.C_Str();

			// process materials
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			_mesh.p_Color = col;
			_mesh.p_Reflectivity = reflectivity;
	
			LoadMaterialTextures(mesh, material, &object->m_Meshes.back(), pth);
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
			glm::vec3 reflectivity;
		};

		std::vector<TransparentMesh> transparent_meshes;
		uint32_t mesh_count = 0;

		void ProcessAssimpNode(aiNode* Node, const aiScene* Scene, Object* object, const std::string& pth)
		{
			// Process all the meshes in the node
			// Add the transparent meshes to the transparent mesh queue and add all the opaque ones

			for (int i = 0; i < Node->mNumMeshes; i++)
			{
				mesh_count++;
				aiMesh* mesh = Scene->mMeshes[Node->mMeshes[i]];
				aiMaterial* material = Scene->mMaterials[mesh->mMaterialIndex];
				aiColor4D diffuse_color;
				aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse_color);

				float transparency = 0.0f;

				if (aiGetMaterialFloat(material, AI_MATKEY_OPACITY, &transparency) == AI_FAILURE)
				{
					transparency = 0.0f;
				}

				aiVector3D _reflectivity;

				if (material->Get(AI_MATKEY_COLOR_REFLECTIVE, _reflectivity) == AI_FAILURE)
				{
					_reflectivity = aiVector3D(0.0f);
				}

				glm::vec3 reflectivity = glm::vec3(_reflectivity.x, _reflectivity.y, _reflectivity.z);

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
					m.reflectivity = reflectivity;

					transparent_meshes.push_back(m);
					continue;
				}

				ProcessAssimpMesh(mesh, Scene, object, pth, 
					final_color, reflectivity);
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
				ProcessAssimpMesh(e.mesh, (const aiScene*)e.scene, e.object, e.pth, e.final_color, e.reflectivity);
			}

			transparent_meshes.clear();
		}

		void LoadOBJFile(Object* object, const std::string& filepath)
		{
			if (filepath.find("glb") != std::string::npos || filepath.find("gltf") != std::string::npos)
			{
				is_gltf = true;
			}

			Assimp::Importer importer;
			Timer timer;

			timer.Start(TimerDurationCast::Seconds);

			const aiScene* Scene = importer.ReadFile
			(
				filepath,
				aiProcess_JoinIdenticalVertices |
				aiProcess_Triangulate |
				aiProcess_CalcTangentSpace |
				aiProcess_GenUVCoords |
				aiProcess_FlipUVs 
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

			long long elapsed_time = timer.End();
			Logger::Log("Loaded Model File " + filepath + "\n\t" + "ELAPSED TIME : " + std::to_string(elapsed_time) + " s" + 
			"\n\tMesh Count : " + std::to_string(mesh_count));

			mesh_count = 0;
			is_gltf = false;

			return;
		}
	}
}
