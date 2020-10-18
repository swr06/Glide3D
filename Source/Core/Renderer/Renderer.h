#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "../Object/Object.h"
#include "../Entity/Entity.h"
#include "../Utils/Vertex.h"
#include "../Utils/Defs.h"
#include "../FpsCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* OpenGL Specific stuff */
#include "../GL_Classes/VertexBuffer.h"
#include "../GL_Classes/IndexBuffer.h"
#include "../GL_Classes/VertexArray.h"
#include "../GL_Classes/Shader.h"
#include "../GL_Classes/SSBO.h"
#include "../GL_Classes/Framebuffer.h"
#include "../Skybox.h"

#include "../Lighting/Light.h"

namespace Glide3D
{
	class Renderer
	{
	public :
		Renderer(GLFWwindow* window);

		void AddDirectionalLight(DirectionalLight& light);
		void AddPointLight(const PointLight& light);

		// Rendering
		void AddEntityToRenderQueue(const std::vector<const Entity*>& entities);
		void Render(FPSCamera* camera, const GLClasses::Framebuffer& fbo);

		void RenderFBO(const GLClasses::Framebuffer& fbo);

		inline void SetEnvironmentMap(Skybox& envmap) noexcept
		{
			m_EnvironmentMap = &envmap;
		}

		uint32_t GetCurrentFrame() const noexcept { return m_CurrentFrame; }

	private :

		// The shader programs
		GLClasses::Shader m_RendererShader;
		GLClasses::Shader m_FBOShader;
		GLClasses::Shader m_DepthShader;
		GLClasses::Shader m_ReflectionShader;

		GLClasses::VertexArray m_FBOVAO;
		GLClasses::VertexBuffer m_FBOVBO;
		GLFWwindow* m_Window;
		GLClasses::SSBO m_LightSSBO;
		std::vector<DirectionalLight> m_DirectionalLights;
		std::vector<PointLight> m_PointLights;
		std::vector<std::vector<const Entity*>> m_RenderEntities;
		Skybox* m_EnvironmentMap = nullptr;
		uint32_t m_CurrentFrame = 0;

		void SetLightUniforms(GLClasses::Shader& shader);
		void BindLightingMaps();
		void RenderShadowMaps();
		void RenderPointLightShadowMap(PointLight& pointlight);
		void RenderReflectionMaps(FPSCamera* camera);
		void _RenderEntitesForReflectionMap();
		void RenderReflectionMapForEntity(const Entity* entity, FPSCamera* camera);
	};
}