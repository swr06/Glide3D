#include "Logger.h"

namespace Glide3D
{
	void Logger::Log(const std::string& txt)
	{
		std::cout << "\n" << "GLIDE3D :  " << txt; 
	}
}
