#pragma once

#include "CG/GLSLProgram.h"

#include <unordered_map>
#include <string>

namespace cg
{
	class ShaderManager
	{
	public:
		static bool loadShader(const std::string& name, std::initializer_list<std::pair<std::string, GLSLShader::GLSLShaderType>> list);
		static GLSLProgram* getShader(const std::string& name);
		static int getShaderID(const std::string& name);
	};
}
