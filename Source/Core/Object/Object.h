#pragma once

#include <iostream>
#include <vector>
#include "../Utils/Vertex.h"
#include "../Utils/Defs.h"
#include <glad/glad.h>

namespace Glide3D
{
	class Object
	{
	public :
		std::vector<Vertex> p_Vertices;
		std::vector<GLuint> p_Indices;
	};
}