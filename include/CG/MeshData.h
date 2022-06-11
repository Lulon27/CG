#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace cg
{
	struct MeshData
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> colors;
		std::vector<GLushort> indices;

		GLenum drawMode = GL_TRIANGLES;
	};
}
