#include "Renderer.h"

/*
The Glide3D Rendering Engine
- Uses instanced rendering
- Deffered shading pipeline
*/

namespace Glide3D
{
	Renderer::Renderer(GLFWwindow* window) : 
		m_FBOVBO(GL_ARRAY_BUFFER), m_Window(window), m_ReflectionMap(128), m_GeometryPassBuffer(800, 600)
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
		m_FBOShader.CreateShaderProgramFromFile("Core/Shaders/FramebufferHDRVert.glsl", "Core/Shaders/FramebufferHDRFrag.glsl");
		m_FBOShader.CompileShaders();
		m_DepthShader.CreateShaderProgramFromFile("Core/Shaders/DepthVert.glsl", "Core/Shaders/DepthFrag.glsl");
		m_DepthShader.CompileShaders();
		m_ReflectionShader.CreateShaderProgramFromFile("Core/Shaders/ReflectionVert.glsl", "Core/Shaders/ReflectionFrag.glsl");
		m_ReflectionShader.CompileShaders();
		m_DeferredGeometryPassShader.CreateShaderProgramFromFile("Core/Shaders/GeometryPassVert.glsl", "Core/Shaders/GeometryPassFrag.glsl");
		m_DeferredGeometryPassShader.CompileShaders();
		m_DeferredLightPassShader.CreateShaderProgramFromFile("Core/Shaders/LightingPassVert.glsl", "Core/Shaders/LightingPassFrag.glsl");
		m_DeferredLightPassShader.CompileShaders();
	}

	void Renderer::AddDirectionalLight(DirectionalLight* light)
	{
		if (m_DirectionalLights.size() + 1 > MAX_DIRECTIONAL_LIGHTS)
		{
			std::stringstream s;
			s << "Max DIRECTIONAL lights allowed is : " << MAX_DIRECTIONAL_LIGHTS << " || Light could not be added!";
			Logger::Log(s.str());
			assert(0);
		}

		m_DirectionalLights.emplace_back(light);
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
		shader.Use();
		shader.SetInteger("u_SceneDirectionalLightCount", m_DirectionalLights.size(), 0);
		shader.SetInteger("u_NumDirectionalLights", m_DirectionalLights.size(), 0);
		shader.SetInteger("u_ScenePointLightCount", m_PointLights.size(), 0);

		for (int i = 0; i < m_DirectionalLights.size(); i++)
		{
			std::string name("u_SceneDirectionalLights[");
			name = name + std::to_string(i) + "]";

			std::string matname = "u_DirectionalLightSpaceVP[" + std::to_string(i) + "]";
			shader.SetMatrix4(matname,
				m_DirectionalLights[i]->m_LightSpaceViewProjection, 0);

			shader.SetVector3f(name + ".m_Direction", m_DirectionalLights[i]->m_Direction);
			shader.SetVector3f(name + ".m_SpecularColor", m_DirectionalLights[i]->m_SpecularColor);
			shader.SetInteger(name + ".m_SpecularExponent", m_DirectionalLights[i]->m_SpecularExponent);
			shader.SetFloat(name + ".m_SpecularStrength", m_DirectionalLights[i]->m_SpecularStrength);
			shader.SetInteger(name + ".m_IsBlinn", (int)m_DirectionalLights[i]->m_IsBlinn);
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
			glBindTexture(GL_TEXTURE_2D, m_DirectionalLights[i]->m_DepthBuffer.GetDepthTexture());
		}
	}

	void Renderer::RenderPointLightShadowMap(PointLight& pointlight)
	{
		// Todo
	}

	void Renderer::_RenderEntitesForReflectionMap(const glm::mat4& projection, const glm::mat4& view)
	{
		// Todo : Implement this with geometry shaders

		if (m_EnvironmentMap)
		{
			m_EnvironmentMap->RenderSkybox(projection, view);
		}

		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);
		glBlendFunc(GL_NONE, GL_NONE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);

		m_ReflectionShader.Use();

		for (auto& entities : m_RenderEntities)
		{
			const Object* object = entities[0]->p_Object;

			std::vector<glm::mat4> Matrices;

			for (auto& e : entities)
			{
				const glm::mat4& model = e->p_Transform.GetTransformationMatrix();
				Matrices.push_back(model);
				Matrices.push_back(glm::mat4(e->p_Transform.GetNormalMatrix()));
			}

			const GLClasses::VertexBuffer& MatrixVBO = object->p_MatrixBuffer;
			MatrixVBO.BufferData(Matrices.size() * sizeof(glm::mat4), &Matrices.front(), GL_STATIC_DRAW);

			for (auto& e : object->p_Meshes)
			{
				const Mesh* mesh = &e;

				bool indexed = mesh->p_Indexed;

				if (mesh->p_AlbedoMap.GetTextureID() != 0)
				{
					mesh->p_AlbedoMap.Bind(0);
				}

				m_ReflectionShader.SetVector3f("u_DefaultColor", glm::vec3(mesh->p_Color));
				m_ReflectionShader.SetInteger("u_HasAlbedoMap", static_cast<int>(mesh->p_AlbedoMap.GetTextureID() != 0));

				const GLClasses::VertexArray& VAO = mesh->p_VertexArray;
				VAO.Bind();
				glDisableVertexAttribArray(1);
				glDisableVertexAttribArray(3);
				glDisableVertexAttribArray(4);
				glDisableVertexAttribArray(9);
				glDisableVertexAttribArray(10);
				glDisableVertexAttribArray(11);
				glDisableVertexAttribArray(12);

				if (indexed)
				{
					glDrawElementsInstanced(GL_TRIANGLES, mesh->p_IndicesCount, GL_UNSIGNED_INT, 0, entities.size());
				}

				else 
				{
					glDrawArraysInstanced(GL_TRIANGLES, 0, mesh->p_VertexCount, entities.size());
				}

				glEnableVertexAttribArray(1);
				glEnableVertexAttribArray(3);
				glEnableVertexAttribArray(4);
				glEnableVertexAttribArray(9);
				glEnableVertexAttribArray(10);
				glEnableVertexAttribArray(11);
				glEnableVertexAttribArray(12);
				VAO.Unbind();
			}
		}

		return;
	}

	void Renderer::RenderReflectionMapForEntity(const Entity* entity, FPSCamera* camera)
	{
		// Todo

		return;
	}

	void Renderer::RenderEnvironmentMap(FPSCamera* camera)
	{
		if (m_EnvironmentMap)
		{
			m_EnvironmentMap->RenderSkybox(camera);
		}
	}

	void Renderer::RenderReflectionMaps(FPSCamera* camera)
	{
		/*
		// Set the uniforms
		m_ReflectionShader.Use();
		m_ReflectionShader.SetInteger("u_AlbedoMap", 0, 0);

		glm::mat4 projection_matrix;

		for (auto& entities : m_RenderEntities)
		{
			for (auto& e : entities)
			{
				if (!e->p_RenderReflectionCubemap) { continue; }

				const GLClasses::CubeReflectionMap& fbo = e->p_ReflectionCubemap;
				const glm::vec3& entity_position = e->p_ReflectionPosition;

				projection_matrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 150.0f);
				fbo.Bind();

				std::array<glm::mat4, 6> view_matrices =
				{
					glm::lookAt(entity_position, entity_position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
					glm::lookAt(entity_position, entity_position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
					glm::lookAt(entity_position, entity_position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
					glm::lookAt(entity_position, entity_position + glm::vec3(0.0f,-1.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
					glm::lookAt(entity_position, entity_position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
					glm::lookAt(entity_position, entity_position + glm::vec3(0.0f, 0.0f,-1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
				};

				for (int i = 0; i < 6; i++)
				{
					fbo.BindFace(i);
					m_ReflectionShader.SetMatrix4("u_ViewProjection", projection_matrix * view_matrices[i], 1);
					_RenderEntitesForReflectionMap(projection_matrix, view_matrices[i]);
				}
			}
		}*/

		// Set the uniforms
		m_ReflectionShader.Use();
		m_ReflectionShader.SetInteger("u_AlbedoMap", 0, 0);

		const glm::vec3& entity_position = glm::vec3(27.0f, 4.0f, 19.0f);
		glm::mat4 projection_matrix;
		const GLClasses::CubeReflectionMap& fbo = m_ReflectionMap;

		projection_matrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 150.0f);
		fbo.Bind();

		std::array<glm::mat4, 6> view_matrices =
		{
			glm::lookAt(entity_position, entity_position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(entity_position, entity_position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(entity_position, entity_position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(entity_position, entity_position + glm::vec3(0.0f,-1.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(entity_position, entity_position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(entity_position, entity_position + glm::vec3(0.0f, 0.0f,-1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};

		for (int i = 0; i < 6; i++)
		{
			fbo.BindFace(i);
			m_ReflectionShader.SetMatrix4("u_ViewProjection", projection_matrix * view_matrices[i], 1);
			_RenderEntitesForReflectionMap(projection_matrix, view_matrices[i]);
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
			if (m_CurrentFrame == 0 || (e->m_UpdateRate > 0 && m_CurrentFrame % e->m_UpdateRate == 0))
			{
				e->m_DepthBuffer.Bind();
				e->m_DepthBuffer.OnUpdate();
				glDisable(GL_CULL_FACE);

				/* Create the view projection matrix for the light */

				e->m_LightSpaceView = glm::lookAt(e->m_ShadowPosition, e->m_ShadowPosition + e->m_Direction, glm::vec3(0.0f, 1.0f, 0.0f));
				e->m_LightSpaceViewProjection = e->m_LightSpaceProjection * e->m_LightSpaceView;

				m_DepthShader.SetMatrix4("u_ViewProjection", e->m_LightSpaceViewProjection);

				for (auto& entities : m_RenderEntities)
				{
					const Object* object = entities[0]->p_Object;

					std::vector<glm::mat4> Matrices;

					for (auto& e : entities)
					{
						const glm::mat4& model = e->p_Transform.GetTransformationMatrix();
						Matrices.push_back(model);
						Matrices.push_back(glm::mat4(e->p_Transform.GetNormalMatrix()));
					}

					const GLClasses::VertexBuffer& MatrixVBO = object->p_MatrixBuffer;
					MatrixVBO.BufferData(Matrices.size() * sizeof(glm::mat4), &Matrices.front(), GL_STATIC_DRAW);

					for (auto& e : object->p_Meshes)
					{
						const Mesh* mesh = &e;

						const std::vector<Vertex>& Vertices = mesh->p_Vertices;
						const std::vector<GLuint>& Indices = mesh->p_Indices;
						bool indexed = mesh->p_Indexed;

						const GLClasses::VertexArray& VAO = mesh->p_VertexArray;
						VAO.Bind();

						glDisableVertexAttribArray(1);
						glDisableVertexAttribArray(2);
						glDisableVertexAttribArray(3);
						glDisableVertexAttribArray(4);
						glDisableVertexAttribArray(9);
						glDisableVertexAttribArray(10);
						glDisableVertexAttribArray(11);
						glDisableVertexAttribArray(12);

						if (indexed)
						{
							glDrawElementsInstanced(GL_TRIANGLES, mesh->p_IndicesCount, GL_UNSIGNED_INT, 0, entities.size());
						}

						else
						{
							glDrawArraysInstanced(GL_TRIANGLES, 0, mesh->p_VertexCount, entities.size());
						}

						glEnableVertexAttribArray(1);
						glEnableVertexAttribArray(2);
						glEnableVertexAttribArray(3);
						glEnableVertexAttribArray(4);
						glEnableVertexAttribArray(9);
						glEnableVertexAttribArray(10);
						glEnableVertexAttribArray(11);
						glEnableVertexAttribArray(12);

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
	void Renderer::AddEntities(const std::vector<const Entity*>& entities)
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
		m_GeometryPassBuffer.SetDimensions(fbo.GetWidth(), fbo.GetHeight());

		if (m_CurrentFrame == 0)
		{
			RenderReflectionMaps(camera);
		}

		RenderShadowMaps();

		// Deferred shading pipeline starts here

		/* Geometry Pass starts here */

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_GeometryPassBuffer.Bind();

		// Clear the geometry buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_DeferredGeometryPassShader.Use();
		m_DeferredGeometryPassShader.SetMatrix4("u_ViewProjection", camera->GetViewProjection());
		m_DeferredGeometryPassShader.SetInteger("u_AlbedoMap", 0, 0);
		m_DeferredGeometryPassShader.SetInteger("u_NormalMap", 1, 0);
		m_DeferredGeometryPassShader.SetInteger("u_SpecularMap", 2, 0);
		m_DeferredGeometryPassShader.SetInteger("u_EnvironmentMap", 3, 0);
		m_DeferredGeometryPassShader.SetVector3f("u_ViewerPosition", camera->GetPosition());
		
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ReflectionMap.GetTexture());

		for (auto& entities : m_RenderEntities)
		{
			const Object* object = entities[0]->p_Object;

			std::vector<glm::mat4> Matrices;

			for (auto& e : entities)
			{
				const glm::mat4& model = e->p_Transform.GetTransformationMatrix();
				Matrices.push_back(model);
				Matrices.push_back(glm::mat4(e->p_Transform.GetNormalMatrix()));
			}

			const GLClasses::VertexBuffer& MatrixVBO = object->p_MatrixBuffer;
			MatrixVBO.BufferData(Matrices.size() * sizeof(glm::mat4), &Matrices.front(), GL_STATIC_DRAW);

			for (auto& e : object->p_Meshes)
			{
				const Mesh* mesh = &e;

				bool indexed = mesh->p_Indexed;

				if (mesh->p_AlbedoMap.GetTextureID() != 0)
				{
					mesh->p_AlbedoMap.Bind(0);
				}

				if (mesh->p_NormalMap.GetTextureID() != 0)
				{
					mesh->p_NormalMap.Bind(1);
				}

				m_DeferredGeometryPassShader.SetVector4f("u_Color", mesh->p_Color);
				m_DeferredGeometryPassShader.SetInteger("u_HasAlbedoMap", static_cast<int>(mesh->p_AlbedoMap.GetTextureID() != 0));
				m_DeferredGeometryPassShader.SetInteger("u_HasNormalMap", static_cast<int>(mesh->p_NormalMap.GetTextureID() != 0));
				m_DeferredGeometryPassShader.SetInteger("u_HasReflections", static_cast<int>(mesh->p_Reflectivity != glm::vec3(0.0f)));
				m_DeferredGeometryPassShader.SetVector3f("u_Reflectance", mesh->p_Reflectivity);
				
				const GLClasses::VertexArray& VAO = mesh->p_VertexArray;
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

		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/* Geometry Pass ends here */
		
		/* Lighting Pass starts here */

		fbo.Bind();
		RenderEnvironmentMap(camera);
		m_DeferredLightPassShader.Use();
		m_DeferredLightPassShader.SetInteger("u_PositionTexture", 0);
		m_DeferredLightPassShader.SetInteger("u_NormalTexture", 1);
		m_DeferredLightPassShader.SetInteger("u_ColorTexture", 2);
		m_DeferredLightPassShader.SetVector3f("u_ViewerPosition", camera->GetPosition());
		m_DeferredLightPassShader.SetVector3f("u_AmbientLight", glm::vec3(1.0f));
		SetLightUniforms(m_DeferredLightPassShader);
		BindLightingMaps();

		// Bind the textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_GeometryPassBuffer.GetPositionTexture());

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_GeometryPassBuffer.GetNormalTexture());

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_GeometryPassBuffer.GetColorTexture());

		m_FBOVAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		m_FBOVAO.Unbind();

		/* Lighting Pass ends here */

		// Clean up
		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Increment the current frame
		m_CurrentFrame++;

		return;
	}

	/*
	Renders a framebuffer onto the main window
	*/
	void Renderer::RenderFBO(const GLClasses::Framebuffer& fbo)
	{
		if (!fbo.IsHDR())
		{
			int w = 0, h = 0;

			glfwGetFramebufferSize(m_Window, &w, &h);

			fbo.Bind();
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glBlitFramebuffer(0, 0, fbo.GetWidth(), fbo.GetHeight(), 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
			glEnable(GL_DEPTH_TEST);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glClear(GL_COLOR_BUFFER_BIT);
			glClear(GL_DEPTH_BUFFER_BIT);

			glDisable(GL_DEPTH_TEST);
			m_FBOShader.Use();
			m_FBOShader.SetInteger("u_FramebufferTexture", 1);
			m_FBOShader.SetFloat("u_Exposure", fbo.GetExposure());
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, fbo.GetTexture());

			m_FBOVAO.Bind();
			glDrawArrays(GL_TRIANGLES, 0, 6);
			m_FBOVAO.Unbind();

			glEnable(GL_DEPTH_TEST);

			glUseProgram(0);
		}
	}
}
