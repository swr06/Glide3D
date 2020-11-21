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
#include "../Tonemapper/Tonemapper.h"

namespace Glide3D
{
	class Renderer
	{
	public :
		Renderer(GLFWwindow* window);

		void SetDirectionalLight(DirectionalLight* light);
		void AddPointLight(PointLight* light);
		inline void SetUsePBR(bool v) noexcept { m_UsePBR = v; };
		inline void SetUseBloom(bool v) noexcept { m_HasBloom = v; };
		inline void SetTemperature(float v) noexcept { m_Temperature = v; }
		
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
		GLClasses::Shader m_DepthShader;
		GLClasses::Shader m_ReflectionShader;
		GLClasses::Shader m_DeferredGeometryPassShader;
		GLClasses::Shader m_DeferredLightPassShader;
		GLClasses::Shader m_DepthCubemapShader; // For omni directional shadow maps
		GLClasses::Shader m_VolumetricLightingShader;
		GLClasses::Shader m_BlurShader;
		GLClasses::Shader m_TemperatureTonemappingShader;
		Tonemapper m_Tonemapper;

		// Bloom Shaders
		GLClasses::Shader m_BloomShader;
		GLClasses::Shader m_BloomBrightnessShader;
		GLClasses::Shader m_GaussianVerticalShader;
		GLClasses::Shader m_GaussianHorizontalShader;

		GLClasses::Framebuffer m_VolumetricPassFBO;
		GLClasses::Framebuffer m_VolumetricPassBlurFBO;
		GLClasses::Framebuffer m_LightingPassFBO;
		GLClasses::Framebuffer m_BloomFBO;
		GLClasses::Framebuffer m_TempFBO;

		GLClasses::VertexArray m_FBOVAO;
		GLClasses::VertexBuffer m_FBOVBO;
		GLFWwindow* m_Window;
		GLClasses::SSBO m_LightSSBO;
		DirectionalLight* m_DirectionalLight = nullptr;
		std::vector<PointLight*> m_PointLights;
		std::vector<const Entity*> m_Entities;
		Skybox* m_EnvironmentMap = nullptr;
		uint32_t m_CurrentFrame = 0;
		float u_VolumetricScattering = 0.75f;

		void SetLightUniforms(GLClasses::Shader& shader);
		void BindLightingMaps();
		void RenderShadowMaps(FPSCamera* camera);
		void RenderPointLightShadowMap(PointLight& pointlight);
		void RenderDirectionalLightShadowMaps();
		void RenderPointLightShadowMaps();
		void RenderReflectionMaps(FPSCamera* camera);
		void _RenderEntitesForReflectionMap(const glm::mat4& projection, const glm::mat4& view);
		void RenderReflectionMapForEntity(const Entity* entity, FPSCamera* camera);
		void RenderEnvironmentMap(FPSCamera* camera);
		inline void SetRenderScale(float v) noexcept { m_RenderScale = v; }
		inline float GetRenderScale() const noexcept { return m_RenderScale; }

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
		float m_Temperature = 1000.0f;
		float m_RenderScale = 1.0f;

		bool m_UsePBR = false;
		bool m_HasBloom = false;

		GLuint m_VolumetricNoiseTexture = 0;
		FPSCamera* m_Camera = nullptr;
	};
}