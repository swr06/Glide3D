#include "Tonemapper.h"

namespace Glide3D
{
	Tonemapper::Tonemapper() : m_FBOVBO(GL_ARRAY_BUFFER)
	{
		std::string vert_path = "Core/Shaders/Tonemapping/TonemapperVert.glsl";

		m_ACES.CreateShaderProgramFromFile(vert_path, "Core/Shaders/Tonemapping/aces.glsl");
		m_Filmic.CreateShaderProgramFromFile(vert_path, "Core/Shaders/Tonemapping/filmic.glsl");
		m_Lottes.CreateShaderProgramFromFile(vert_path, "Core/Shaders/Tonemapping/lottes.glsl");
		m_Reinhard.CreateShaderProgramFromFile(vert_path, "Core/Shaders/Tonemapping/reinhard.glsl");
		m_Reinhard2.CreateShaderProgramFromFile(vert_path, "Core/Shaders/Tonemapping/reinhard2.glsl");
		m_Uchimura.CreateShaderProgramFromFile(vert_path, "Core/Shaders/Tonemapping/uchimura.glsl");
		m_Uncharted2.CreateShaderProgramFromFile(vert_path, "Core/Shaders/Tonemapping/uncharted2.glsl");
		m_Unreal.CreateShaderProgramFromFile(vert_path, "Core/Shaders/Tonemapping/unreal.glsl");

		m_ACES.CompileShaders();
		m_Filmic.CompileShaders();
		m_Lottes.CompileShaders();
		m_Reinhard.CompileShaders();
		m_Reinhard2.CompileShaders();
		m_Uchimura.CompileShaders();
		m_Uncharted2.CompileShaders();
		m_Unreal.CompileShaders();

		float Vertices[] =
		{
			-1.0f,  1.0f,  0.0f, 1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f, -1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,  1.0f,  1.0f,  1.0f, 1.0f
		};

		m_FBOVBO.BufferData(sizeof(Vertices), Vertices, GL_STATIC_DRAW);
		m_FBOVAO.Bind();
		m_FBOVBO.Bind();
		m_FBOVBO.VertexAttribPointer(0, 2, GL_FLOAT, 0, 4 * sizeof(GLfloat), 0);
		m_FBOVBO.VertexAttribPointer(1, 2, GL_FLOAT, 0, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
		m_FBOVAO.Unbind();
	}

	void Tonemapper::Render(const GLClasses::Framebuffer& fbo)
	{
		GLClasses::Shader* shader = nullptr;

		switch (fbo.p_TonemappingType)
		{
			case TonemappingType::ACES : 
			{
				shader = &m_ACES;
				break;
			}

			case TonemappingType::Filmic:
			{
				shader = &m_Filmic;
				break;
			}

			case TonemappingType::Lottes:
			{
				shader = &m_Lottes;
				break;
			}

			case TonemappingType::Reinhard:
			{
				shader = &m_Reinhard;
				break;
			}

			case TonemappingType::Reinhard2:
			{
				shader = &m_Reinhard2;
				break;
			}

			case TonemappingType::Uchimura:
			{
				shader = &m_Uchimura;
				break;
			}

			case TonemappingType::Uncharted2:
			{
				shader = &m_Uncharted2;
				break;
			}

			case TonemappingType::Unreal:
			{
				shader = &m_Unreal;
				break;
			}

			default : 
			{
				shader = &m_ACES;
			}
		}

		glDisable(GL_DEPTH_TEST);
		shader->Use();
		shader->SetInteger("u_FramebufferTexture", 1);
		shader->SetFloat("u_Exposure", fbo.GetExposure());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, fbo.GetTexture());

		m_FBOVAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		m_FBOVAO.Unbind();

		glEnable(GL_DEPTH_TEST);

		glUseProgram(0);
	}
}
