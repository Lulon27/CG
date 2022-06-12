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

	void drawWithTransform(const std::shared_ptr<Object>& obj, glm::mat4x4 transform, GLuint vaoID, const glm::mat4x4& projView)
	{
		// Translation
		transform = glm::translate(transform, obj->position);
		// Rotation
		transform = glm::rotate(transform, obj->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		transform = glm::rotate(transform, obj->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		transform = glm::rotate(transform, obj->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

		for (const std::shared_ptr<Object>& childObj : obj->getChildren())
		{
			if (childObj->getVAO() == 0)
			{
				continue;
			}
			// Recursively draw children by continuing with the current model matrix
			// In order to transform them relative to their parent
			drawWithTransform(childObj, transform, childObj->getVAO(), projView);
		}

		// MVP
		transform = projView * transform;

		GLSLProgram* shader = obj->getShader();
		glUseProgram(shader->getHandle());
		shader->setUniform("mvp", transform);
		glBindVertexArray(vaoID);
		glDrawElements(obj->getDrawMode(), obj->getIndexBufferSize(), GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(0);
	}

	void Scene::renderScene()
	{
		GLuint vaoID;
		glm::mat4x4 projView = m_camera.getProjection() * glm::lookAt(m_camera.getPosition(), center, up);

		for(const std::shared_ptr<Object>& obj : m_objects)
		{ 
			vaoID = obj->getVAO();
			if (vaoID == 0)
			{
				// Doesn't have a VAO, cannot be rendered
				// This can happen if there was an initialization error with the object
				continue;
			}

			drawWithTransform(obj, glm::mat4x4(1.0f), vaoID, projView);
		}
	}
}