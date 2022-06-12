#pragma once

#include <glm/glm.hpp>
#include <string>
#include <memory>

#include "CG/MeshGLInfo.h"
#include "CG/MeshData.h"
#include "CG/GLSLProgram.h"

namespace cg
{
	class Object
	{
	public:
		Object(const std::string& debugName = "Object");
		~Object();

		void setShader(GLSLProgram* shader);
		void setMesh(const MeshData& mesh);

		GLuint getVAO() { return m_vao; }
		GLSLProgram* getShader() const { return m_shader; }
		unsigned int getIndexBufferSize() const { return m_meshInfo->getIndexBufferSize(); }
		GLenum getDrawMode() const { return m_meshInfo->getDrawMode(); }

		void addChild(std::shared_ptr<Object> obj) { m_children.push_back(obj); }
		const std::vector< std::shared_ptr<Object>>& getChildren() { return m_children; }

		void rotateAroundOrigin(float deg, const glm::vec3& axis);

		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

		// Relative rotation in degrees
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	private:
		Object(const Object&) = delete;
		Object(Object&&) = delete;

		Object& operator=(const Object&) = delete;
		Object& operator=(Object&&) = delete;

	private:
		GLSLProgram* m_shader = nullptr;
		std::shared_ptr<MeshGLInfo> m_meshInfo = nullptr;

		// vertex-array-object ID
		// Updated if shader or mesh info changes
		// If the VAO is not set (0), the object won't be rendered
		GLuint m_vao = 0;

		std::vector< std::shared_ptr<Object>> m_children;

		std::string m_debugName;
	};
}
