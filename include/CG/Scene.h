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
	private:
		std::vector<std::shared_ptr<Object>> m_objects;
		Camera m_camera;
	};
}