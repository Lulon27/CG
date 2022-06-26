#include "CG/VertexArrayObject.h"

#include <iostream>

namespace cg
{
	VertexArrayObject::VertexArrayObject()
	{
		// Do not generate VAO by default
	}

	VertexArrayObject::~VertexArrayObject()
	{
		// Delete if exists
		deleteVAO();
	}

	void VertexArrayObject::generateVAO()
	{
		if (m_vao == 0)
		{
			glGenVertexArrays(1, &m_vao);
			std::cout << "Generated Vertex Array: " << m_vao << "\n";
		}
		else
		{
			std::cout << "Attempted VAO generation but was already generated\n";
		}
	}

	void VertexArrayObject::deleteVAO()
	{
		if (m_vao > 0)
		{
			std::cout << "Deleted Vertex Array: " << m_vao << "\n";
			glDeleteVertexArrays(1, &m_vao);
			m_vao = 0;
		}
	}

	bool VertexArrayObject::bindShaderAttribVec3f(GLuint buffer, GLSLProgram* shader, const std::string& attribName)
	{
		GLint location;
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);

		location = glGetAttribLocation(shader->getHandle(), attribName.c_str());
		if (location == -1)
		{
			std::cerr << "Failed to init set up VAO: " << m_vao << '\n';
			std::cerr << "Shader program " << shader->getHandle() << " does not have attribute " << attribName << '\n';
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			return false;
		}
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return true;
	}

	bool VertexArrayObject::bindIndexBuffer(GLuint buffer)
	{
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return true;
	}
}