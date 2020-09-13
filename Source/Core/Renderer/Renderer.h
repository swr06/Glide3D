#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <glad/glad.h>

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

namespace Glide3D
{
	class Renderer
	{
	public :
		Renderer();
		void RenderObjects(const std::vector<Entity>& entities, FPSCamera* camera);

	private :
		GLClasses::VertexArray m_VAO;
		GLClasses::VertexBuffer m_VBO;
		GLClasses::VertexBuffer m_MatrixVBO;
		GLClasses::Shader m_DefaultShader;
	};
}