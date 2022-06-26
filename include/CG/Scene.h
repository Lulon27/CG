#pragma once

#include <vector>
#include <memory>

#include "CG/Object.h"
#include "CG/Camera.h"

namespace cg
{
	class Scene
	{
	public:
		void renderScene();
		void addObject(std::shared_ptr<Object> obj);

		Camera& getCamera() { return m_camera; }

		void setGlobalDirectionalLight(const glm::vec3& light) { m_globalDirLight = light; }
		const glm::vec3& getGlobalDirectionalLight() const { return m_globalDirLight; }

		void setUseViewLight(bool b) { m_useViewLight = b; }
		bool getUseViewLight() const { return m_useViewLight; }

	private:
		std::vector<std::shared_ptr<Object>> m_objects;
		Camera m_camera;

		glm::vec3 m_globalDirLight = glm::vec3(0.0f, 1.0f, 0.0f);
		bool m_useViewLight = false;
	};
}