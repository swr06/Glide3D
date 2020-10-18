#include "Renderer.h"

#define MAX_DIRECTIONAL_LIGHTS 2
#define MAX_POINT_LIGHTS 100

namespace Glide3D
{
	Renderer::Renderer(GLFWwindow* window) : 
		m_FBOVBO(GL_ARRAY_BUFFER), m_Window(window)
	{
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
		m_DepthShader.CreateShaderProgramFromFile("Core/Shaders/DepthVert.glsl", "Core/Shaders/DepthFrag.glsl");
		m_DepthShader.CompileShaders();
		m_ReflectionShader.CreateShaderProgramFromFile("Core/Shaders/ReflectionVert.glsl", "Core/Shaders/ReflectionFrag.glsl");
		m_ReflectionShader.CompileShaders();
	}

	void Renderer::AddDirectionalLight(DirectionalLight& light)
	{
		if (m_DirectionalLights.size() + 1 > MAX_DIRECTIONAL_LIGHTS)
		{
			std::stringstream s;
			s << "Max DIRECTIONAL lights allowed is : " << MAX_DIRECTIONAL_LIGHTS << " || Light could not be added!";
			Logger::Log(s.str());
			assert(0);
		}

		m_DirectionalLights.emplace_back(std::move(light));
	}

	void Renderer::AddPointLight(const PointLight& light)
	{
		if (m_PointLights.size() + 1 > MAX_POINT_LIGHTS)
		{
			std::stringstream s;
			s << "Max POINT lights allowed is : " << MAX_POINT_LIGHTS << " || Light could not be added!";
			Logger::Log(s.str());
			throw s.str().c_str();
		}

		m_PointLights.push_back(light);
	}

	void Renderer::SetLightUniforms(GLClasses::Shader& shader)
	{
		shader.SetInteger("u_SceneDirectionalLightCount", m_DirectionalLights.size(), 0);
		shader.SetInteger("u_NumDirectionalLights", m_DirectionalLights.size(), 0);
		shader.SetInteger("u_ScenePointLightCount", m_PointLights.size(), 0);

		for (int i = 0; i < m_DirectionalLights.size(); i++)
		{
			std::string name("u_SceneDirectionalLights[");
			name = name + std::to_string(i) + "]";

			std::string matname = "u_DirectionalLightSpaceVP[" + std::to_string(i) + "]";
			shader.SetMatrix4(matname,
				m_DirectionalLights[i].m_LightSpaceViewProjection, 0);

			shader.SetVector3f(name + ".m_Direction", m_DirectionalLights[i].m_Direction);
			shader.SetVector3f(name + ".m_SpecularColor", m_DirectionalLights[i].m_SpecularColor);
			shader.SetInteger(name + ".m_SpecularExponent", m_DirectionalLights[i].m_SpecularExponent);
			shader.SetFloat(name + ".m_SpecularStrength", m_DirectionalLights[i].m_SpecularStrength);
			shader.SetInteger(name + ".m_IsBlinn", (int)m_DirectionalLights[i].m_IsBlinn);
			shader.SetInteger(name + ".m_DepthMap", (int)5 + i); // 5 slots are used for the materials
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
			shader.SetInteger(name + ".m_IsBlinn", (int)m_PointLights[i].m_IsBlinn);
		}
	}

	void Renderer::BindLightingMaps()
	{
		for (int i = 0 ; i < m_DirectionalLights.size() ; i++)
		{
			glActiveTexture(GL_TEXTURE5 + i);
			glBindTexture(GL_TEXTURE_2D, m_DirectionalLights[i].m_DepthBuffer.GetDepthTexture());
		}
	}

	void Renderer::RenderPointLightShadowMap(PointLight& pointlight)
	{
		// Todo

	}

