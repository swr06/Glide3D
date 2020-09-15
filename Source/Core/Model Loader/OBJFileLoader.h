#pragma once

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <sstream>
#include "../Object/Object.h"
#include "../Application/Logger.h"

#include <cstdio>
#include <cstdlib>

namespace Glide3D
{
	namespace FileLoader
	{
		void LoadOBJFile(Object* object, const std::string& filepath);
	}
}