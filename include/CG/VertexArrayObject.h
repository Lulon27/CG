#pragma once

#include <glad/glad.h>

#include "CG/GLSLProgram.h"

namespace cg
{
	class VertexArrayObject
	{
	public:
		VertexArrayObject();
		~VertexArrayObject();

		GLuint getVAO() { return m_vao; }

		void generateVAO();
		void deleteVAO();

		bool bindShaderAttribVec3f(GLuint buffer, GLSLProgram* shader, const std::string& attribName);

		bool bindIndexBuffer(GLuint buffer);

	private:
		VertexArrayObject(const VertexArrayObject&) = delete;
		VertexArrayObject(VertexArrayObject&&) = delete;

		VertexArrayObject& operator=(const VertexArrayObject&) = delete;
		VertexArrayObject& operator=(VertexArrayObject&&) = delete;

	private:
		GLuint m_vao = 0;
	};
}
