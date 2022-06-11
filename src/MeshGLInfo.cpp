#include "CG/MeshGLInfo.h"

#include <glad/glad.h>

namespace cg
{
	MeshGLInfo::MeshGLInfo()
	{
		glGenBuffers(1, &m_positionBuffer);
		glGenBuffers(1, &m_colorBuffer);
		glGenBuffers(1, &m_indexBuffer);
	}

	MeshGLInfo::~MeshGLInfo()
	{
		glDeleteBuffers(1, &m_indexBuffer);
		glDeleteBuffers(1, &m_colorBuffer);
		glDeleteBuffers(1, &m_positionBuffer);
	}

	std::shared_ptr<MeshGLInfo> MeshGLInfo::generate(const MeshData& meshData)
	{
        std::shared_ptr<MeshGLInfo> info = std::make_shared<MeshGLInfo>();


        glBindBuffer(GL_ARRAY_BUFFER, info->m_positionBuffer);
        glBufferData(GL_ARRAY_BUFFER, meshData.vertices.size() * sizeof(glm::vec3), meshData.vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, info->m_colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, meshData.colors.size() * sizeof(glm::vec3), meshData.colors.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, info->m_indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.indices.size() * sizeof(GLushort), meshData.indices.data(), GL_STATIC_DRAW);

		info->m_drawAmount = meshData.indices.size();
		info->m_drawMode = meshData.drawMode;

        return info;
	}
}