	void Renderer::_RenderEntitesForReflectionMap()
	{
		for (auto& entities : m_RenderEntities)
		{
			Object* object = entities[0]->p_Object;

			for (auto& e : object->p_Meshes)
			{
				Mesh* mesh = &e;

				const std::vector<Vertex>& Vertices = mesh->p_Vertices;
				const std::vector<GLuint>& Indices = mesh->p_Indices;
				bool indexed = mesh->p_Indexed;

				std::vector<glm::mat4> Matrices;

				for (auto& e : entities)
				{
					const glm::mat4& model = e->p_Transform.GetTransformationMatrix();
					Matrices.push_back(model);
					Matrices.push_back(glm::mat4(e->p_Transform.GetNormalMatrix()));
				}


				if (mesh->p_AlbedoMap.GetTextureID() != 0)
				{
					mesh->p_AlbedoMap.Bind(0);
				}

				m_ReflectionShader.SetVector3f("u_DefaultColor", glm::vec3(mesh->p_Color));
				m_ReflectionShader.SetInteger("u_HasAlbedoMap", static_cast<int>(mesh->p_AlbedoMap.GetTextureID() != 0));

				GLClasses::VertexArray& VAO = mesh->p_VertexArray;
				GLClasses::VertexBuffer& MatrixVBO = mesh->p_MatrixBuffer;

				MatrixVBO.BufferData(Matrices.size() * sizeof(glm::mat4), &Matrices.front(), GL_STATIC_DRAW);
				VAO.Bind();

				if (indexed)
				{
					glDrawElementsInstanced(GL_TRIANGLES, mesh->p_IndicesCount, GL_UNSIGNED_INT, 0, entities.size());
				}

				else
				{
					glDrawArraysInstanced(GL_TRIANGLES, 0, mesh->p_VertexCount, entities.size());
				}

				VAO.Unbind();
			}
		}
	}

