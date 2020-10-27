#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Glide3D
{
	class Transform
	{
	public : 
		void Translate(const glm::vec3& position) noexcept;
		void Rotate(float angle, const glm::vec3& axis = glm::vec3(1.0f, 1.0f, 1.0f)) noexcept;
		void Scale(const glm::vec3& scale) noexcept;
		void SetPosition(const glm::vec3& position) noexcept;
		const glm::vec3& GetPosition() const noexcept { return m_Position; }

		const glm::mat4& GetTransformationMatrix() const;
		const glm::mat3& GetNormalMatrix() const;

	private :
		glm::mat4 m_TransformationMatrix = glm::mat4(1.0f);
		glm::mat3 m_NormalMatrix = glm::mat3(1.0f);
		glm::vec3 m_Position = glm::vec3(0.0f);

		inline void ComputeNormalMatrix();
		friend class Renderer;
	};
}