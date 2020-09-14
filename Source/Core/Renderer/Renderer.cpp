#include "Renderer.h"

namespace Glide3D
{
	constexpr unsigned int indices_count = 10000;

	Renderer::Renderer(GLFWwindow* window) : m_VBO(GL_ARRAY_BUFFER), m_MatrixVBO(GL_ARRAY_BUFFER), 
		m_FBOVBO(GL_ARRAY_BUFFER), m_Window(window)
	{
		bool IndexBufferInitialized = false;

		/*
		Setup all the opengl buffer and array objects
		*/
		m_VAO.Bind();
		m_IBO.Bind();
		m_VBO.Bind();
		m_VBO.VertexAttribPointer(0, 3, GL_FLOAT, 0, sizeof(Vertex), (void*)(offsetof(Vertex, position)));
		m_VBO.VertexAttribPointer(1, 3, GL_FLOAT, 0, sizeof(Vertex), (void*)(offsetof(Vertex, normals)));
		m_VBO.VertexAttribPointer(2, 2, GL_FLOAT, 0, sizeof(Vertex), (void*)(offsetof(Vertex, tex_coords)));
		m_MatrixVBO.Bind();

		// Structure padding shouldn't be an issue since the size is 16
		m_MatrixVBO.VertexAttribPointer(3, 4, GL_FLOAT, 0, 4 * 4 * sizeof(GLfloat), (void*)0); // column 1
		m_MatrixVBO.VertexAttribPointer(4, 4, GL_FLOAT, 0, 4 * 4 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 4)); // column 2
		m_MatrixVBO.VertexAttribPointer(5, 4, GL_FLOAT, 0, 4 * 4 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 8)); // column 3
		m_MatrixVBO.VertexAttribPointer(6, 4, GL_FLOAT, 0, 4 * 4 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 12)); // column 4

		// Set the matrix attributes to be per-instance
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		m_VAO.Unbind();

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
		m_DefaultShader.CreateShaderProgramFromFile("Core/Shaders/RendererVert.glsl", "Core/Shaders/RendererFrag.glsl");
		m_DefaultShader.CompileShaders();
		m_FBOShader.CreateShaderProgramFromFile("Core/Shaders/FramebufferVert.glsl", "Core/Shaders/FramebufferFrag.glsl");
		m_FBOShader.CompileShaders();
	}

	/*
	Renders a group of the same entity at different positions using instanced rendering
	*/
	void Renderer::RenderObjects(const std::vector<Entity>& entities, FPSCamera* camera)
	{
		unsigned int entity_num = 0;

		const std::vector<Vertex>& Vertices = entities[0].p_Object->p_Vertices;
		const std::vector<GLuint>& Indices = entities[0].p_Object->p_Indices;
		std::vector<glm::mat4> ModelMatrices;

		for (auto& e : entities)
		{
			ModelMatrices.push_back(e.p_Transform.GetTransformationMatrix());
			entity_num++;
		}

		bool indexed = false;

		m_DefaultShader.Use();
		m_DefaultShader.SetMatrix4("u_ViewProjection", camera->GetViewProjection());

		if (Indices.size() > 0)
		{
			m_IBO.BufferData(Indices.size() * sizeof(GLuint), (void*)&Indices.front(), GL_STATIC_DRAW);
			indexed = true;
		}

		m_VBO.BufferData(Vertices.size() * sizeof(Vertex), (void*)&Vertices.front(), GL_STATIC_DRAW);
		m_MatrixVBO.BufferData(ModelMatrices.size() * 4 * 4 * sizeof(GLfloat), &ModelMatrices.front(), GL_STATIC_DRAW);
		m_VAO.Bind();

		if (indexed)
		{
			GLCall(glDrawElementsInstanced(GL_TRIANGLES, (Vertices.size() / 4) * 6, GL_UNSIGNED_INT, 0, entities.size()));
		}

		else
		{
			GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, Vertices.size(), entities.size()));
		}
	
		m_VAO.Unbind();
		glUseProgram(0);

		return;
	}

	/*
	Renders a framebuffer onto the main window
	*/
	void Renderer::RenderFBO(const GLClasses::Framebuffer& fbo)
	{
		glDisable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		m_FBOShader.Use();
		m_FBOShader.SetInteger("u_FramebufferTexture", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, fbo.GetTexture());
		m_FBOVAO.Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
		m_FBOVAO.Unbind();

		glUseProgram(0);
	}
}
