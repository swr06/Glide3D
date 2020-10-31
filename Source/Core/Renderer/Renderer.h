#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <random>

#include "../Object/Object.h"
#include "../Entity/Entity.h"
#include "../Utils/Vertex.h"
#include "../Utils/Defs.h"
#include "../FpsCamera.h"
#include "../Macros.h"
#include "../GeometryRenderBuffer.h"
#include "../Utils/Timer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

/* OpenGL Specific stuff */
#include "../GL_Classes/VertexBuffer.h"
#include "../GL_Classes/IndexBuffer.h"
#include "../GL_Classes/VertexArray.h"
#include "../GL_Classes/Shader.h"
#include "../GL_Classes/SSBO.h"
#include "../GL_Classes/Framebuffer.h"
#include "../Skybox.h"
#include "../ObjectTypes/Cube.h"

#include "../Lighting/Light.h"

namespace Glide3D
{
	class Renderer
	{
	public :
		Renderer(GLFWwindow* window);

		void AddDirectionalLight(DirectionalLight* light);
		void AddPointLight(PointLight* light);
		void SetUsePBR(bool v) noexcept { m_UsePBR = v; };

		// Rendering
		void AddEntities(const std::vector<const Entity*>& entities);
		void AddEntity(const Entity* entity);
		void Render(FPSCamera* camera, const GLClasses::Framebuffer& fbo);

		void RenderFBO(const GLClasses::Framebuffer& fbo);

		inline void SetEnvironmentMap(Skybox& envmap) noexcept
		{
			m_EnvironmentMap = &envmap;
		}

		uint32_t GetCurrentFrame() const noexcept { return m_CurrentFrame; }
		void RecompileShaders();

	private :

		// The shader programs
		GLClasses::Shader m_RendererShader;
		GLClasses::Shader m_FBOShader; // The hdr shader
		GLClasses::Shader m_DepthShader;
		GLClasses::Shader m_ReflectionShader;
		GLClasses::Shader m_DeferredGeometryPassShader;
		GLClasses::Shader m_DeferredLightPassShader;

		GLClasses::VertexArray m_FBOVAO;
		GLClasses::VertexBuffer m_FBOVBO;
		GLFWwindow* m_Window;
		GLClasses::SSBO m_LightSSBO;
		std::vector<DirectionalLight*> m_DirectionalLights;
		std::vector<PointLight*> m_PointLights;
		std::unordered_map<uint32_t, std::vector<const Entity*>> m_Entities;
		Skybox* m_EnvironmentMap = nullptr;
		uint32_t m_CurrentFrame = 0;

		void SetLightUniforms(GLClasses::Shader& shader);
		void BindLightingMaps();
		void RenderShadowMaps(FPSCamera* camera);
		void RenderPointLightShadowMap(PointLight& pointlight);
		void RenderReflectionMaps(FPSCamera* camera);
		void _RenderEntitesForReflectionMap(const glm::mat4& projection, const glm::mat4& view);
		void RenderReflectionMapForEntity(const Entity* entity, FPSCamera* camera);
		void RenderEnvironmentMap(FPSCamera* camera);

		// Deferred rendering pass stuff
		GeometryRenderBuffer m_GeometryPassBuffer;

		// Tests
		GLClasses::CubeReflectionMap m_ReflectionMap;

		void RenderImGuiElements();
		friend class Application;

		double m_ShadowMapRenderTime = 0;
		double m_ReflectionMapRenderTime = 0;
		double m_GeometryPassTime = 0;
		double m_LightingPassTime = 0;
		double m_TotalRenderTime = 0;
		uint32_t m_DrawCalls = 0;
		uint32_t m_VertexCount = 0;
		uint32_t m_IndexCount = 0;

		float m_Roughness = 0.1f;
		float m_Metalness = 0.1f;

		bool m_UsePBR = false;
	};
}