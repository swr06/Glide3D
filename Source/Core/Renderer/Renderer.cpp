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
		m_DepthShader.CreateShaderProgramFromFile("Core/Shaders/DepthVert.glsl", "Core/Shaders/DepthFrag.glsl");
		m_DepthShader.CompileShaders();
		m_ReflectionShader.CreateShaderProgramFromFile("Core/Shaders/ReflectionVert.glsl", "Core/Shaders/ReflectionFrag.glsl");
		m_ReflectionShader.CompileShaders();
		m_DeferredGeometryPassShader.CreateShaderProgramFromFile("Core/Shaders/GeometryPassVert.glsl", "Core/Shaders/GeometryPassFrag.glsl");
		m_DeferredGeometryPassShader.CompileShaders();
		m_DeferredLightPassShader.CreateShaderProgramFromFile("Core/Shaders/LightingPassVert.glsl", "Core/Shaders/LightingPassFrag.glsl");
		m_DeferredLightPassShader.CompileShaders();
		m_DepthCubemapShader.CreateShaderProgramFromFile("Core/Shaders/DepthCubemapVert.glsl", "Core/Shaders/DepthCubemapFrag.glsl", "Core/Shaders/DepthCubemapGeometry.glsl");
		m_DepthCubemapShader.CompileShaders();
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

	void Renderer::AddPointLight(PointLight* light)
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
			shader.SetFloat(name + ".m_ShadowStrength", m_DirectionalLights[i]->m_ShadowStrength);
			shader.SetInteger(name + ".m_IsBlinn", (int)m_DirectionalLights[i]->m_IsBlinn);
		}

		for (int i = 0; i < m_PointLights.size(); i++)
		{
			std::string name("u_ScenePointLights[");
			name = name + std::to_string(i) + "]";

			shader.SetVector3f(name + ".m_Position", m_PointLights[i]->m_Position);
			shader.SetVector3f(name + ".m_SpecularColor", m_PointLights[i]->m_SpecularColor);
			shader.SetInteger(name + ".m_SpecularExponent", m_PointLights[i]->m_SpecularExponent);
			shader.SetFloat(name + ".m_SpecularStrength", m_PointLights[i]->m_SpecularStrength);
			shader.SetFloat(name + ".m_Linear", m_PointLights[i]->m_Linear);
			shader.SetFloat(name + ".m_Constant", m_PointLights[i]->m_Constant);
			shader.SetFloat(name + ".m_Quadratic", m_PointLights[i]->m_Quadratic);
			shader.SetInteger(name + ".m_IsBlinn", (int)m_PointLights[i]->m_IsBlinn);
			shader.SetFloat(name + ".m_ShadowStrength", (int)m_PointLights[i]->m_ShadowStrength);
			shader.SetFloat(name + ".m_FarPlane", (int)m_PointLights[i]->m_FarPlane);
		}

		for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++)
		{
			shader.SetInteger("m_DirectionalLightShadowmaps[" + std::to_string(i) + "]", (int)6 + i); // 5 slots are used for the materials
		}

		for (int i = 0; i < MAX_POINT_LIGHTS; i++)
		{
			shader.SetInteger("m_PointlightShadowmaps[" + std::to_string(i) + "]", (int)9 + i); 
		}
	}

	void Renderer::BindLightingMaps()
	{
		for (int i = 0 ; i < m_DirectionalLights.size() ; i++)
		{
			glActiveTexture(GL_TEXTURE6 + i);
			glBindTexture(GL_TEXTURE_2D, m_DirectionalLights[i]->m_DepthBuffer.GetDepthTexture());
		}

		for (int i = 0; i < m_PointLights.size(); i++)
		{
			glActiveTexture(GL_TEXTURE9 + i);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_PointLights[i]->m_DepthShadowMap.GetTexture());
		}
	}

	void Renderer::RenderPointLightShadowMap(PointLight& pointlight)
	{
		if (m_CurrentFrame == 0 || (pointlight.m_ShadowMapUpdateRate > 0 && m_CurrentFrame % pointlight.m_ShadowMapUpdateRate == 0))
		{
			// Bind the point light's fbo
			pointlight.m_DepthShadowMap.Bind();
			glClear(GL_DEPTH_BUFFER_BIT);

			glm::vec3 light_pos = pointlight.m_Position;

			m_DepthCubemapShader.Use();

			std::array<glm::mat4, 6> view_matrices =
			{
				glm::lookAt(light_pos, light_pos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
				glm::lookAt(light_pos, light_pos + glm::vec3(-1.0f,0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
				glm::lookAt(light_pos, light_pos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
				glm::lookAt(light_pos, light_pos + glm::vec3(0.0f,-1.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
				glm::lookAt(light_pos, light_pos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
				glm::lookAt(light_pos, light_pos + glm::vec3(0.0f, 0.0f,-1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
			};

			glm::mat4 projection_matrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.2f, pointlight.m_FarPlane);

			for (int i = 0; i < 6; i++)
			{
				m_DepthCubemapShader.SetMatrix4("u_ViewProjectionMatrices[" + std::to_string(i) + "]", projection_matrix * view_matrices[i]);
			}

			m_DepthCubemapShader.SetFloat("u_FarPlane", pointlight.m_FarPlane);
			m_DepthCubemapShader.SetVector3f("u_LightPosition", light_pos);
			m_DepthCubemapShader.Use();

			for (auto& entities : m_Entities)
			{
				if (entities.second.size() <= 0) { continue; }
				const Object* object = entities.second[0]->p_Object;

				for (auto& e : object->m_Meshes)
				{
					const Mesh* mesh = &e;
					bool indexed = mesh->p_Indexed;
					const GLClasses::VertexArray& VAO = mesh->p_VertexArray;

					VAO.Bind();

					if (indexed)
					{
						glDrawElementsInstanced(GL_TRIANGLES, mesh->p_IndicesCount, GL_UNSIGNED_INT, 0, entities.second.size());
					}

					else
					{
						glDrawArraysInstanced(GL_TRIANGLES, 0, mesh->p_VertexCount, entities.second.size());
					}

					VAO.Unbind();
				}
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	void Renderer::RenderDirectionalLightShadowMaps()
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

				for (auto& entities : m_Entities)
				{
					if (entities.second.size() <= 0) { continue; }

					const Object* object = entities.second[0]->p_Object;

					for (auto& e : object->m_Meshes)
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
							glDrawElementsInstanced(GL_TRIANGLES, mesh->p_IndicesCount, GL_UNSIGNED_INT, 0, entities.second.size());
						}

						else
						{
							glDrawArraysInstanced(GL_TRIANGLES, 0, mesh->p_VertexCount, entities.second.size());
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
	}

	void Renderer::RenderPointLightShadowMaps()
	{
		for (auto& e : m_PointLights)
		{
			PointLight& pointlight = *e;
			RenderPointLightShadowMap(pointlight);
		}
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

		for (auto& entities : m_Entities)
		{
			if (entities.second.size() <= 0) { continue; }

			const Object* object = entities.second[0]->p_Object;

			for (auto& e : object->m_Meshes)
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
					glDrawElementsInstanced(GL_TRIANGLES, mesh->p_IndicesCount, GL_UNSIGNED_INT, 0, entities.second.size());
				}

				else 
				{
					glDrawArraysInstanced(GL_TRIANGLES, 0, mesh->p_VertexCount, entities.second.size());
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

	void Renderer::RenderImGuiElements()
	{
		if (ImGui::Begin("Renderer Stats"))
		{
			ImGui::Text("Geometry Pass Render Time : %f ms", m_GeometryPassTime);
			ImGui::Text("Lighting Pass Time : %f ms", m_LightingPassTime);
			ImGui::Text("Shadow Map Render Time : %f ms", m_ShadowMapRenderTime);
			ImGui::Text("Reflection Map Render Time : %f ms", m_ReflectionMapRenderTime);
			ImGui::Text("Total Render Time : %f ms", m_TotalRenderTime);
			ImGui::Text("Total Draw Calls : %d", m_DrawCalls);
			ImGui::Text("Total Vertices : %d", m_VertexCount);
			ImGui::Text("Total Indices : %d", m_IndexCount);
			ImGui::End();
		}
	}

	void Renderer::RenderReflectionMaps(FPSCamera* camera)
	{
		m_ReflectionMapRenderTime = glfwGetTime();

		/*
		// Set the uniforms
		m_ReflectionShader.Use();
		m_ReflectionShader.SetInteger("u_AlbedoMap", 0, 0);

		glm::mat4 projection_matrix;

		for (auto& entities : m_Entities)
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

		m_ReflectionMapRenderTime = glfwGetTime() - m_ReflectionMapRenderTime;
		m_ReflectionMapRenderTime *= 1000.0f;

		return;
	}

	void Renderer::RenderShadowMaps(FPSCamera* camera)
	{
		m_ShadowMapRenderTime = glfwGetTime();

		RenderDirectionalLightShadowMaps();
		RenderPointLightShadowMaps();

		m_ShadowMapRenderTime = glfwGetTime() - m_ShadowMapRenderTime;
		m_ShadowMapRenderTime *= 1000.0f;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(0);
	}

	/*
	Adds a group of the same entity at different positions using instanced rendering
	*/
	void Renderer::AddEntities(const std::vector<const Entity*>& entities)
	{
		for (auto& e : entities)
		{
			const Object* object = e->p_Object;

			if (object)
			{
				m_Entities[object->GetID()].push_back(e);
			}

			else
			{
				Logger::Log("Attempted to add an entity to the render list without a parent object! || COULD NOT DRAW ENTITY LIST OF SIZE : " + entities.size());
			}
		}

		Logger::Log("Entity array of size : " + std::to_string(entities.size()) + " Was added to the entity list");

		return;
	}

	void Renderer::AddEntity(const Entity* entity)
	{
		const Object* object = entity->p_Object;

		if (object)
		{
			m_Entities[object->GetID()].push_back(entity);
		}

		else
		{
			Logger::Log("Attempted to add an entity to the render list without a parent object!");
		}
	}
	
	/* 
	Renders all the entities to the window
	*/
	void Renderer::Render(FPSCamera* camera, const GLClasses::Framebuffer& fbo)
	{
		m_DrawCalls = 0;
		m_VertexCount = 0;
		m_IndexCount = 0;
		m_TotalRenderTime = glfwGetTime();
		m_GeometryPassBuffer.SetDimensions(fbo.GetWidth(), fbo.GetHeight());

		for (auto& entities : m_Entities)
		{
			if (entities.second.size() <= 0) { continue; }
			const Object* object = entities.second[0]->p_Object;

			std::vector<glm::mat4> Matrices;

			for (auto& e : entities.second)
			{
				const glm::mat4& model = e->p_Transform.GetTransformationMatrix();
				Matrices.push_back(model);
				Matrices.push_back(glm::mat4(e->p_Transform.GetNormalMatrix()));
			}

			const GLClasses::VertexBuffer& MatrixVBO = object->m_MatrixBuffer;
			MatrixVBO.BufferData(Matrices.size() * sizeof(glm::mat4), &Matrices.front(), GL_STATIC_DRAW);
		}

		if (m_CurrentFrame == 0)
		{
			RenderReflectionMaps(camera);
		}

		RenderShadowMaps(camera);

		// Deferred shading pipeline starts here

		/* Geometry Pass starts here */

		m_GeometryPassTime = glfwGetTime();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_GeometryPassBuffer.Bind();

		// Clear the geometry buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_DeferredGeometryPassShader.Use();
		m_DeferredGeometryPassShader.SetMatrix4("u_ViewProjection", camera->GetViewProjection());
		
		m_DeferredGeometryPassShader.SetInteger("u_AlbedoMap", 0, 0);
		m_DeferredGeometryPassShader.SetInteger("u_NormalMap", 1, 0);
		m_DeferredGeometryPassShader.SetInteger("u_SpecularMap", 2, 0);

		m_DeferredGeometryPassShader.SetInteger("u_MetalnessMap", 3, 0);
		m_DeferredGeometryPassShader.SetInteger("u_RoughnessMap", 4, 0);
		m_DeferredGeometryPassShader.SetInteger("u_AOMap", 5, 0);

		m_DeferredGeometryPassShader.SetInteger("u_EnvironmentMap", 6, 0);
		m_DeferredGeometryPassShader.SetVector3f("u_ViewerPosition", camera->GetPosition());
		m_DeferredGeometryPassShader.SetFloat("u_Time", (float)glfwGetTime());

		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ReflectionMap.GetTexture());

		for (auto& entities : m_Entities)
		{
			if (entities.second.size() <= 0) { continue; }

			const Object* object = entities.second[0]->p_Object;

			if (object->p_CanFacecull)
			{
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
			}

			std::vector<glm::mat4> Matrices;

			for (auto& e : entities.second)
			{
				const glm::mat4& model = e->p_Transform.GetTransformationMatrix();
				Matrices.push_back(model);
				Matrices.push_back(glm::mat4(e->p_Transform.GetNormalMatrix()));
			}

			const GLClasses::VertexBuffer& MatrixVBO = object->m_MatrixBuffer;
			MatrixVBO.BufferData(Matrices.size() * sizeof(glm::mat4), &Matrices.front(), GL_STATIC_DRAW);

			for (auto& e : object->m_Meshes)
			{
				if (e.p_HasWavePhysics)
				{
					m_DeferredGeometryPassShader.SetBool("u_HasWavePhysics", true);
					m_DeferredGeometryPassShader.SetFloat("u_WavePhysicsProps.m_WavePosY", e.p_WaveAffectY);
					m_DeferredGeometryPassShader.SetVector2f("u_WavePhysicsProps.m_WaveFreq", e.p_WaveAffectFreq);
					m_DeferredGeometryPassShader.SetVector2f("u_WavePhysicsProps.m_WaveSpeed", e.p_WaveAffectSpeed);
				}

				else
				{
					m_DeferredGeometryPassShader.SetInteger("u_HasWavePhysics", 0);
				}

				const Mesh* mesh = &e;

				m_VertexCount += mesh->p_VertexCount;
				m_IndexCount += mesh->p_IndicesCount;

				bool indexed = mesh->p_Indexed;

				if (mesh->p_AlbedoMap.GetTextureID() != 0)
				{
					mesh->p_AlbedoMap.Bind(0);
				}

				if (mesh->p_NormalMap.GetTextureID() != 0)
				{
					mesh->p_NormalMap.Bind(1);
				}

				if (mesh->p_MetalnessMap.GetTextureID() != 0)
				{
					mesh->p_MetalnessMap.Bind(3);
				}

				if (mesh->p_RoughnessMap.GetTextureID() != 0)
				{
					mesh->p_RoughnessMap.Bind(4);
				}

				if (mesh->p_AmbientOcclusionMap.GetTextureID() != 0)
				{
					mesh->p_AmbientOcclusionMap.Bind(5);
				}

				m_DeferredGeometryPassShader.SetVector4f("u_Color", mesh->p_Color);
				m_DeferredGeometryPassShader.SetBool("u_HasAlbedoMap", mesh->p_AlbedoMap.GetTextureID() != 0);
				m_DeferredGeometryPassShader.SetBool("u_HasNormalMap", mesh->p_NormalMap.GetTextureID() != 0);
				m_DeferredGeometryPassShader.SetBool("u_HasReflections", mesh->p_Reflectivity != glm::vec3(0.0f));
				m_DeferredGeometryPassShader.SetBool("u_HasMetalnessMap", mesh->p_MetalnessMap.GetTextureID() != 0);
				m_DeferredGeometryPassShader.SetBool("u_HasRoughnessMap", mesh->p_RoughnessMap.GetTextureID() != 0);
				m_DeferredGeometryPassShader.SetBool("u_HasAOMap", mesh->p_AmbientOcclusionMap.GetTextureID() != 0);
				m_DeferredGeometryPassShader.SetVector3f("u_Reflectance", mesh->p_Reflectivity);
				
				const GLClasses::VertexArray& VAO = mesh->p_VertexArray;
				VAO.Bind();

				if (indexed)
				{
					glDrawElementsInstanced(GL_TRIANGLES, mesh->p_IndicesCount, GL_UNSIGNED_INT, 0, entities.second.size());
					m_DrawCalls++;
				}

				else
				{
					glDrawArraysInstanced(GL_TRIANGLES, 0, mesh->p_VertexCount, entities.second.size());
					m_DrawCalls++;
				}

				VAO.Unbind();
			}
		}

		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_GeometryPassTime = glfwGetTime() - m_GeometryPassTime;
		m_GeometryPassTime *= 1000.0f;

		/* Geometry Pass ends here */
		
		/* Lighting Pass starts here */

		m_LightingPassTime = glfwGetTime();

		fbo.Bind();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		RenderEnvironmentMap(camera);
		m_DeferredLightPassShader.Use();

		// Deferred renderer textures
		m_DeferredLightPassShader.SetInteger("u_PositionTexture", 0);
		m_DeferredLightPassShader.SetInteger("u_NormalTexture", 1);
		m_DeferredLightPassShader.SetInteger("u_ColorTexture", 2);
		m_DeferredLightPassShader.SetInteger("u_PBRComponentTexture", 3);

		m_DeferredLightPassShader.SetVector3f("u_ViewerPosition", camera->GetPosition());
		m_DeferredLightPassShader.SetVector3f("u_AmbientLight", glm::vec3(1.0f));

		// PBR
		m_DeferredLightPassShader.SetBool("u_UsesPBRLighting", m_UsePBR);
		
		SetLightUniforms(m_DeferredLightPassShader);
		BindLightingMaps();

		// Bind the textures from the deferred rendering stage
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_GeometryPassBuffer.GetPositionTexture());

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_GeometryPassBuffer.GetNormalTexture());

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_GeometryPassBuffer.GetColorTexture());

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_GeometryPassBuffer.GetPBRComponentTexture());

		m_FBOVAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		m_FBOVAO.Unbind();

		m_LightingPassTime = glfwGetTime() - m_LightingPassTime;
		m_LightingPassTime *= 1000.0f;

		glDisable(GL_BLEND);

		/* Lighting Pass ends here */

		// Clean up
		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Increment the current frame
		m_CurrentFrame++;

		m_TotalRenderTime = glfwGetTime() - m_TotalRenderTime;
		m_TotalRenderTime *= 1000.0f;

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

			// Do Tonemapping
			m_Tonemapper.Render(fbo);
		}
	}

	void Renderer::RecompileShaders()
	{
		if (m_EnvironmentMap)
		{
			m_EnvironmentMap->Recompile();
		}

		m_RendererShader.Destroy();
		m_DepthShader.Destroy();
		m_ReflectionShader.Destroy();
		m_DeferredGeometryPassShader.Destroy();
		m_DeferredLightPassShader.Destroy();

		m_RendererShader.CreateShaderProgramFromFile("Core/Shaders/RendererVert.glsl", "Core/Shaders/RendererFrag.glsl");
		m_RendererShader.CompileShaders();
		m_DepthShader.CreateShaderProgramFromFile("Core/Shaders/DepthVert.glsl", "Core/Shaders/DepthFrag.glsl");
		m_DepthShader.CompileShaders();
		m_ReflectionShader.CreateShaderProgramFromFile("Core/Shaders/ReflectionVert.glsl", "Core/Shaders/ReflectionFrag.glsl");
		m_ReflectionShader.CompileShaders();
		m_DeferredGeometryPassShader.CreateShaderProgramFromFile("Core/Shaders/GeometryPassVert.glsl", "Core/Shaders/GeometryPassFrag.glsl");
		m_DeferredGeometryPassShader.CompileShaders();
		m_DeferredLightPassShader.CreateShaderProgramFromFile("Core/Shaders/LightingPassVert.glsl", "Core/Shaders/LightingPassFrag.glsl");
		m_DeferredLightPassShader.CompileShaders();
	}
}
