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

		const glm::mat4& GetTransformationMatrix() const;

	private :
		glm::mat4 m_TransformationMatrix = glm::mat4(1.0f);
	};
}