#include "CG/Object.h"

namespace cg
{
	static void updateVAO(GLuint& vao, MeshGLInfo* meshInfo, GLSLProgram* shader);

	Object::Object(const std::string& debugName)
		: m_debugName(debugName)
	{

	}

	Object::~Object()
	{
		if (m_vao > 0)
		{
			std::cout << "Deleted Vertex Array: " << m_vao << "\n";
			glDeleteVertexArrays(1, &m_vao);
		}
	}

	void Object::setShader(GLSLProgram* shader)
	{
		m_shader = shader;
		updateVAO(m_vao, m_meshInfo.get(), m_shader);
	}

	void Object::setMesh(const MeshData& mesh)
	{
		m_meshInfo = MeshGLInfo::generate(mesh);
		updateVAO(m_vao, m_meshInfo.get(), m_shader);
	}

	void updateVAO(GLuint& vao, MeshGLInfo* meshInfo, GLSLProgram* shader)
	{
		if (meshInfo == nullptr || shader == nullptr)
		{
			if (vao > 0)
			{
				std::cout << "Deleted Vertex Array: " << vao << "\n";
				glDeleteVertexArrays(1, &vao);
				vao = 0;
			}
			return;
		}

		if (vao > 0)
		{
			std::cout << "Deleted Vertex Array: " << vao << "\n";
			glDeleteVertexArrays(1, &vao);
			vao = 0;
		}
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		std::cout << "Generated Vertex Array: " << vao << "\n";


		GLint location;
		//Enable position attribute
		glBindBuffer(GL_ARRAY_BUFFER, meshInfo->getPositionBufferID());
		location = glGetAttribLocation(shader->getHandle(), "position");
		if (location == -1)
		{
			std::cerr << "Failed to init set up VAO: " << vao << "\n";
			std::cerr << "Shader program " << shader->getHandle() << " does not have attribute \"position\"\n";
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDeleteVertexArrays(1, &vao);
			std::cout << "Deleted Vertex Array: " << vao << "\n";
			vao = 0;
			return;
		}
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, 0);


		//Enable color attribute
		glBindBuffer(GL_ARRAY_BUFFER, meshInfo->getColorBufferID());
		location = glGetAttribLocation(shader->getHandle(), "color");
		if (location == -1)
		{
			std::cerr << "Shader program " << shader->getHandle() << " does not have attribute \"color\"\n";
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDeleteVertexArrays(1, &vao);
			std::cout << "Deleted Vertex Array: " << vao << "\n";
			vao = 0;
			return;
		}
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshInfo->getIndexBufferID());

		glBindVertexArray(0);
	}
}