	void Renderer::RenderReflectionMapForEntity(const Entity* entity, FPSCamera* camera)
	{
		const glm::vec3& entity_position = entity->p_Transform.GetPosition();
		glm::mat4 projection_matrix;
		const GLClasses::CubeReflectionMap& fbo = entity->p_ReflectionCubemap;

		projection_matrix = glm::perspective(90.0f, camera->GetAspect(), 0.0f, 100.0f);
		fbo.Bind();

		std::array<glm::mat4, 6> view_matrices =
		{
			glm::lookAt(entity_position, entity_position + glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(entity_position, entity_position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(entity_position, entity_position + glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(entity_position, entity_position + glm::vec3( 0.0f,-1.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(entity_position, entity_position + glm::vec3( 0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(entity_position, entity_position + glm::vec3( 0.0f, 0.0f,-1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};

		for (int i = 0; i < 6; i++)
		{
			fbo.BindFace(i);
			m_ReflectionShader.SetMatrix4("u_ViewProjection", projection_matrix * view_matrices[i], 0);
			_RenderEntitesForReflectionMap();
		}

		// Unbind everything
		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		return;
	}

	void Renderer::RenderReflectionMaps(FPSCamera* camera)
	{
		// Set the uniforms
		m_ReflectionShader.Use();
		m_ReflectionShader.SetInteger("u_AlbedoMap", 0, 0);

		for (auto& e : m_RenderEntities)
		{
			for (auto& i : e)
			{
				if (m_CurrentFrame == 0 || m_CurrentFrame % i->p_ReflectionProps.update_rate == 0)
				{
					RenderReflectionMapForEntity(i, camera);
				}
			}
		}

		return;
	}

	void Renderer::RenderShadowMaps()
	{
		/* Render the depth maps of each light */

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		m_DepthShader.Use();

		for (auto& e : m_DirectionalLights)
		{
			if (m_CurrentFrame == 0 || (e.m_UpdateRate > 0 && m_CurrentFrame % e.m_UpdateRate == 0))
			{
				e.m_DepthBuffer.Bind();
				e.m_DepthBuffer.OnUpdate();
				glDisable(GL_CULL_FACE);

				/* Create the view projection matrix for the light */

				e.m_LightSpaceView = glm::lookAt(e.m_ShadowPosition, e.m_ShadowPosition + e.m_Direction, glm::vec3(0.0f, 1.0f, 0.0f));
				e.m_LightSpaceViewProjection = e.m_LightSpaceProjection * e.m_LightSpaceView;

				m_DepthShader.SetMatrix4("u_ViewProjection", e.m_LightSpaceViewProjection);

				for (auto& entities : m_RenderEntities)
				{
					Object* object = entities[0]->p_Object;

					for (auto& e : object->p_Meshes)
					{
						Mesh* mesh = &e;

						const std::vector<Vertex>& Vertices = mesh->p_Vertices;
						const std::vector<GLuint>& Indices = mesh->p_Indices;
						bool indexed = mesh->p_Indexed;

						std::vector<glm::mat4> Matrices;

						for (auto& e : entities)
						{
							const glm::mat4& model = e->p_Transform.GetTransformationMatrix();
							Matrices.push_back(model);
							Matrices.push_back(glm::mat4(e->p_Transform.GetNormalMatrix()));
						}

						GLClasses::VertexArray& VAO = mesh->p_VertexArray;
						GLClasses::VertexBuffer& MatrixVBO = mesh->p_MatrixBuffer;

						MatrixVBO.BufferData(Matrices.size() * sizeof(glm::mat4), &Matrices.front(), GL_STATIC_DRAW);
						VAO.Bind();

						if (indexed)
						{
							glDrawElementsInstanced(GL_TRIANGLES, mesh->p_IndicesCount, GL_UNSIGNED_INT, 0, entities.size());
						}

						else
						{
							glDrawArraysInstanced(GL_TRIANGLES, 0, mesh->p_VertexCount, entities.size());
						}

						VAO.Unbind();
					}
				}
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(0);
	}

	/*
	Adds a group of the same entity at different positions using instanced rendering
	*/
	void Renderer::AddEntityToRenderQueue(const std::vector<const Entity*>& entities)
	{
		const Object* object = entities[0]->p_Object;

		if (object)
		{
			m_RenderEntities.push_back(entities);
		}

		else
		{
			Logger::Log("Attempted to add an entity to the render list without a parent object! || COULD NOT DRAW ENTITY LIST OF SIZE : " + entities.size());
		}

		return;
	}
	
	/* 
	Renders all the entities to the window
	*/
	void Renderer::Render(FPSCamera* camera, const GLClasses::Framebuffer& fbo)
	{
		RenderShadowMaps();
		
		/* Light depth map rendering ends here */

		fbo.Bind();

		if (m_EnvironmentMap)
		{
			m_EnvironmentMap->RenderSkybox(camera);
			m_EnvironmentMap->GetTexture().Bind(4);
		}

		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_RendererShader.Use();
		m_RendererShader.SetFloat("u_AmbientStrength", 0.7f);
		m_RendererShader.SetInteger("u_AlbedoMap", 0);
		m_RendererShader.SetInteger("u_NormalMap", 1);
		m_RendererShader.SetInteger("u_LightMap", 2);
		m_RendererShader.SetInteger("u_Parallax", 3);
		m_RendererShader.SetInteger("u_EnvironmentMap", 4);
		m_RendererShader.SetVector3f("u_ViewerPosition", camera->GetPosition());  
		m_RendererShader.SetMatrix4("u_ViewProjection", camera->GetViewProjection());

		SetLightUniforms(m_RendererShader);
		BindLightingMaps();

		for (auto& entities : m_RenderEntities)
		{
			Object* object = entities[0]->p_Object;

			for (auto& e : object->p_Meshes)
			{
				Mesh* mesh = &e;

				const std::vector<Vertex>& Vertices = mesh->p_Vertices;
				const std::vector<GLuint>& Indices = mesh->p_Indices;
				bool indexed = mesh->p_Indexed;

				std::vector<glm::mat4> Matrices;

				for (auto& e : entities)
				{
					const glm::mat4& model = e->p_Transform.GetTransformationMatrix();
					Matrices.push_back(model);
					Matrices.push_back(glm::mat4(e->p_Transform.GetNormalMatrix()));
				}


				if (mesh->p_AlbedoMap.GetTextureID() != 0)
				{
					mesh->p_AlbedoMap.Bind(0);
				}

				if (mesh->p_NormalMap.GetTextureID() != 0)
				{
					mesh->p_NormalMap.Bind(1);
				}

				/* These uniforms vary from Object to object */
				m_RendererShader.SetVector4f("u_Color", mesh->p_Color);
				m_RendererShader.SetInteger("u_HasAlbedoMap", static_cast<int>(mesh->p_AlbedoMap.GetTextureID() != 0));
				m_RendererShader.SetInteger("u_HasNormalMap", static_cast<int>(mesh->p_NormalMap.GetTextureID() != 0));
				m_RendererShader.SetFloat("u_Reflectance", (float)object->p_Reflectance);

				GLClasses::VertexArray& VAO = mesh->p_VertexArray;
				GLClasses::VertexBuffer& MatrixVBO = mesh->p_MatrixBuffer;

				MatrixVBO.BufferData(Matrices.size() * sizeof(glm::mat4), &Matrices.front(), GL_STATIC_DRAW);
				VAO.Bind();

				if (indexed)
				{
					glDrawElementsInstanced(GL_TRIANGLES, mesh->p_IndicesCount, GL_UNSIGNED_INT, 0, entities.size());
				}

				else
				{
					glDrawArraysInstanced(GL_TRIANGLES, 0, mesh->p_VertexCount, entities.size());
				}

				VAO.Unbind();
			}
		}

		m_RenderEntities.clear();
		glUseProgram(0);

		// Increment the current frame
		m_CurrentFrame++;

		return;
	}

	/*
	Renders a framebuffer onto the main window
	*/
	void Renderer::RenderFBO(const GLClasses::Framebuffer& fbo)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
