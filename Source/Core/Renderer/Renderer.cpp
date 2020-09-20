#include "Renderer.h"

#define MAX_DIRECTIONAL_LIGHTS 4
#define MAX_POINT_LIGHTS 100

namespace Glide3D
{
	Renderer::Renderer(GLFWwindow* window) : 
		m_FBOVBO(GL_ARRAY_BUFFER), m_Window(window)
	{
		bool IndexBufferInitialized = false;

		/* Framebuffer stuff */

		// basic quad vertices
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

		/* Create and compile the shaders */
		m_RendererShader.CreateShaderProgramFromFile("Core/Shaders/RendererVert.glsl", "Core/Shaders/RendererFrag.glsl");
		m_RendererShader.CompileShaders();
		m_FBOShader.CreateShaderProgramFromFile("Core/Shaders/FramebufferVert.glsl", "Core/Shaders/FramebufferFrag.glsl");
		m_FBOShader.CompileShaders();
	}

	void Renderer::AddDirectionalLight(const DirectionalLight& light)
	{
		if (m_DirectionalLights.size() + 1 > MAX_DIRECTIONAL_LIGHTS)
		{
			std::stringstream s;
			s << "Max DIRECTIONAL lights allowed is : " << MAX_DIRECTIONAL_LIGHTS << " || Light could not be added!";
			Logger::Log(s.str());
			assert(0);
		}

		m_DirectionalLights.push_back(light);
	}

	void Renderer::AddPointLight(const PointLight& light)
	{
		if (m_PointLights.size() + 1 > MAX_POINT_LIGHTS)
		{
			std::stringstream s;
			s << "Max POINT lights allowed is : " << MAX_POINT_LIGHTS << " || Light could not be added!";
			Logger::Log(s.str());
			assert(0);
		}

		m_PointLights.push_back(light);
	}

	void Renderer::SetLightUniforms(GLClasses::Shader& shader)
	{
		shader.SetInteger("u_SceneDirectionalLightCount", m_DirectionalLights.size(), 0);
		shader.SetInteger("u_ScenePointLightCount", m_PointLights.size(), 0);

		for (int i = 0; i < m_DirectionalLights.size(); i++)
		{
			std::string name("u_SceneDirectionalLights[");
			name = name + std::to_string(i) + "]";

			shader.SetVector3f(name + ".m_Position", m_DirectionalLights[i].m_Position);
			shader.SetVector3f(name + ".m_SpecularColor", m_DirectionalLights[i].m_SpecularColor);
			shader.SetInteger(name + ".m_SpecularExponent", m_DirectionalLights[i].m_SpecularExponent);
			shader.SetFloat(name + ".m_SpecularStrength", m_DirectionalLights[i].m_SpecularStrength);
		}

		for (int i = 0; i < m_PointLights.size(); i++)
		{
			std::string name("u_ScenePointLights[");
			name = name + std::to_string(i) + "]";

			shader.SetVector3f(name + ".m_Position", m_PointLights[i].m_Position);
			shader.SetVector3f(name + ".m_SpecularColor", m_PointLights[i].m_SpecularColor);
			shader.SetInteger(name + ".m_SpecularExponent", m_PointLights[i].m_SpecularExponent);
			shader.SetFloat(name + ".m_SpecularStrength", m_PointLights[i].m_SpecularStrength);
			shader.SetFloat(name + ".m_Linear", m_PointLights[i].m_Linear);
			shader.SetFloat(name + ".m_Constant", m_PointLights[i].m_Constant);
			shader.SetFloat(name + ".m_Quadratic", m_PointLights[i].m_Quadratic);
		}
	}

	void Renderer::StartRender(const FPSCamera* camera)
	{
		m_RendererShader.Use();
		m_RendererShader.SetFloat("u_AmbientStrength", 0.75f);
		m_RendererShader.SetInteger("u_AlbedoMap", 0);
		m_RendererShader.SetVector3f("u_ViewerPosition", camera->GetPosition());  // -3 1 -12 (Insert another light)
		m_RendererShader.SetMatrix4("u_ViewProjection", camera->GetViewProjection());
		SetLightUniforms(m_RendererShader);
	}

	/*
	Renders a group of the same entity at different positions using instanced rendering
	*/
	void Renderer::RenderObjects(const std::vector<Entity>& entities)
	{
		unsigned int entity_num = entities.size();

		Object* object = entities[0].p_Object;

		if (object)
		{
			if (object->p_CanFacecull)
			{
				glEnable(GL_CULL_FACE);
			}

			else
			{
				glDisable(GL_CULL_FACE);
			}

			const std::vector<Vertex>& Vertices = entities[0].p_Object->p_Vertices;
			const std::vector<GLuint>& Indices = entities[0].p_Object->p_Indices;
			std::vector<glm::mat4> Matrices;

			for (auto& e : entities)
			{
				const glm::mat4& model = e.p_Transform.GetTransformationMatrix();
				Matrices.push_back(model);
				Matrices.push_back(glm::mat4(e.p_Transform.GetNormalMatrix()));
				entity_num++;
			}

			const bool& indexed = object->p_Indexed;
			bool can_render = true; // Flag to assure that the size of the vertices is over zero

			if (object->p_AlbedoMap)
			{
				object->p_AlbedoMap->Bind(0);
			}

			/* These uniforms vary from Object to object */
			m_RendererShader.SetVector3f("u_Color", object->p_DefaultColor);
			m_RendererShader.SetInteger("u_HasAlbedoMap", static_cast<int>(object->p_AlbedoMap->GetTextureID() != 0));

			GLClasses::VertexArray& VAO = object->p_VertexArray;
			GLClasses::VertexBuffer& MatrixVBO = object->p_MatrixBuffer;
			
			MatrixVBO.BufferData(Matrices.size() * sizeof(glm::mat4), &Matrices.front(), GL_STATIC_DRAW);
			VAO.Bind();
			
			if (indexed)
			{
				glDrawElementsInstanced(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0, entities.size());
			}

			else
			{
				glDrawArraysInstanced(GL_TRIANGLES, 0, Vertices.size(), entities.size());
			}

			VAO.Unbind();
		}

		else
		{
			Logger::Log("Attempted to draw an entity without a parent object! || COULD NOT DRAW ENTITY LIST OF SIZE : " + entities.size());
		}
		return;
	}

	void Renderer::EndRender()
	{
		glUseProgram(0);
	}

	/*
	Renders a framebuffer onto the main window
	*/
	void Renderer::RenderFBO(const GLClasses::Framebuffer& fbo)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		glDisable(GL_DEPTH_TEST);
		m_FBOShader.Use();
		m_FBOShader.SetInteger("u_FramebufferTexture", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, fbo.GetTexture());
		m_FBOVAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		m_FBOVAO.Unbind();
		glEnable(GL_DEPTH_TEST);


		glUseProgram(0);
	}
}
