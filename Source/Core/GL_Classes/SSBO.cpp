#include "SSBO.h"

namespace GLClasses
{
	SSBO::SSBO()
	{
		glGenBuffers(1, &m_SSBO);
	}

	SSBO::~SSBO()
	{
		
	}
}