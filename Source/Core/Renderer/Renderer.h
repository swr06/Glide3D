#include <iostream>
#include <vector>
#include <map>
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

#include "../Lighting/Light.h"

namespace Glide3D
{
	class Renderer
	{
	public :
		Renderer(GLFWwindow* window);

		void AddLight(const DirectionalLight& light);

		// Rendering
		void StartRender(const FPSCamera* camera);
		void RenderObjects(const std::vector<Entity>& entities);
		void EndRender();

		void RenderFBO(const GLClasses::Framebuffer& fbo);

	private :
		GLClasses::VertexArray m_VAO;
		GLClasses::VertexBuffer m_VBO;
		GLClasses::VertexBuffer m_MatrixVBO;
		GLClasses::IndexBuffer m_IBO;
		GLClasses::Shader m_DefaultShader;

		GLClasses::VertexArray m_FBOVAO;
		GLClasses::VertexBuffer m_FBOVBO;
		GLClasses::Shader m_FBOShader;
		GLFWwindow* m_Window;
		GLClasses::SSBO m_LightSSBO;

		std::vector<DirectionalLight> m_DirectionalLights;
	};
}