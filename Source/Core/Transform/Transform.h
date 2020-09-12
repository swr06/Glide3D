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
		void Translate(const glm::vec3& position);
		void Rotate(float angle, const glm::vec3& axis = glm::vec3(1.0f, 1.0f, 1.0f));
		void Scale(const glm::vec3& scale);

		const glm::mat4& GetTransformationMatrix();

	private :
		glm::mat4 m_TransformationMatrix;
	};
}