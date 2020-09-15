#pragma once

#include <iostream>
#include <vector>
#include "../Utils/Vertex.h"
#include "../Utils/Defs.h"
#include "../GL_Classes/Texture.h"
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Glide3D
{
	/*
	Has a texture, light map, albedo and normal map
	*/
	class Object
	{
	public :
		std::vector<glm::vec3> p_Vertices;
		std::vector<glm::vec3> p_Normals;
		std::vector<glm::vec2> p_TextureCoords;
		std::vector<GLuint> p_Indices;
		/*GLClasses::Texture* p_Texture;
		GLClasses::Texture* p_LightMap;
		GLClasses::Texture* p_AlbedoMap;
		GLClasses::Texture* p_NormalMap;*/
	};
}