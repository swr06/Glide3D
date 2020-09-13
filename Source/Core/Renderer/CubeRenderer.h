#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../GL_Classes/VertexBuffer.h"
#include "../GL_Classes/IndexBuffer.h"
#include "../GL_Classes/VertexArray.h"
#include "../GL_Classes/Shader.h"
#include "../GL_Classes/Texture.h"

namespace Glide3D
{
	struct Cube
	{
		float length;
		float height;
		float breadth;
	};

	class CubeRenderer
	{
	public :
		CubeRenderer();
		~CubeRenderer();
		void RenderCube(const glm::vec3& position, GLClasses::Texture* texture, float rotation, const glm::mat4& projection, const glm::mat4& view = glm::mat4(1.0f), GLClasses::Shader* shader = nullptr);

	private :

		GLClasses::VertexBuffer m_VBO;
		GLClasses::IndexBuffer m_IBO;
		GLClasses::VertexArray m_VAO;
		GLClasses::Shader m_DefaultShader;
	};
}
