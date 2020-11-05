#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "../GL_Classes/Framebuffer.h"
#include "../GL_Classes/VertexBuffer.h"
#include "../GL_Classes/IndexBuffer.h"
#include "../GL_Classes/VertexArray.h"
#include "../GL_Classes/Shader.h"
#include "../GL_Classes/SSBO.h"
#include "../GL_Classes/Shader.h"

namespace Glide3D
{
	class Renderer;

	class Tonemapper
	{
	public :

		Tonemapper();

		// Renders source `fbo` framebuffer to the currently bound framebuffer
		void Render(const GLClasses::Framebuffer& fbo);

	private :
		GLClasses::Shader m_ACES;
		GLClasses::Shader m_Filmic;
		GLClasses::Shader m_Lottes;
		GLClasses::Shader m_Reinhard;
		GLClasses::Shader m_Reinhard2;
		GLClasses::Shader m_Uchimura;
		GLClasses::Shader m_Uncharted2;
		GLClasses::Shader m_Unreal;
		GLClasses::VertexArray m_FBOVAO;
		GLClasses::VertexBuffer m_FBOVBO;


		friend class Renderer;
	};
}