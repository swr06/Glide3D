#pragma once

#include <iostream>

#include "GL_Classes/CubeTextureMap.h"
#include "GL_Classes/Shader.h"
#include "GL_Classes/VertexBuffer.h"
#include "GL_Classes/VertexArray.h"
#include "FpsCamera.h"

namespace Glide3D
{
	struct skybox_paths
	{
		std::string right;
		std::string left;
		std::string top;
		std::string bottom;
		std::string front;
		std::string back;
	};

	class Skybox
	{
	public:
		Skybox(const skybox_paths& paths);
		inline const GLClasses::CubeTextureMap& GetTexture() { return m_CubeMap; }
		void RenderSkybox(FPSCamera* camera);
		void RenderSkybox(const glm::mat4& projection, const glm::mat4& view);

	private :
		GLClasses::VertexBuffer m_VBO;
		GLClasses::VertexArray m_VAO;
		GLClasses::Shader m_SkyboxShader;
		GLClasses::CubeTextureMap m_CubeMap;
	};
}