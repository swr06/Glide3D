#include "CubeRenderer.h"

namespace Glide3D
{
	CubeRenderer::CubeRenderer() : m_VBO(GL_ARRAY_BUFFER)
	{
        m_DefaultShader.CreateShaderProgramFromFile("Core/Shaders/CubeRendererVert.glsl", "Core/Shaders/CubeRendererFrag.glsl");
        m_DefaultShader.CompileShaders();

        float vertices[] = {

            // back face
            0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
            1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
            0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
            0.0f, 0.0f, 0.0f,  0.0f, 0.0f,

            // front face
            0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
            1.0f, 0.0f,  1.0f, 1.0f, 0.0f,
            1.0f, 1.0f,  1.0f, 1.0f, 1.0f,
            1.0f, 1.0f,  1.0f, 1.0f, 1.0f,
            0.0f, 1.0f,  1.0f, 0.0f, 1.0f,
            0.0f, 0.0f,  1.0f, 0.0f, 0.0f,

            // left
            0.0f, 1.0f, 1.0f,  1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
            0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
            0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
            0.0f, 1.0f, 1.0f,  1.0f, 0.0f,

            // right
            1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
            1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
            1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
            1.0f, 1.0f, 1.0f,  1.0f, 0.0f,

             // bottom
            0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
            1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
            1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
            1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

            // top
            0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
            1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
            1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
            0.0f, 1.0f, 1.0f,  0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,  0.0f, 1.0f
        };

        m_VAO.Bind();
        m_VBO.BufferData(sizeof(vertices), vertices, GL_STATIC_DRAW);
        m_VBO.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        m_VBO.VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        m_VAO.Unbind();
	}

	CubeRenderer::~CubeRenderer()
	{

	}

    void CubeRenderer::RenderCube(const glm::vec3& position, GLClasses::Texture* texture, float rotation, const glm::mat4& projection, const glm::mat4& view, GLClasses::Shader* shader)
	{
        GLClasses::Shader* use_shader;

        if (shader == nullptr)
        {
            use_shader = &m_DefaultShader;
        }

        else
        {
            use_shader = shader;
        }

        glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(1.0f, 0.5f, 0.5f));

        //texture->Bind(1);

        use_shader->Use();
        use_shader->SetMatrix4("u_Model", model);
        use_shader->SetMatrix4("u_View", view);
        use_shader->SetMatrix4("u_Projection", projection);
        use_shader->SetInteger("u_Texture", 1);

        m_VAO.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        m_VAO.Unbind();
	}
}