#include "OBJFileLoader.h"

namespace Glide3D
{
	namespace FileLoader
	{
		void LoadOBJFile(Object* object, const std::string& filepath)
		{
			std::vector<Vertex>& ObjectVertices = object->p_Vertices;
			std::vector<GLuint>& ObjectIndices = object->p_Indices;

			// buffers to store the data
			std::vector<glm::vec3> vertices;
			std::vector<glm::vec2> texture_coordinates;
			std::vector<glm::vec3> vertex_normals;
			std::vector<glm::ivec3> faces;

			FILE* obj_file = fopen(filepath.c_str(), "r");
			char* option = new char[256];
			memset(option, '\0', 256);

			if (obj_file)
			{
				while (true)
				{
					int val = fscanf(obj_file, "%s", option);

					if (val != EOF)
					{
						if (!strcmp(option, "v")) // v for vertex
						{
							glm::vec3 v;
							fscanf(obj_file, "%f %f %f\n", &v.x, &v.y, &v.z);
							vertices.push_back(v);
						}

						else if (!strcmp(option, "vt")) // vt for vertex texture coordinates
						{
							glm::vec2 vt;
							fscanf(obj_file, "%f %f\n", &vt.x, &vt.y);
							texture_coordinates.push_back(vt);
						}

						else if (!strcmp(option, "vn")) // v for vertex
						{
							glm::vec3 vn;
							fscanf(obj_file, "%f %f %f\n", &vn.x, &vn.y, &vn.z);
							vertex_normals.push_back(vn);
						}

						else if (!strcmp(option, "f"))
						{
							int vertex_index[3];
							int tex_index[3];
							int normal_index[3];

							fscanf(obj_file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertex_index[0], &tex_index[0], &normal_index[0],
								&vertex_index[1], &tex_index[1], &normal_index[1],
								&vertex_index[2], &tex_index[2], &normal_index[2]);

							faces.push_back(glm::ivec3(vertex_index[0], tex_index[0], normal_index[0]));
							faces.push_back(glm::ivec3(vertex_index[1], tex_index[1], normal_index[1]));
							faces.push_back(glm::ivec3(vertex_index[2], tex_index[2], normal_index[2]));
						}

						else
						{
							fscanf(obj_file, "\n");
						}
					}

					else
					{
						break;
					}
				}

				/*
				Create the triangle mesh
				*/
				for (int i = 0 ; i < faces.size() ; i++)
				{
					faces[i] -= 1;

					Vertex v;

					v.position = vertices[faces[i].x];
					v.tex_coords = texture_coordinates[faces[i].y];
					v.normals = vertex_normals[faces[i].z];
					ObjectVertices.push_back(v);				
				}
			}

			else
			{
				std::cout << "FATAL ERROR ! COULD NOT LOAD : " << filepath;
			}
		}
	}
}
