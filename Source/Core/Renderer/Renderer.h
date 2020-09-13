#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <glad/glad.h>

#include "../Object/Object.h"
#include "../Entity/Entity.h"
#include "../Utils/Vertex.h"
#include "../Utils/Defs.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* OpenGL Specific stuff */
#include "../GL_Classes/VertexBuffer.h"
#include "../GL_Classes/IndexBuffer.h"
#include "../GL_Classes/VertexArray.h"

namespace Glide3D
{
	class Renderer
	{
	public :
		Renderer();
		void RenderObjects(const std::vector<Entity>& entities);

	private :
		GLClasses::VertexArray m_VAO;
		GLClasses::VertexBuffer m_VBO;
		GLClasses::VertexBuffer m_MatrixVBO;
		static GLClasses::IndexBuffer m_IBO;
	};
}