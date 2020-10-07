#include "Transform.h"

namespace Glide3D
{
	void Transform::Translate(const glm::vec3& position) noexcept
	{
		m_Position += position;
		m_TransformationMatrix = glm::translate(m_TransformationMatrix, position);
		ComputeNormalMatrix();
	}

	void Transform::Rotate(float angle, const glm::vec3& axis) noexcept
	{
		m_TransformationMatrix = glm::rotate(m_TransformationMatrix, angle, axis);
		ComputeNormalMatrix();
	}

	void Transform::Scale(const glm::vec3& scale) noexcept
	{
		m_TransformationMatrix = glm::scale(m_TransformationMatrix, scale);
		ComputeNormalMatrix();
	}

	void Transform::SetPosition(const glm::vec3& position) noexcept
	{
		m_TransformationMatrix = glm::translate(m_TransformationMatrix, -m_Position);
		m_Position = position;
		m_TransformationMatrix = glm::translate(m_TransformationMatrix, m_Position);
		ComputeNormalMatrix();
	}

	const glm::mat4& Transform::GetTransformationMatrix() const
	{
		return m_TransformationMatrix;
	}

	const glm::mat3& Transform::GetNormalMatrix() const
	{
		return m_NormalMatrix;
	}

	inline void Transform::ComputeNormalMatrix()
	{
		m_NormalMatrix = glm::mat3(glm::transpose(glm::inverse(m_TransformationMatrix)));
		return;
	}

}