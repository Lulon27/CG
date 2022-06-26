#include "CG/Scene.h"

#include "CG/GLSLProgram.h"
#include "CG/VertexArrayObject.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>

namespace cg
{
	static const glm::vec3 center(0.0f, 0.0f, 0.0f);
	static const glm::vec3 up(0.0f, 1.0f, 0.0f);

	void Scene::addObject(std::shared_ptr<Object> obj)
	{
		m_objects.push_back(obj);
	}

	void drawWithTransform(const std::shared_ptr<Object>& obj, glm::mat4x4 transform, VertexArrayObject& vao, const glm::mat4x4& proj, const glm::mat4x4& view, const glm::vec4& lightVec)
	{
		// Translation
		transform = glm::translate(transform, obj->position);
		// Rotation
		transform = glm::rotate(transform, glm::radians(obj->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		transform = glm::rotate(transform, glm::radians(obj->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		transform = glm::rotate(transform, glm::radians(obj->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		for (const std::shared_ptr<Object>& childObj : obj->getChildren())
		{
			// Recursively draw children by continuing with the current model matrix
			// In order to transform them relative to their parent
			drawWithTransform(childObj, transform, childObj->getVAO(), proj, view, lightVec);
		}

		if (vao.getVAO() == 0)
		{
			// Doesn't have a VAO, cannot be rendered
			// This can happen if there was an initialization error with the object
			return;
		}

		glm::mat3 nm = glm::inverseTranspose(glm::mat3(transform));
		glm::mat4 mv = view * transform;

		// MVP
		glm::mat4 mvp = proj * view * transform;


		GLSLProgram* shader = obj->getShader();
		glUseProgram(shader->getHandle());

		// Gouraud
		shader->setUniform("light", lightVec);
		shader->setUniform("lightI", float(1.0f));
		shader->setUniform("surfKa", glm::vec3(0.1f, 0.1f, 0.1f));
		shader->setUniform("surfKd", glm::vec3(obj->getColor()));
		shader->setUniform("surfKs", glm::vec3(1, 1, 1));
		shader->setUniform("surfShininess", float(8.0f));

		// Shaded
		shader->setUniform("mvp", mvp);
		shader->setUniform("lightDirection", glm::vec3(0.0f, 0.0f, -1.0f));

		shader->setUniform("modelviewMatrix", mv);
		shader->setUniform("normalMatrix", nm);
		shader->setUniform("projectionMatrix", proj);

		glBindVertexArray(vao.getVAO());
		glDrawElements(obj->getDrawMode(), obj->getIndexBufferSize(), GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(0);
	}

	void Scene::renderScene()
	{
		glm::mat4x4 proj = m_camera.getProjection();
		glm::mat4x4 view = glm::lookAt(m_camera.getPosition(), center, up);

		auto light = getUseViewLight() ? glm::vec4(0, 0, 0, 1) : glm::vec4(getGlobalDirectionalLight(), 0.0f);

		for(const std::shared_ptr<Object>& obj : m_objects)
		{ 
			drawWithTransform(obj, glm::mat4x4(1.0f), obj->getVAO(), proj, view, light);
		}
	}
}