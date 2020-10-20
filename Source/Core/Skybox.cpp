#include "Skybox.h"

namespace Glide3D
{
	Skybox::Skybox(const skybox_paths& paths) : m_VBO(GL_ARRAY_BUFFER)
	{
        GLfloat SkyboxVertices[] =
        {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

        m_CubeMap.CreateCubeTextureMap({
            paths.right,
            paths.left,
            paths.top,
            paths.bottom,
            paths.front,
            paths.back
            });

        m_SkyboxShader.CreateShaderProgramFromFile("Core/Shaders/SkyboxVertex.glsl", "Core/Shaders/SkyboxFrag.glsl");
        m_SkyboxShader.CompileShaders();

        m_VAO.Bind();
        m_VBO.BufferData(sizeof(SkyboxVertices), SkyboxVertices, GL_STATIC_DRAW);
        m_VBO.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        m_VAO.Unbind();
	}

    void Skybox::RenderSkybox(FPSCamera* camera)
    {
        glDepthMask(GL_FALSE);
        glDisable(GL_CULL_FACE);
        m_SkyboxShader.Use();

        m_SkyboxShader.SetMatrix4("u_Projection", camera->GetProjectionMatrix());
        m_SkyboxShader.SetMatrix4("u_View", glm::mat4(glm::mat3(camera->GetViewMatrix())));
        m_SkyboxShader.SetInteger("u_Skybox", 0);

        m_VAO.Bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeMap.GetID());
        (glDrawArrays(GL_TRIANGLES, 0, 36));

        m_VAO.Unbind();

        glDepthMask(GL_TRUE);
    }

    void Skybox::RenderSkybox(const glm::mat4& projection, const glm::mat4& view)
    {
        glDepthMask(GL_FALSE);
        glDisable(GL_CULL_FACE);
        m_SkyboxShader.Use();

        m_SkyboxShader.SetMatrix4("u_Projection", projection);
        m_SkyboxShader.SetMatrix4("u_View", glm::mat4(glm::mat3(view))); // remove translation
        m_SkyboxShader.SetInteger("u_Skybox", 0);

        m_VAO.Bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeMap.GetID());
        (glDrawArrays(GL_TRIANGLES, 0, 36));

        m_VAO.Unbind();

        glDepthMask(GL_TRUE);
    }
}
