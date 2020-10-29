#pragma once

#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "GL_Classes/VertexBuffer.h"
#include "GL_Classes/IndexBuffer.h"
#include "GL_Classes/VertexArray.h"
#include "GL_Classes/Shader.h"
#include "GL_Classes/SSBO.h"
#include "GL_Classes/Framebuffer.h"

namespace Glide3D
{
	class SSAOHandler
	{
	public :

		SSAOHandler();

	private :

		std::vector<glm::vec3> m_SSAOKernel;
		std::vector<glm::vec3> m_SSAONoise;

		GLuint m_SSAOTexture;
		GLuint m_SSAOFBO;
		GLuint m_SSAOColorbuffer;
	};
}