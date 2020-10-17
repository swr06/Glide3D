#pragma once

#include <glad/glad.h>

#include <iostream>
#include <vector>
#include <filesystem>
#include <sstream>

#include "../Object/Object.h"
#include "../Application/Logger.h"
#include "../Utils/Timer.h"

#include <cstdio>
#include <cstdlib> 

namespace Glide3D
{
	namespace FileLoader
	{
		void LoadOBJFile(Object* object, const std::string& filepath);
	}
}