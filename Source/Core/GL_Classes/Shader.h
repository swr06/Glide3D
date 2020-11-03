#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <chrono>
#include <unordered_map>
#include <filesystem>

namespace GLClasses
{
	class Shader
	{
	public:

		Shader() {}; 

		Shader(const Shader&) = delete;
		Shader operator=(Shader const&) = delete;
		Shader(Shader&& v)
		{
			Location_map = v.Location_map;
			m_Program = v.m_Program;
			m_VertexData = v.m_VertexData;
			m_VertexPath = v.m_VertexPath;
			m_FragmentData = v.m_FragmentData;
			m_FragmentPath = v.m_FragmentPath;

			v.m_Program = 0;
		}


		~Shader();

		void CompileShaders();
		void CreateShaderProgramFromFile(const std::string& vertex_pth, const std::string& fragment_pth, const std::string& geometry_path = "");
		void CreateShaderProgramFromString(const std::string& vertex_data, const std::string& fragment_data, const std::string& geometry_data = "");
		inline GLuint GetProgramID() const { return m_Program; };
		
		inline void Use() 
		{
			glUseProgram(this->m_Program);
		}

		void Destroy();
		void ValidateProgram();
		void SetFloat(const std::string& name, GLfloat value, GLboolean useShader = GL_FALSE);
		void SetInteger(const std::string& name, GLint value, GLboolean useShader = GL_FALSE);
		void SetBool(const std::string& name, bool value, GLboolean useShader = GL_FALSE);
		void SetVector2f(const std::string& name, GLfloat x, GLfloat y, GLboolean useShader = GL_FALSE);
		void SetVector2f(const std::string& name, const glm::vec2& value, GLboolean useShader = GL_FALSE);
		void SetVector3f(const std::string& name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = GL_FALSE);
		void SetVector3f(const std::string& name, const glm::vec3& value, GLboolean useShader = GL_FALSE);
		void SetVector4f(const std::string& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = GL_FALSE);
		void SetVector4f(const std::string& name, const glm::vec4& value, GLboolean useShader = GL_FALSE);
		void SetMatrix4(const std::string& name, const glm::mat4& matrix, GLboolean useShader = GL_FALSE);
		void SetMatrix3(const std::string& name, const glm::mat3& matrix, GLboolean useShader = GL_FALSE);
		void SetIntegerArray(const std::string& name, const GLint* value, GLsizei count, GLboolean useShader = GL_FALSE);
		void SetTextureArray(const std::string& name, const GLuint first, const GLuint count, GLboolean useShader = GL_FALSE);

	 private:

		std::unordered_map<std::string, GLint> Location_map; // To avoid unnecessary calls to glGetUniformLocation()
		GLuint m_Program = 0;

		GLint GetUniformLocation(const std::string& uniform_name);
		std::string m_VertexData;
		std::string m_VertexPath;
		std::string m_FragmentData;
		std::string m_FragmentPath;
		std::string m_GeometryData;
		std::string m_GeometryPath;
	};
}
