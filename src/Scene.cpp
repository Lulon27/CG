#include "CG/Scene.h"

#include "CG/GLSLProgram.h"

#include <glm/glm.hpp>

namespace cg
{
	static const glm::vec3 center(0.0f, 0.0f, 0.0f);
	static const glm::vec3 up(0.0f, 1.0f, 0.0f);

	void Scene::addObject(std::shared_ptr<Object> obj)
	{
		m_objects.push_back(obj);
	}

	void Scene::renderScene()
	{
		GLuint vaoID;
		glm::mat4x4 mvp;
		GLSLProgram* shader;

		glm::mat4x4 view = glm::lookAt(m_camera.getPosition(), center, up);

		for(const std::shared_ptr<Object>& obj : m_objects)
		{ 
			vaoID = obj->getVAO();
			if (vaoID == 0)
			{
				// Doesn't have a VAO, cannot be rendered
				// This can happen if there was an initialization error with the object
				continue;
			}

			shader = obj->getShader();
			glUseProgram(shader->getHandle());
			mvp = m_camera.getProjection() * view * obj->getModelMatrix();
			shader->setUniform("mvp", mvp);
			glBindVertexArray(vaoID);
			glDrawElements(obj->getDrawMode(), obj->getIndexBufferSize(), GL_UNSIGNED_SHORT, 0);
			glBindVertexArray(0);
		}
	}
}