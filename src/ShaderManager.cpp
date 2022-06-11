#include "CG/ShaderManager.h"

namespace cg
{
	bool ShaderManager::loadShader(const std::string& name, std::initializer_list<std::pair<std::string, GLSLShader::GLSLShaderType>> list)
	{
        // Put an empty program into the map
		GLSLProgram& program = m_programs.emplace(std::piecewise_construct, std::make_tuple(name), std::make_tuple()).first->second;

        for (const auto& [path, type] : list)
        {
            if (!program.compileShaderFromFile(path.c_str(), type))
            {
                std::cerr << program.log();
                return false;
            }
        }

        if (!program.link())
        {
            std::cerr << program.log();
            return false;
        }

        return true;
	}

    GLSLProgram* ShaderManager::getShader(const std::string& name)
    {
        return &m_programs[name];
    }

    int ShaderManager::getShaderID(const std::string& name)
    {
        return m_programs[name].getHandle();
    }
}