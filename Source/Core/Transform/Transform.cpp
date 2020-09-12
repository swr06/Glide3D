#include "Transform.h"

namespace Glide3D
{
	void Transform::Translate(const glm::vec3& position)
	{
		m_TransformationMatrix = glm::translate(m_TransformationMatrix, position);
	}

	void Transform::Rotate(float angle, const glm::vec3& axis)
	{
		m_TransformationMatrix = glm::rotate(m_TransformationMatrix, angle, axis);
	}

	void Transform::Scale(const glm::vec3& scale)
	{
		m_TransformationMatrix = glm::scale(m_TransformationMatrix, scale);
	}

	const glm::mat4& Transform::GetTransformationMatrix()
	{
		return m_TransformationMatrix;
	}
}