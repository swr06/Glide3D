#pragma once

#include <iostream>
#include <vector>
#include "../Utils/Vertex.h"
#include "../Utils/Defs.h"
#include "../GL_Classes/Texture.h"
#include <glad/glad.h>

namespace Glide3D
{
	/*
	Has a texture, light map, albedo and normal map
	*/
	class Object
	{
	public :
		std::vector<Vertex> p_Vertices;
		std::vector<GLuint> p_Indices;
		GLClasses::Texture p_Texture;
		GLClasses::Texture p_LightMap;
		GLClasses::Texture p_AlbedoMap;
		GLClasses::Texture p_NormalMap;
	};
}