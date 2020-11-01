#include "OBJFileLoader.h"

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
					_mesh->p_AlbedoMap.CreateTexture(texture_path, true) ;

					if (texture_path.find("curtain") != std::string::npos)
					{
						_mesh->p_HasWavePhysics = true;
						_mesh->p_WaveAffectY = 30;
						_mesh->p_WaveAffectFreq = glm::vec2(0.5f, 0.25f);
						_mesh->p_WaveAffectSpeed = glm::vec2(2.5f, 5.0f);
					}

					if (texture_path.find("fabric") != std::string::npos)
					{
						_mesh->p_HasWavePhysics = true;
						_mesh->p_WaveAffectY = 58;
						_mesh->p_WaveAffectFreq = glm::vec2(0.5f, 0.25f);
						_mesh->p_WaveAffectSpeed = glm::vec2(2.5f, 4.0f);
					}

					break;
				}

				case aiTextureType_SPECULAR:
				{
					_mesh->p_LightMap.CreateTexture(texture_path, false);
					break;
				}

				case aiTextureType_HEIGHT:
				{
					_mesh->p_NormalMap.CreateTexture(texture_path, false);
					break;
				}

				case aiTextureType_METALNESS:
				{
					_mesh->p_MetalnessMap.CreateTexture(texture_path, false);
					break;
				}

				case aiTextureType_DIFFUSE_ROUGHNESS:
				{
					_mesh->p_RoughnessMap.CreateTexture(texture_path, false);
					break;
				}

				case aiTextureType_AMBIENT_OCCLUSION:
				{
					_mesh->p_AmbientOcclusionMap.CreateTexture(texture_path, false);
					break;
				}

				}
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

			_mesh.p_Name = mesh->mName.C_Str();

			// process materials
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			_mesh.p_Color = col;
			_mesh.p_Reflectivity = reflectivity;
	
			LoadMaterialTexture(mesh, material, aiTextureType_DIFFUSE, &object->m_Meshes.back(), pth);
			LoadMaterialTexture(mesh, material, aiTextureType_SPECULAR, &object->m_Meshes.back(), pth);
			LoadMaterialTexture(mesh, material, aiTextureType_HEIGHT, &object->m_Meshes.back(), pth);
			LoadMaterialTexture(mesh, material, aiTextureType_METALNESS, &object->m_Meshes.back(), pth);
			LoadMaterialTexture(mesh, material, aiTextureType_DIFFUSE_ROUGHNESS, &object->m_Meshes.back(), pth);
			LoadMaterialTexture(mesh, material, aiTextureType_AMBIENT_OCCLUSION, &object->m_Meshes.back(), pth);
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
			// Set the object's name
			std::string filename = std::filesystem::path(filepath).filename().string();
			object->m_ObjectName = filename;

			Assimp::Importer importer;
			Timer timer;

			timer.Start(TimerDurationCast::Seconds);

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

			long long elapsed_time = timer.End();
			Logger::Log("Loaded Model File " + filepath + "\n\t" + "ELAPSED TIME : " + std::to_string(elapsed_time) + " s" + 
			"\n\tMesh Count : " + std::to_string(mesh_count));

			mesh_count = 0;

			return;
		}
	}
}
