#include "UBO.h"

namespace Glide3D
{
	UBO::UBO(GLsizeiptr size)
	{
		glGenBuffers(1, &m_UBO);
		glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
		glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}