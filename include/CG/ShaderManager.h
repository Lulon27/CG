#pragma once

#include "CG/GLSLProgram.h"

#include <unordered_map>
#include <string>

namespace cg
{
	class ShaderManager
	{
	public:
		bool loadShader(const std::string& name, std::initializer_list<std::pair<std::string, GLSLShader::GLSLShaderType>> list);
		GLSLProgram* getShader(const std::string& name);
		int getShaderID(const std::string& name);

	private:
		std::unordered_map<std::string, GLSLProgram> m_programs;
	};
}
