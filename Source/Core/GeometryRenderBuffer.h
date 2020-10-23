#pragma once

#include <glad/glad.h>

#include <iostream>
#include <string>
#include <vector>

namespace Glide3D
{
	class GeometryRenderBuffer
	{
	private :

		GLuint m_AlbedoTexture;
		GLuint m_DepthTexture;
	};
}