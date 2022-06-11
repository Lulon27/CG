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
		Object();
		~Object();

		void setShader(GLSLProgram* shader);
		void setMesh(const MeshData& mesh);

		GLuint getVAO() { return m_vao; }
		GLSLProgram* getShader() const { return m_shader; }
		const glm::mat4x4& getModelMatrix() const { return m_model; }
		unsigned int getIndexBufferSize() const { return m_meshInfo->getIndexBufferSize(); }
		GLenum getDrawMode() const { return m_meshInfo->getDrawMode(); }

		void setModelMatrix(const glm::mat4x4& mat) { m_model = mat; }

	private:
		Object(const Object&) = delete;
		Object(Object&&) = delete;

		Object& operator=(const Object&) = delete;
		Object& operator=(Object&&) = delete;

	private:
		glm::mat4x4 m_model = glm::mat4x4(1.0f);

		GLSLProgram* m_shader = nullptr;
		std::shared_ptr<MeshGLInfo> m_meshInfo = nullptr;

		// vertex-array-object ID
		// Updated if shader or mesh info changes
		// If the VAO is not set (0), the object won't be rendered
		GLuint m_vao = 0;
	};
}
