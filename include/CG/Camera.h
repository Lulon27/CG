#pragma once

#include "glm/glm.hpp"

namespace cg
{
	class Camera
	{
	public:
		const glm::vec3& getPosition() const { return m_position; }
		const glm::mat4x4& getProjection() const { return m_projection; }

		void setPosition(const glm::vec3& position) { m_position = position; }

		void calculateProjection(float fovyDeg, float aspect, float zNear, float zFar)
		{
			m_projection = glm::perspective(glm::radians(fovyDeg), aspect, zNear, zFar);
		}

	private:
		glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::mat4x4 m_projection = glm::mat4x4(1.0f);
	};
}